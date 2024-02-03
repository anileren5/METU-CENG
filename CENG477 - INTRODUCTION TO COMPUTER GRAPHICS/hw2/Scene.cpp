/**#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <cmath>

#include "Scene.h"
#include "Camera.h"
#include "Color.h"
#include "Mesh.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "tinyxml2.h"
#include "Helpers.h"

#include "Methods.h"**/
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>

#include "tinyxml2.h"
#include "Triangle.h"
#include "Helpers.h"
#include "matrixOps.h"
#include "vectorOps.h"
#include "DrawRelated.h"

//I deleted scene.h here

using namespace tinyxml2;
using namespace std;

/*
	Transformations, clipping, culling, rasterization are done here.
	You may define helper functions.
*/


void Scene::forwardRenderingPipeline(Camera *camera)
{
	// TODO: Implement this function.
	int meshAmount = this->meshes.size();
	int currentMeshIndex,triangleIndex;
	Matrix4 camTr = makeCameraTransformationMatrix(camera);
	Matrix4 projTr;
	if(camera->projectionType == 1){
		projTr = createPerspectiveMatrix(camera);
	}
	else projTr = createOrthographicMatrix(camera);
	Matrix4 viewTr = createViewportMatrix(camera);

	for(currentMeshIndex = 0; currentMeshIndex < meshAmount; currentMeshIndex++){
		//model transform
		Matrix4 modelTr = combineModelingMatrices(this, this->meshes[currentMeshIndex]);
		//viewtr * (divide) perxcam * model * vertices
		for(triangleIndex=0; triangleIndex< meshes[currentMeshIndex]->numberOfTriangles ; triangleIndex++){
			Vec3 *v1 =  this->vertices[meshes[currentMeshIndex]->triangles[triangleIndex].vertexIds[0]-1];
			Vec3 *v2 =  this->vertices[meshes[currentMeshIndex]->triangles[triangleIndex].vertexIds[1]-1];
			Vec3 *v3 =  this->vertices[meshes[currentMeshIndex]->triangles[triangleIndex].vertexIds[2]-1];
			
			Vec4 v1_t;
			Vec4 v2_t;
			Vec4 v3_t;
			v1_t.x = v1->x; v1_t.y=v1->y; v1_t.z= v1->z; v1_t.t = 1; v1_t.colorId = v1->colorId;
			v2_t.x = v2->x; v2_t.y=v2->y; v2_t.z= v2->z; v2_t.t = 1; v2_t.colorId = v2->colorId;
			v3_t.x = v3->x; v3_t.y=v3->y; v3_t.z= v3->z; v3_t.t = 1; v3_t.colorId = v3->colorId;
			if (camera->projectionType == 1){
				// Perspective projection
				// Perspective*Camera*Modeling*Point
				v1_t = multiplyMatrixWithVec4(createPerspectiveMatrix(camera),multiplyMatrixWithVec4(makeCameraTransformationMatrix(camera), multiplyMatrixWithVec4(combineModelingMatrices(this, this->meshes[currentMeshIndex]), v1_t)));
				v2_t = multiplyMatrixWithVec4(createPerspectiveMatrix(camera),multiplyMatrixWithVec4(makeCameraTransformationMatrix(camera), multiplyMatrixWithVec4(combineModelingMatrices(this, this->meshes[currentMeshIndex]), v2_t)));
				v3_t = multiplyMatrixWithVec4(createPerspectiveMatrix(camera),multiplyMatrixWithVec4(makeCameraTransformationMatrix(camera), multiplyMatrixWithVec4(combineModelingMatrices(this, this->meshes[currentMeshIndex]), v3_t)));
			}
			else{
				// Orthographic projection
				// Ortographic*Camera*Modeling*Point
				v1_t = multiplyMatrixWithVec4(createOrthographicMatrix(camera),multiplyMatrixWithVec4(makeCameraTransformationMatrix(camera), multiplyMatrixWithVec4(combineModelingMatrices(this, this->meshes[currentMeshIndex]), v1_t)));
				v2_t = multiplyMatrixWithVec4(createOrthographicMatrix(camera),multiplyMatrixWithVec4(makeCameraTransformationMatrix(camera), multiplyMatrixWithVec4(combineModelingMatrices(this, this->meshes[currentMeshIndex]), v2_t)));
				v3_t = multiplyMatrixWithVec4(createOrthographicMatrix(camera),multiplyMatrixWithVec4(makeCameraTransformationMatrix(camera), multiplyMatrixWithVec4(combineModelingMatrices(this, this->meshes[currentMeshIndex]), v3_t)));
			}
			

			//Check culling, continue on the loop if enabled&&facingBack
			if(this-> cullingEnabled) {
				if (facingBack(v1_t, v2_t, v3_t))
				{
					continue;
				} 	
			}
			//Common Proccess
			perspectiveDivideVector4(v1_t);
			perspectiveDivideVector4(v2_t);
			perspectiveDivideVector4(v3_t);

			if(meshes[currentMeshIndex]->type!=1){
				
				//perspective divide
				
				//if(v1_t.t != 1 && v1_t.t != 0) {v1_t.x = v1_t.x/v1_t.t; v1_t.y = v1_t.y/v1_t.t; v1_t.z = v1_t.z/v1_t.t; v1_t.t = 1;}
				//if(v2_t.t != 1 && v2_t.t != 0) {v2_t.x = v2_t.x/v2_t.t; v2_t.y = v2_t.y/v2_t.t; v2_t.z = v2_t.z/v2_t.t; v2_t.t = 1;}
				//if(v3_t.t != 1 && v3_t.t != 0) {v3_t.x = v3_t.x/v3_t.t; v3_t.y = v3_t.y/v3_t.t; v3_t.z = v3_t.z/v3_t.t; v3_t.t = 1;}
				
				Vec4 v01_1= v1_t;
                Vec4 v01_2= v2_t;
				Vec4 v20_1= v3_t;
				Vec4 v20_2= v1_t;
				Vec4 v12_1= v2_t;
				Vec4 v12_2= v3_t;
				Color * c1 = this->colorsOfVertices[v1_t.colorId-1];
				Color * c2 = this->colorsOfVertices[v2_t.colorId-1];
				Color * c3 = this->colorsOfVertices[v3_t.colorId-1];
				//clipping
				bool v1v2Clipped  = clippedLine(this,v01_1, v01_2,*c1,*c2);
				bool v2v3Clipped = clippedLine(this,v12_1, v12_2,*c2,*c3);
				bool v3v1Clipped = clippedLine(this,v20_1, v20_2,*c3,*c1);
				//apply viewport transformation
				v01_1 = multiplyMatrixWithVec4(viewTr, v01_1);
				v01_2 = multiplyMatrixWithVec4(viewTr,v01_2);
				v12_1 = multiplyMatrixWithVec4(viewTr, v12_1);
				v12_2 = multiplyMatrixWithVec4(viewTr, v12_2);
				v20_1 = multiplyMatrixWithVec4(viewTr,v20_1);
				v20_2 = multiplyMatrixWithVec4(viewTr, v20_2);
				//draw
				if( v1v2Clipped == true) {
			
					drawLine(this,v01_1,v01_2, camera);
				}
				if(v2v3Clipped== true) {

					drawLine(this,v12_1,v12_2, camera);
				}
				if(v3v1Clipped== true) {
		
					drawLine(this,v20_1,v20_2, camera);
				}

			}   
			else{
				//perspective divide
				//if(v1_t.t != 1 && v1_t.t != 0) {v1_t.x = v1_t.x/v1_t.t; v1_t.y = v1_t.y/v1_t.t; v1_t.z = v1_t.z/v1_t.t; v1_t.t = 1;}
				//if(v2_t.t != 1 && v2_t.t != 0) {v2_t.x = v2_t.x/v2_t.t; v2_t.y = v2_t.y/v2_t.t; v2_t.z = v2_t.z/v2_t.t; v2_t.t = 1;}
				//if(v3_t.t != 1 && v3_t.t != 0) {v3_t.x = v3_t.x/v3_t.t; v3_t.y = v3_t.y/v3_t.t; v3_t.z = v3_t.z/v3_t.t; v3_t.t = 1;}

				//viewport transformation
				v1_t = multiplyMatrixWithVec4(viewTr, v1_t);
				v2_t = multiplyMatrixWithVec4(viewTr, v2_t);
				v3_t = multiplyMatrixWithVec4(viewTr, v3_t);
				triangleRaster(this, v1_t, v2_t, v3_t, camera);
			}
			
		}
		
	}
}

/*
	Parses XML file
*/
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLElement *xmlElement;

	xmlDoc.LoadFile(xmlPath);

	XMLNode *rootNode = xmlDoc.FirstChild();

	// read background color
	xmlElement = rootNode->FirstChildElement("BackgroundColor");
	str = xmlElement->GetText();
	sscanf(str, "%lf %lf %lf", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	// read culling
	xmlElement = rootNode->FirstChildElement("Culling");
	if (xmlElement != NULL)
	{
		str = xmlElement->GetText();

		if (strcmp(str, "enabled") == 0)
		{
			this->cullingEnabled = true;
		}
		else
		{
			this->cullingEnabled = false;
		}
	}

	// read cameras
	xmlElement = rootNode->FirstChildElement("Cameras");
	XMLElement *camElement = xmlElement->FirstChildElement("Camera");
	XMLElement *camFieldElement;
	while (camElement != NULL)
	{
		Camera *camera = new Camera();

		camElement->QueryIntAttribute("id", &camera->cameraId);

		// read projection type
		str = camElement->Attribute("type");

		if (strcmp(str, "orthographic") == 0)
		{
			camera->projectionType = ORTOGRAPHIC_PROJECTION;
		}
		else
		{
			camera->projectionType = PERSPECTIVE_PROJECTION;
		}

		camFieldElement = camElement->FirstChildElement("Position");
		str = camFieldElement->GetText();
		sscanf(str, "%lf %lf %lf", &camera->position.x, &camera->position.y, &camera->position.z);

		camFieldElement = camElement->FirstChildElement("Gaze");
		str = camFieldElement->GetText();
		sscanf(str, "%lf %lf %lf", &camera->gaze.x, &camera->gaze.y, &camera->gaze.z);

		camFieldElement = camElement->FirstChildElement("Up");
		str = camFieldElement->GetText();
		sscanf(str, "%lf %lf %lf", &camera->v.x, &camera->v.y, &camera->v.z);

		camera->gaze = normalizeVec3(camera->gaze);
		camera->u = crossProductVec3(camera->gaze, camera->v);
		camera->u = normalizeVec3(camera->u);

		camera->w = inverseVec3(camera->gaze);
		camera->v = crossProductVec3(camera->u, camera->gaze);
		camera->v = normalizeVec3(camera->v);

		camFieldElement = camElement->FirstChildElement("ImagePlane");
		str = camFieldElement->GetText();
		sscanf(str, "%lf %lf %lf %lf %lf %lf %d %d",
			   &camera->left, &camera->right, &camera->bottom, &camera->top,
			   &camera->near, &camera->far, &camera->horRes, &camera->verRes);

		camFieldElement = camElement->FirstChildElement("OutputName");
		str = camFieldElement->GetText();
		camera->outputFilename = string(str);

		this->cameras.push_back(camera);

		camElement = camElement->NextSiblingElement("Camera");
	}

	// read vertices
	xmlElement = rootNode->FirstChildElement("Vertices");
	XMLElement *vertexElement = xmlElement->FirstChildElement("Vertex");
	int vertexId = 1;

	while (vertexElement != NULL)
	{
		Vec3 *vertex = new Vec3();
		Color *color = new Color();

		vertex->colorId = vertexId;

		str = vertexElement->Attribute("position");
		sscanf(str, "%lf %lf %lf", &vertex->x, &vertex->y, &vertex->z);

		str = vertexElement->Attribute("color");
		sscanf(str, "%lf %lf %lf", &color->r, &color->g, &color->b);

		this->vertices.push_back(vertex);
		this->colorsOfVertices.push_back(color);

		vertexElement = vertexElement->NextSiblingElement("Vertex");

		vertexId++;
	}

	// read translations
	xmlElement = rootNode->FirstChildElement("Translations");
	XMLElement *translationElement = xmlElement->FirstChildElement("Translation");
	while (translationElement != NULL)
	{
		Translation *translation = new Translation();

		translationElement->QueryIntAttribute("id", &translation->translationId);

		str = translationElement->Attribute("value");
		sscanf(str, "%lf %lf %lf", &translation->tx, &translation->ty, &translation->tz);

		this->translations.push_back(translation);

		translationElement = translationElement->NextSiblingElement("Translation");
	}

	// read scalings
	xmlElement = rootNode->FirstChildElement("Scalings");
	XMLElement *scalingElement = xmlElement->FirstChildElement("Scaling");
	while (scalingElement != NULL)
	{
		Scaling *scaling = new Scaling();

		scalingElement->QueryIntAttribute("id", &scaling->scalingId);
		str = scalingElement->Attribute("value");
		sscanf(str, "%lf %lf %lf", &scaling->sx, &scaling->sy, &scaling->sz);

		this->scalings.push_back(scaling);

		scalingElement = scalingElement->NextSiblingElement("Scaling");
	}

	// read rotations
	xmlElement = rootNode->FirstChildElement("Rotations");
	XMLElement *rotationElement = xmlElement->FirstChildElement("Rotation");
	while (rotationElement != NULL)
	{
		Rotation *rotation = new Rotation();

		rotationElement->QueryIntAttribute("id", &rotation->rotationId);
		str = rotationElement->Attribute("value");
		sscanf(str, "%lf %lf %lf %lf", &rotation->angle, &rotation->ux, &rotation->uy, &rotation->uz);

		this->rotations.push_back(rotation);

		rotationElement = rotationElement->NextSiblingElement("Rotation");
	}

	// read meshes
	xmlElement = rootNode->FirstChildElement("Meshes");

	XMLElement *meshElement = xmlElement->FirstChildElement("Mesh");
	while (meshElement != NULL)
	{
		Mesh *mesh = new Mesh();

		meshElement->QueryIntAttribute("id", &mesh->meshId);

		// read projection type
		str = meshElement->Attribute("type");

		if (strcmp(str, "wireframe") == 0)
		{
			mesh->type = WIREFRAME_MESH;
		}
		else
		{
			mesh->type = SOLID_MESH;
		}

		// read mesh transformations
		XMLElement *meshTransformationsElement = meshElement->FirstChildElement("Transformations");
		XMLElement *meshTransformationElement = meshTransformationsElement->FirstChildElement("Transformation");

		while (meshTransformationElement != NULL)
		{
			char transformationType;
			int transformationId;

			str = meshTransformationElement->GetText();
			sscanf(str, "%c %d", &transformationType, &transformationId);

			mesh->transformationTypes.push_back(transformationType);
			mesh->transformationIds.push_back(transformationId);

			meshTransformationElement = meshTransformationElement->NextSiblingElement("Transformation");
		}

		mesh->numberOfTransformations = mesh->transformationIds.size();

		// read mesh faces
		char *row;
		char *cloneStr;
		int v1, v2, v3;
		XMLElement *meshFacesElement = meshElement->FirstChildElement("Faces");
		str = meshFacesElement->GetText();
		cloneStr = strdup(str);

		row = strtok(cloneStr, "\n");
		while (row != NULL)
		{
			int result = sscanf(row, "%d %d %d", &v1, &v2, &v3);

			if (result != EOF)
			{
				mesh->triangles.push_back(Triangle(v1, v2, v3));
			}
			row = strtok(NULL, "\n");
		}
		mesh->numberOfTriangles = mesh->triangles.size();
		this->meshes.push_back(mesh);

		meshElement = meshElement->NextSiblingElement("Mesh");
	}
}

void Scene::assignColorToPixel(int i, int j, Color c)
{
	this->image[i][j].r = c.r;
	this->image[i][j].g = c.g;
	this->image[i][j].b = c.b;
}

/*
	Initializes image with background color
*/
void Scene::initializeImage(Camera *camera)
{
	if (this->image.empty())
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			vector<Color> rowOfColors;
			vector<double> rowOfDepths;

			for (int j = 0; j < camera->verRes; j++)
			{
				rowOfColors.push_back(this->backgroundColor);
				rowOfDepths.push_back(1.01);
			}

			this->image.push_back(rowOfColors);
			this->depth.push_back(rowOfDepths);
		}
	}
	else
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			for (int j = 0; j < camera->verRes; j++)
			{
				assignColorToPixel(i, j, this->backgroundColor);
				this->depth[i][j] = 1.01;
				this->depth[i][j] = 1.01;
				this->depth[i][j] = 1.01;
			}
		}
	}
}

/*
	If given value is less than 0, converts value to 0.
	If given value is more than 255, converts value to 255.
	Otherwise returns value itself.
*/
int Scene::makeBetweenZeroAnd255(double value)
{
	if (value >= 255.0)
		return 255;
	if (value <= 0.0)
		return 0;
	return (int)(value);
}

/*
	Writes contents of image (Color**) into a PPM file.
*/
void Scene::writeImageToPPMFile(Camera *camera)
{
	ofstream fout;

	fout.open(camera->outputFilename.c_str());

	fout << "P3" << endl;
	fout << "# " << camera->outputFilename << endl;
	fout << camera->horRes << " " << camera->verRes << endl;
	fout << "255" << endl;

	for (int j = camera->verRes - 1; j >= 0; j--)
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			fout << makeBetweenZeroAnd255(this->image[i][j].r) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].g) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
		}
		fout << endl;
	}
	fout.close();
}

/*
	Converts PPM image in given path to PNG file, by calling ImageMagick's 'convert' command.
	os_type == 1 		-> Ubuntu
	os_type == 2 		-> Windows
	os_type == other	-> No conversion
*/
void Scene::convertPPMToPNG(string ppmFileName)
{
	string command;

	// TODO: Change implementation if necessary.
	//command = "./magick convert " + ppmFileName + " " + ppmFileName + ".png";
	//system(command.c_str());
	command = "convert " + ppmFileName + " " + ppmFileName + ".png";
	system(command.c_str());
	return;
}