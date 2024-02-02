#ifndef __HW1__UTILS__
#define __HW1__UTILS__

#include "GeometricalStructures.h"
#include "parser.h"
using namespace parser;

namespace Utils {
    Vec3f getCrossProduct(Vec3f  v1, Vec3f  v2);
    Vec3f getNormalizedVector(Vec3f v);
    Vec3f getScaledVectorByScalar(Vec3f v, double scalar);
    Vec3f getVectorAddition(Vec3f v1, Vec3f v2);
    Vec3f getVectorSubtraction(Vec3f v1, Vec3f v2);
    Vec3i getPixelColorClapped(Vec3f unclappedColor);
    Vec3f getSpecificPointOnRay(Ray ray, double t);
    Vec3f getDimensionalMultiplication(Vec3f v1, Vec3f v2);
    double getDistanceBetweenPoints(Vec3f p1, Vec3f p2);
    double getDeterminant(Vec3f v1, Vec3f v2, Vec3f v3);
    double getDotProduct(Vec3f v1, Vec3f v2);
}

#endif