// Given libraries
#include "parser.h"
#include "ppm.h"

// Standard libraries
#include <iostream>
#include <vector>
#include <cmath>
#include <pthread.h>

// Custom libraries
#include "ImagePlane.h"
#include "GeometricalStructures.h"
#include "Utils.h"

// Macros
#define DOUBLE_MAX 1.79769e+308

// Namespace resolutions
using namespace std;
using namespace parser;
using namespace Utils;

// Global Variables
Scene SCENE;
Camera CAMERA;
Vec3i BACKGROUND_COLOR;
int WIDTH, HEIGHT;
double LEFT, RIGHT, BOTTOM, TOP, DISTANCE_TO_IMAGE_PLANE, EPSILON;
Vec3f CAMERA_POSITION, GAZE_VECTOR, UP_VECTOR, HORIZONTAL_VECTOR, AMBIENT_LIGHT, ZERO_VECTOR, BACKGROUND_COLOR_F; // -w, v, u
int N_SPHERE, N_TRIANGLE, N_MESH, N_POINT_LIGHTS, MAX_RECURSION_DEPTH;
ImagePlane IMAGE_PLANE;
vector<int*> garbage_collector;

void initGlobalVariables(int camera_index) {
    CAMERA = SCENE.cameras[camera_index];
    WIDTH = CAMERA.image_width; HEIGHT = CAMERA.image_height;
    LEFT = CAMERA.near_plane.x; RIGHT = CAMERA.near_plane.y; BOTTOM = CAMERA.near_plane.z; TOP = CAMERA.near_plane.w; 
    CAMERA_POSITION = CAMERA.position;
    DISTANCE_TO_IMAGE_PLANE = CAMERA.near_distance;        
    GAZE_VECTOR = getNormalizedVector(CAMERA.gaze); UP_VECTOR = getNormalizedVector(CAMERA.up); HORIZONTAL_VECTOR = getNormalizedVector(getCrossProduct(GAZE_VECTOR, UP_VECTOR));
    N_SPHERE = SCENE.spheres.size(); N_TRIANGLE = SCENE.triangles.size(); N_MESH = SCENE.meshes.size();
    N_POINT_LIGHTS = SCENE.point_lights.size(); AMBIENT_LIGHT = SCENE.ambient_light;
    MAX_RECURSION_DEPTH = SCENE.max_recursion_depth;
	ZERO_VECTOR.x = ZERO_VECTOR.y = ZERO_VECTOR.z = 0;
	BACKGROUND_COLOR = SCENE.background_color;
	BACKGROUND_COLOR_F.x = BACKGROUND_COLOR.x; BACKGROUND_COLOR_F.y = BACKGROUND_COLOR.y; BACKGROUND_COLOR_F.z = BACKGROUND_COLOR.z; 
	EPSILON = SCENE.shadow_ray_epsilon; 
}

Vec3f getPixelCenterCoordinates(int i, int j) { // pixel on the upper left corner has indices i=0, j=0 , i: increases towards right, j: increases toward bottom
    // Please refer to "Ray tracing geometry" slide page 14 for calculations
    double s_u = (i + 0.5)*(RIGHT - LEFT) / WIDTH;
    double s_v = (j + 0.5)*(TOP - BOTTOM) / HEIGHT;
    Vec3f m = getVectorAddition(CAMERA_POSITION,getScaledVectorByScalar(GAZE_VECTOR, DISTANCE_TO_IMAGE_PLANE));
    Vec3f q = getVectorAddition(m, getVectorAddition(getScaledVectorByScalar(HORIZONTAL_VECTOR, LEFT), getScaledVectorByScalar(UP_VECTOR, TOP)));
    Vec3f s = getVectorAddition(q, getVectorSubtraction(getScaledVectorByScalar(HORIZONTAL_VECTOR, s_u), getScaledVectorByScalar(UP_VECTOR, s_v)));
    return s;
}

Ray getEyeRay(Vec3f pixelCenterCoordinates) { // Ray coming out of camera (e) and passing through pixel center (p = pixelCenterCoordinates)
    Vec3f direction_vector = getVectorSubtraction(pixelCenterCoordinates, CAMERA_POSITION);
    Ray eye_ray; // r(t)
    eye_ray.direction_vector = getNormalizedVector(direction_vector);
    eye_ray.starting_point = CAMERA_POSITION;
    return eye_ray;
}

HitRecord intersectRayWithSphere(Sphere sphere, Ray ray, int indexInCorrespondingArray) {
    // Please refer to "Ray tracing geometry" slide page 23 for calculations
	Vec3f sphere_center = SCENE.vertex_data[sphere.center_vertex_id - 1];
	double A = getDotProduct(ray.direction_vector, ray.direction_vector);
	double B = 2 * getDotProduct(ray.direction_vector, getVectorSubtraction(ray.starting_point, sphere_center));
	double C = getDotProduct(getVectorSubtraction(ray.starting_point, sphere_center), getVectorSubtraction(ray.starting_point, sphere_center)) - sphere.radius * sphere.radius;
	double delta = B*B - 4*A*C;

	if(delta < 0) return HitRecord();
	else {	
		double t1 = (-1 * B + sqrtf(delta))/2*A;
		double t2 = (-1 * B - sqrtf(delta))/2*A;
		double t = t1 < t2 ? t1 : t2; 
		HitRecord hitRecord(indexInCorrespondingArray, t, ray, sphere, SCENE);
		return hitRecord;
	}
}

HitRecord intersectRayWithTriangle(Triangle triangle, Ray ray, int indexInCorrespondingArray) {
    // Please refer to "Ray tracing geometry" slide pages 42 and 44 for calculations    
    Vec3f a = SCENE.vertex_data[triangle.indices.v0_id-1], b = SCENE.vertex_data[triangle.indices.v1_id-1], c =  SCENE.vertex_data[triangle.indices.v2_id-1];

	// Apply Cramer's Rule
	double determinant_A = getDeterminant(getVectorSubtraction(a, b), getVectorSubtraction(a, c), ray.direction_vector);
	if(determinant_A == 0.0) return HitRecord(); // No solution

	double t = (getDeterminant(getVectorSubtraction(a, b), getVectorSubtraction(a, c), getVectorSubtraction(a, ray.starting_point)))/determinant_A;
	if(t <= 0.0) return HitRecord(); // No solution

	double gamma = (getDeterminant(getVectorSubtraction(a, b), getVectorSubtraction(a, ray.starting_point), ray.direction_vector))/determinant_A;
	if(!(gamma >= 0 && gamma <= 1)) return HitRecord(); // No solution

	double beta = (getDeterminant(getVectorSubtraction(a, ray.starting_point), getVectorSubtraction(a, c), ray.direction_vector))/determinant_A;
	if(!(beta >= 0 && beta <= 1)) return HitRecord(); // No solution

	double alpha = 1 - beta - gamma;
	if(!(alpha >= 0 && alpha <= 1)) return HitRecord(); // No solution

	return HitRecord(indexInCorrespondingArray, t, ray, triangle, SCENE);
}

HitRecord intersectRayWithMesh(Mesh mesh, Ray ray, int indexInCorrespondingArray, bool checkingShadow) {
	HitRecord hitRecordClosest;
	double t_min = DOUBLE_MAX;
	int N_FACE = mesh.faces.size();
	for(int i=0;i<N_FACE;i++) {
		Triangle triangle; triangle.material_id = mesh.material_id; triangle.indices.v0_id = mesh.faces[i].v0_id; triangle.indices.v1_id = mesh.faces[i].v1_id; triangle.indices.v2_id = mesh.faces[i].v2_id;
		HitRecord hitRecordMesh = intersectRayWithTriangle(triangle, ray, indexInCorrespondingArray);
		if(hitRecordMesh.isHit && hitRecordMesh.t >= EPSILON && hitRecordMesh.t < t_min) {
			t_min = hitRecordMesh.t;
			hitRecordClosest = HitRecord(indexInCorrespondingArray, t_min, ray, mesh, getSpecificPointOnRay(ray, hitRecordMesh.t), getNormalizedVector(getCrossProduct(getVectorSubtraction(SCENE.vertex_data[mesh.faces[i].v1_id - 1], SCENE.vertex_data[mesh.faces[i].v0_id - 1]), getVectorSubtraction(SCENE.vertex_data[mesh.faces[i].v2_id - 1], SCENE.vertex_data[mesh.faces[i].v0_id - 1]))));
			if (checkingShadow) break;
		}
	}
	return hitRecordClosest;
}

HitRecord intersectRayWithAllObjects(Ray ray, bool checkingShadow, Vec3f pointLightPosition) {
	HitRecord hitRecordClosest; hitRecordClosest.isHit = false;
	double t_min = DOUBLE_MAX; double distance = getDistanceBetweenPoints(pointLightPosition, CAMERA.position); // This is for preventing self intersection
	double t_max = fmin(getVectorSubtraction(pointLightPosition, ray.starting_point).x / ray.direction_vector.x, fmin(getVectorSubtraction(pointLightPosition, ray.starting_point).y / ray.direction_vector.y, getVectorSubtraction(pointLightPosition, ray.starting_point).z / ray.direction_vector.z));
	bool isInShadow = false;

	if (!checkingShadow || !isInShadow) {
		for(int i=0;i<N_SPHERE;i++) {
			HitRecord hitRecordSphere = intersectRayWithSphere(SCENE.spheres[i], ray, i);
			if(hitRecordSphere.isHit && hitRecordSphere.t >= 0 && hitRecordSphere.t < t_min) {
				t_min = hitRecordSphere.t;
				hitRecordClosest = hitRecordSphere;
				if (checkingShadow && t_max > hitRecordSphere.t) {
					isInShadow = true;
					break;
				} 
			}
		}		
	}

	if (!checkingShadow || !isInShadow) {
		for(int i=0;i<N_TRIANGLE;i++) {
			HitRecord hitRecordTriangle = intersectRayWithTriangle(SCENE.triangles[i], ray, i);
			if(hitRecordTriangle.isHit && hitRecordTriangle.t >= 0 && hitRecordTriangle.t < t_min) {
				t_min = hitRecordTriangle.t;
				hitRecordClosest = hitRecordTriangle;
				if (checkingShadow && t_max > hitRecordTriangle.t) {
					isInShadow = true;
					break;
				} 
			}
		}
	}

	if (!checkingShadow || !isInShadow) {
		for(int i=0; i<N_MESH;i++) {	
			HitRecord hitRecordMesh = intersectRayWithMesh(SCENE.meshes[i], ray, i, false);
			if(hitRecordMesh.isHit && hitRecordMesh.t >= 0 && hitRecordMesh.t < t_min) {
				t_min = hitRecordMesh.t;
				hitRecordClosest = hitRecordMesh;
				if (checkingShadow && t_max > hitRecordMesh.t) {
					isInShadow = true;
					break;
				} 
			}
		}
	}

	if ((!isInShadow || (isInShadow && distance == 0)) && checkingShadow) {hitRecordClosest.isHit = false; return hitRecordClosest;}
	else return hitRecordClosest;
}

Vec3f getAmbientComponent(Material material){
	return getDimensionalMultiplication(material.ambient, SCENE.ambient_light);
}

Vec3f getDiffuseComponent(PointLight pointLight, Material material, Vec3f surface_normal, Vec3f pointOnSurface) {
    // Please refer to "Ray tracing shading" slide page 33 for calculations    
    Vec3f received_irradience = getScaledVectorByScalar(pointLight.intensity, pow(getDistanceBetweenPoints(ZERO_VECTOR, getVectorSubtraction(pointLight.position, pointOnSurface)), -2));
    Vec3f l = getNormalizedVector(getVectorSubtraction(pointLight.position, pointOnSurface));
    double cos_theta = fmax(0,getDotProduct(l, getNormalizedVector(surface_normal)));
    return getScaledVectorByScalar(getDimensionalMultiplication(material.diffuse, received_irradience), cos_theta);   
}

Vec3f getSpecularComponent(PointLight pointLight, Ray ray, Material material, Vec3f surface_normal, Vec3f pointOnSurface) {
    // Please refer to "Ray tracing shading" slide pages 43 for calculations            
    Vec3f w_i = getNormalizedVector(getVectorSubtraction(pointLight.position, pointOnSurface));
    Vec3f h = getNormalizedVector(getVectorSubtraction(w_i, ray.direction_vector));
    double cos_alpha = fmax(0, getDotProduct(getNormalizedVector(surface_normal), getNormalizedVector(h)));
    Vec3f received_irradience = getScaledVectorByScalar(pointLight.intensity, pow(getDistanceBetweenPoints(ZERO_VECTOR, getVectorSubtraction(pointLight.position, pointOnSurface)), -2));
    return getScaledVectorByScalar(getDimensionalMultiplication(material.specular, received_irradience), powf(cos_alpha, material.phong_exponent));
}

Vec3f getPixelColorHelper(HitRecord hitRecord, int depth) {
    if(hitRecord.isHit) {
		int material_id = (hitRecord.hitObject == 0 ? hitRecord.sphere.material_id : (hitRecord.hitObject == 1 ? hitRecord.triangle.material_id : hitRecord.mesh.material_id));

		Vec3f color = getAmbientComponent(SCENE.materials[(hitRecord.hitObject == 0 ? hitRecord.sphere.material_id : (hitRecord.hitObject == 1 ? hitRecord.triangle.material_id : hitRecord.mesh.material_id)) - 1]); // Add ambient component regardles of presence of the shadow
		
		for(int i=0;i<N_POINT_LIGHTS;i++) { // Add specular and diffuse component by taking presence of the shadow into account
			Vec3f w_i = getNormalizedVector(getVectorSubtraction(SCENE.point_lights[i].position, hitRecord.pointOnSurface));
			Vec3f w_i_epsiloned = getScaledVectorByScalar(w_i, EPSILON);
			Ray ray; ray.direction_vector = w_i; ray.starting_point = hitRecord.pointOnSurface;
	        if(!intersectRayWithAllObjects(ray, true, SCENE.point_lights[i].position).isHit) { // Add specular and diffuse component if shadow does not exist
		        Vec3f specular = getSpecularComponent(SCENE.point_lights[i], hitRecord.ray, SCENE.materials[(hitRecord.hitObject == 0 ? hitRecord.sphere.material_id : (hitRecord.hitObject == 1 ? hitRecord.triangle.material_id : hitRecord.mesh.material_id))-1], hitRecord.normal, hitRecord.pointOnSurface);
		        Vec3f diffuse = getDiffuseComponent(SCENE.point_lights[i], SCENE.materials[(hitRecord.hitObject == 0 ? hitRecord.sphere.material_id : (hitRecord.hitObject == 1 ? hitRecord.triangle.material_id : hitRecord.mesh.material_id))-1], hitRecord.normal, hitRecord.pointOnSurface);
				color = getVectorAddition(color, getVectorAddition(diffuse, specular));
	        }
    	}

        if(SCENE.materials[(hitRecord.hitObject == 0 ? hitRecord.sphere.material_id : (hitRecord.hitObject == 1 ? hitRecord.triangle.material_id : hitRecord.mesh.material_id)) - 1].is_mirror && depth > 0 ) { // Calculate mirrorComponent caused by reflections
        	Vec3f w_i = getNormalizedVector(getVectorAddition(hitRecord.ray.direction_vector, getScaledVectorByScalar(hitRecord.normal, -2 * getDotProduct(hitRecord.ray.direction_vector, hitRecord.normal))));
        	Ray reflectedRay; reflectedRay.starting_point = getVectorAddition(hitRecord.pointOnSurface, getScaledVectorByScalar(w_i, EPSILON)); reflectedRay.direction_vector = w_i;
            HitRecord hitRecordMirror = intersectRayWithAllObjects(reflectedRay, false, ZERO_VECTOR);
			// Avoid self reflection by checking hitObhect and indexInCorrespondingArray and add mirrorComponents
            if(!(hitRecordMirror.indexInCorrespondingArray == hitRecord.indexInCorrespondingArray && hitRecordMirror.hitObject == hitRecord.hitObject)) color = getVectorAddition(color, getDimensionalMultiplication(getPixelColorHelper(hitRecordMirror, depth-1), SCENE.materials[(hitRecord.hitObject == 0 ? hitRecord.sphere.material_id : (hitRecord.hitObject == 1 ? hitRecord.triangle.material_id : hitRecord.mesh.material_id)) - 1].mirror));
   		}
		
		return color;
  	}

  	return BACKGROUND_COLOR_F; // Actually, this is redundant because I set all pixel colors to background color at the beginning.
}

Vec3i getPixelColor(int i, int j) {
	Vec3f pixelCenterCoordinates = getPixelCenterCoordinates(i,j);
    Ray eye_ray = getEyeRay(pixelCenterCoordinates);
    HitRecord hitResult = intersectRayWithAllObjects(eye_ray, false, ZERO_VECTOR);
	return getPixelColorClapped(getPixelColorHelper(hitResult, MAX_RECURSION_DEPTH));
}

// This is a thread routine responsible for calculating color of pixels in a column
void* traceSubPlane(void* arg) {
    int i = *((int*)arg);  
	for (int j=0;j<HEIGHT;j++) IMAGE_PLANE.setPixelColor(i, j, getPixelColor(i,j)); 
	return NULL;
}

int main(int argc, char* argv[]) {
    // Initial setup
    SCENE.loadFromXml(argv[1]);
    int N_CAMERA = SCENE.cameras.size();

    // For each camera
    for(int i=0; i < N_CAMERA; i++) {
        // Initialize global variables
        initGlobalVariables(i);

		vector<pthread_t> threads; for (int i=0;i<WIDTH;i++){pthread_t thread; threads.push_back(thread);}

        // Initialize image plane
        IMAGE_PLANE = ImagePlane();
        IMAGE_PLANE.init(WIDTH, HEIGHT, SCENE.background_color);

		// Assign a thread to each column for calculations of pixel colors
        for(int i = 0; i < WIDTH; i++) {
			int* index = new int; 
			*index = i;
			garbage_collector.push_back(index);
        	if (pthread_create(&threads[i], NULL, &traceSubPlane, index) != 0) cerr << "An error occured during thread creation!" << endl; 
		} 

		for (int i=0;i<WIDTH;i++) if (pthread_join(threads[i], NULL) != 0) cerr << "An error occured during thread join!" << endl;

		// Save an image for each camera
        write_ppm(CAMERA.image_name.c_str(), IMAGE_PLANE.getImage(), WIDTH, HEIGHT);
    }

	// Free all garbage memory
	int N_GARBAGE = garbage_collector.size();
	for (int i=0;i<N_GARBAGE;i++) delete garbage_collector[i];
}