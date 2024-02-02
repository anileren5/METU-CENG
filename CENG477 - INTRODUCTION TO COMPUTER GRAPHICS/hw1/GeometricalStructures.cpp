// Custom libraries
#include "Utils.h"
#include "GeometricalStructures.h"

// Namespace resolutions
using namespace Utils;

HitRecord::HitRecord() {
    this->isHit = false;
}

HitRecord::HitRecord(int indexInCorrespondingArray, double t, Ray ray, Sphere sphere, Scene& SCENE) {
    this->isHit = true;
    this->hitObject = 0;
    this->indexInCorrespondingArray = indexInCorrespondingArray;
    this->t = t;
    this->ray = ray;
    this->sphere = sphere;
    this->pointOnSurface = getSpecificPointOnRay(ray, t);
    this->normal = getNormalizedVector(getVectorSubtraction(this->pointOnSurface, SCENE.vertex_data[sphere.center_vertex_id - 1]));
}

HitRecord::HitRecord(int indexInCorrespondingArray, double t, Ray ray, Triangle triangle, Scene& SCENE) {
    this->isHit = true;
    this->hitObject = 1;
    this->indexInCorrespondingArray = indexInCorrespondingArray;
    this->t = t;
    this->ray = ray;
    this->triangle = triangle;
    this->normal = getNormalizedVector(getCrossProduct(getVectorSubtraction(SCENE.vertex_data[triangle.indices.v1_id-1], SCENE.vertex_data[triangle.indices.v0_id-1]), getVectorSubtraction(SCENE.vertex_data[triangle.indices.v2_id-1], SCENE.vertex_data[triangle.indices.v0_id-1])));
    this->pointOnSurface = getSpecificPointOnRay(this->ray, this->t);
}

HitRecord::HitRecord(int indexInCorrespondingArray, double t, Ray ray, Mesh mesh, Vec3f pointOnSurface, Vec3f normal) {
    this->mesh = mesh;
    this->ray = ray;
    this->hitObject = 2;
    this->indexInCorrespondingArray = indexInCorrespondingArray;
    this->isHit = true;
    this->t = t;
    this->pointOnSurface = pointOnSurface;
    this->normal = normal;
}