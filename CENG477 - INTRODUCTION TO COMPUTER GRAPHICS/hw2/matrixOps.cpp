#include "matrixOps.h"
Matrix4 makeMatrix4FromVectorsOnColumns(const Vec3& u, const Vec3& v, const Vec3& w) {
    double values[4][4] = {
        {u.x, v.x, w.x, 0},
        {u.y, v.y, w.y, 0},
        {u.z, v.z, w.z, 0},
        {0, 0, 0, 1}
    };
    return Matrix4(values);
}
Matrix4 makeMatrix4FromVectorsOnRows(const Vec3& u, const Vec3& v, const Vec3& w) {
    double values[4][4] = {
        {u.x, u.y, u.z, 0},
        {v.x, v.y, v.z, 0},
        {w.x, w.y, w.z, 0},
        {0, 0, 0, 1}
    };
    return Matrix4(values);
}

Matrix4 createScalingMatrix(Scaling *s){
	Matrix4 result = getIdentityMatrix();
    
    result.values[0][0] = s->sx;
	result.values[1][1] = s->sy;
	result.values[2][2] = s->sz;
	//kaldırabilirim belki
	result.values[3][3] = 1;

	return result;
}
Matrix4 createTranslationMatrix(Translation *t){
	double values[4][4] = {
        {1, 0, 0, t->tx},
        {0, 1, 0, t->ty},
        {0, 0, 1, t->tz},
        {0, 0, 0, 1}
    };
    return Matrix4(values);
}

Matrix4 makeRotationMatrix4(Rotation *r){
    //Declare vectors and M matrix of rotation
	
    Vec3 uVector;
    Vec3 vVector;
    Vec3 wVector;
    Matrix4 mMatrix;
    Matrix4 result;
    

    //Get uVector from rotation
	uVector = Vec3(r->ux, r->uy, r->uz);
    //Normalize
	
    uVector = normalizeVec3(uVector);
	
    //Generate vVector perpendicular to uVector by finding the smallest component and make it 0 in v,
    //change sign of others
	if(abs(uVector.x)<=abs(uVector.y)){
		if(abs(uVector.x)<=abs(uVector.z)){
            vVector = Vec3{0, -uVector.z, uVector.y};
		}
		else{
            vVector = Vec3{-uVector.y, 0, uVector.x};
		}
	}
	else{
		if(abs(uVector.y)<=abs(uVector.z)){
            vVector = Vec3{-uVector.z, 0, uVector.x};
		}
		else{
            vVector = Vec3{-uVector.y, uVector.x, 0};
		}
	}

    //Normalize vVector
	vVector = normalizeVec3(vVector);

    //Generate w easily by cross product of u and v

	wVector = crossProductVec3(uVector,vVector);

	//normalize wVector
	//Might be unnecessary
    wVector = normalizeVec3(wVector);

    //Now we have an orthogonal basis for rotation

    //Build Matrices of the pipeline
	Matrix4 M = makeMatrix4FromVectorsOnRows(uVector, vVector, wVector);
	Matrix4 M_T = makeMatrix4FromVectorsOnColumns(uVector,vVector,wVector);
    Matrix4 R = getIdentityMatrix();
    double rad = (r->angle*PI_NUMBER)/180;

	R.values[1][1] = cos(rad);
	R.values[1][2] = -1*sin(rad);
	R.values[2][1] = sin(rad);
	R.values[2][2] = cos(rad);
	result = multiplyMatrixWithMatrix(R,M);
	result = multiplyMatrixWithMatrix(M_T, result);
	return result;
	
}

Matrix4 combineModelingMatrices(Scene* scene, Mesh* mesh) {
    Matrix4 result = getIdentityMatrix();

    for (int i = 0; i < mesh->numberOfTransformations; ++i) {
        const char transformationType = mesh->transformationTypes[i];
        const int transformationId = mesh->transformationIds[i] - 1;

        switch (transformationType) {
            case 's': {
                Matrix4 scalingMatrix = createScalingMatrix(scene->scalings[transformationId]);
                result = multiplyMatrixWithMatrix(scalingMatrix, result);
                break;
            }
            case 'r': {
                Matrix4 rotationMatrix = makeRotationMatrix4(scene->rotations[transformationId]);
                result = multiplyMatrixWithMatrix(rotationMatrix, result);
                break;
            }
            case 't': {
                Matrix4 translationMatrix = createTranslationMatrix(scene->translations[transformationId]);
                result = multiplyMatrixWithMatrix(translationMatrix, result);
                break;
            }
            default:
                break;
        }
    }

    return result;
}

Matrix4 makeCameraTransformationMatrix(Camera* camera){
    Matrix4 matrix = makeMatrix4FromVectorsOnRows(camera->u, camera->v, camera->w);

    // Extract camera axes and position for brevity
    //Vec3& u = camera->u;
    //Vec3& v = camera->v;
    //Vec3& w = camera->w;

    Vec3& position = camera->position;
    //Translation along u
    matrix.values[0][3] = -dotProductVec3(camera->u, position);

    //Translation along v
    matrix.values[1][3] = -dotProductVec3(camera->v, position);

    //Translation along w
    matrix.values[2][3] = -dotProductVec3(camera->w, position);

    //Homogeneous coordinates are left as they are
    matrix.values[3][3] = 1.0;

    return matrix;
}

//Create orthographic projection matrix from camera properties
Matrix4 createOrthographicMatrix(Camera* camera) {
    Matrix4 result;

    // Those scalors scale u,v,z s.t bounding box becomes 1*1*1 cube
    double rlScalor = 1.0 / (camera->right - camera->left);
    double tbScalor = 1.0 / (camera->top - camera->bottom);
    double fnScalor = 1.0 / (camera->far - camera->near);

    //1st line of each mıltiplies scalor with 2 to to make bounding box 2*2*2 cube
    result.values[0][0] = 2.0 * rlScalor;
    //2nd line of each translates bounding box's center tı the camera position
    result.values[0][3] = -(camera->right + camera->left) * rlScalor;

    result.values[1][1] = 2.0 * tbScalor;
    result.values[1][3] = -(camera->top + camera->bottom) * tbScalor;
    
    result.values[2][2] = -2.0 * fnScalor;
    result.values[2][3] = -(camera->far + camera->near) * fnScalor;

    //Homogenous component, no change there
    result.values[3][3] = 1.0;

    return result;
}

Matrix4 createPerspectiveMatrix(Camera *camera){
	Matrix4 resultingMatrix;
	//2*(n/r-l)
	resultingMatrix.values[0][0] = (2.0*camera->near)/(camera->right - camera->left);
	resultingMatrix.values[0][1] = 0;
	//(r+l/r-l)
	resultingMatrix.values[0][2] = (camera->right + camera->left)/(camera->right - camera->left);
	resultingMatrix.values[0][3] = 0;
	resultingMatrix.values[1][0] = 0;
	//2*(n/r-l)
	resultingMatrix.values[1][1] =  (2.0*camera->near)/(camera->top - camera->bottom);
	//(t+b)/(t-b)
	resultingMatrix.values[1][2] = (camera->top + camera->bottom)/(camera->top - camera->bottom);
	resultingMatrix.values[1][3] = 0;

	resultingMatrix.values[2][0] = 0;
	resultingMatrix.values[2][1] = 0;
	//-(f+n/f-n)
	resultingMatrix.values[2][2] =  (-1.0)*(camera->far + camera->near)/(camera->far - camera->near);
	//-2(f*n/f-n)
	resultingMatrix.values[2][3] = (-2.0)*(camera->far * camera->near)/(camera->far - camera->near);

	resultingMatrix.values[3][0] = 0;
	resultingMatrix.values[3][1] = 0;
	//Negate w
	resultingMatrix.values[3][2] = -1.0;
	resultingMatrix.values[3][3] = 0;

	return resultingMatrix;
}

Matrix4 createViewportMatrix(Camera *camera){
	Matrix4 resultingMatrix;

    //Scale x by Resolution and divide by 2 to map [-1,1] to [-0.5,nx-0.5]
	resultingMatrix.values[0][0] = camera->horRes/2.0;
	resultingMatrix.values[0][1] = 0;
	resultingMatrix.values[0][2] = 0;
	resultingMatrix.values[0][3] = (camera->horRes-1)/2.0;

    //Scale y by Resolution and divide by 2 to map [-1,1] to [-0.5,ny-0.5]
	resultingMatrix.values[1][0] = 0;
	resultingMatrix.values[1][1] = camera->verRes/2.0;
	resultingMatrix.values[1][2] = 0;
	resultingMatrix.values[1][3] = (camera->verRes-1)/2.0;

    //z value is mapped to [0,1] for depth testing purposes
	resultingMatrix.values[2][0] = 0;
	resultingMatrix.values[2][1] = 0;
	resultingMatrix.values[2][2] = 0.5;
	resultingMatrix.values[2][3] = 0.5;
    //No change in homogenous coords

	return resultingMatrix;
}