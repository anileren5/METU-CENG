#include "Matrix4.h"
#include "Vec3.h"
#include "Camera.h"
#include "Scene.h"
#include "Helpers.h"
#include <cmath>
#define PI_NUMBER 3.141592653589793238462643383279502884197
Matrix4 makeCameraTransformationMatrix(Camera *);
Matrix4 createOrthographicMatrix(Camera *);
Matrix4 createPerspectiveMatrix(Camera *);
Matrix4 createViewportMatrix(Camera *);
Matrix4 combineModelingMatrices(Scene *, Mesh *);
Matrix4 makeRotationMatrix4(Rotation *r);