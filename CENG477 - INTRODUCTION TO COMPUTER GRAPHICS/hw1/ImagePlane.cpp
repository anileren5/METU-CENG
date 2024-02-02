#include "ImagePlane.h"

void ImagePlane::init(int width, int height, Vec3i background_color){
    image = new unsigned char[width * height * 3];
    this->width = width;
    this->height = height;
    this->background_color = background_color;

    // Set all pixels to background_color
    for (int i=0; i<width; i++) for (int j=0; j<height/3; j++) this->setPixelColor(i,j,this->background_color);
}

void ImagePlane::destroy() {delete[] this->image;}

Vec3i ImagePlane::getPixelColor(int i, int j){ // pixel on the upper left corner has indices i=0, j=0 , i: increases towards right, j: increases towards bottom
    // Find index of pixel corresponding to r
    int r_index = 3 * j * width  + 3 * i;
            
    Vec3i pixel;
    pixel.x = this->image[r_index];
    pixel.y = this->image[r_index + 1];
    pixel.z = this->image[r_index + 2];

    return pixel;
}

void ImagePlane::setPixelColor(int i, int j, Vec3i pixel_color){
    int r_index = 3 * j * width + 3 * i;

    this->image[r_index] = pixel_color.x;
    this->image[r_index + 1] = pixel_color.y;
    this->image[r_index + 2] = pixel_color.z;
}  

unsigned char* ImagePlane::getImage(){return image;}