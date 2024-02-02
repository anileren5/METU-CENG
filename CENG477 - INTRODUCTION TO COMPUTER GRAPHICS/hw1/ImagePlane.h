#ifndef __HW1__IMAGE_PLANE__
#define __HW1__IMAGE_PLANE__

#include "parser.h"
#include "Utils.h"

using namespace parser;

class ImagePlane {
    private:
        unsigned char* image;
        int width, height;
        Vec3i background_color;

    public:
        void init(int width, int height, Vec3i background_color);
        void destroy();
        Vec3i getPixelColor(int i, int j);
        void setPixelColor(int i, int j, Vec3i pixel_color);
        unsigned char* getImage(); 
};

#endif