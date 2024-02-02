#ifndef __HW1__GEOMETRICAL_STRUCTURES__
#define __HW1__GEOMETRICAL_STRUCTURES__

// Standard libraries
#include <iostream>

// Given libraries
#include "parser.h"

// Custom libraries
#include "Utils.h"

// Namespace resolutions
using namespace parser;
using namespace std;

// Data structures
typedef struct Ray {
    Vec3f starting_point;
    Vec3f direction_vector;
} Ray;

class HitRecord {
    public:
        bool isHit;
        int hitObject, indexInCorrespondingArray;
        double t;
        Ray ray;
        Triangle triangle;
        Sphere sphere;
        Mesh mesh;
        Vec3f normal, pointOnSurface;

        HitRecord();
        HitRecord(int indexInCorrespondingArray, double t, Ray ray, Sphere sphere, Scene& SCENE);
        HitRecord(int indexInCorrespondingArray, double t, Ray ray, Triangle triangle, Scene& SCENE);
        HitRecord(int indexInCorrespondingArray, double t, Ray ray, Mesh mesh, Vec3f pointOnSurface, Vec3f normal);
};


#endif