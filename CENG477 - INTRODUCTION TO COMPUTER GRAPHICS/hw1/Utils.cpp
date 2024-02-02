// Custom librariries
#include "Utils.h"

// Standard libraries
#include <cmath>

Vec3f Utils::getCrossProduct(Vec3f  v1, Vec3f  v2) {
    Vec3f result;
    result.x = v1.y*v2.z - v1.z*v2.y;
    result.y = v1.z*v2.x - v1.x*v2.z;
    result.z = v1.x*v2.y - v1.y*v2.x;
    return result;
}

Vec3f Utils::getNormalizedVector(Vec3f v) {
    Vec3f normalizedVector;
    double vectorNorm = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
    normalizedVector.x = v.x/vectorNorm;
    normalizedVector.y = v.y/vectorNorm;
    normalizedVector.z = v.z/vectorNorm;
    return normalizedVector;
}

Vec3f Utils::getScaledVectorByScalar(Vec3f v, double scalar) {
    Vec3f result;
    result.x = v.x * scalar;
    result.y = v.y * scalar;
    result.z = v.z * scalar;
    return result;
}

Vec3f Utils::getVectorAddition(Vec3f v1, Vec3f v2) { 
    Vec3f result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    return result;
}

Vec3f Utils::getVectorSubtraction(Vec3f v1, Vec3f v2) {
    return getVectorAddition(v1, getScaledVectorByScalar(v2, -1));
}

Vec3i Utils::getPixelColorClapped(Vec3f unclappedColor) {
    unclappedColor.x = unclappedColor.x > 255 ? 255 : unclappedColor.x;
    unclappedColor.y = unclappedColor.y > 255 ? 255 : unclappedColor.y;
    unclappedColor.z = unclappedColor.z > 255 ? 255 : unclappedColor.z;
    unclappedColor.x = unclappedColor.x < 0 ? 0 : unclappedColor.x;
    unclappedColor.y = unclappedColor.y < 0 ? 0 : unclappedColor.y;
    unclappedColor.z = unclappedColor.z < 0 ? 0 : unclappedColor.z;
    Vec3i clappedColor;
    clappedColor.x = round(unclappedColor.x);
    clappedColor.y = round(unclappedColor.y);
    clappedColor.z = round(unclappedColor.z);
    return clappedColor;
}

Vec3f Utils::getSpecificPointOnRay(Ray ray, double t) {
    return getVectorAddition(ray.starting_point, getScaledVectorByScalar(ray.direction_vector, t));
}

Vec3f Utils::getDimensionalMultiplication(Vec3f v1, Vec3f v2) {
    Vec3f dimensionalMultiplication;
    dimensionalMultiplication.x = v1.x * v2.x;
    dimensionalMultiplication.y = v1.y * v2.y;
    dimensionalMultiplication.z = v1.z * v2.z;
    return dimensionalMultiplication;
}

double Utils::getDistanceBetweenPoints(Vec3f p1, Vec3f p2) {
    return sqrtf((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y) + (p1.z-p2.z)*(p1.z-p2.z));
}

double Utils::getDotProduct(Vec3f v1, Vec3f v2) {
    double dotProduct = 0;
    dotProduct += v1.x * v2.x;
    dotProduct += v1.y * v2.y;
    dotProduct += v1.z * v2.z;
    return dotProduct;
}

double Utils::getDeterminant(Vec3f v1, Vec3f v2, Vec3f v3) {
    // Please refer to "Ray tracing geometry" slide page 43 for calculations
    double a = v1.x, b = v1.y, c = v1.z;
    double d = v2.x, e = v2.y, f = v2.z;
    double g = v3.x, h = v3.y, i = v3.z;
    return a*(e*i-h*f)+b*(g*f-d*i)+c*(d*h-e*g);
}
