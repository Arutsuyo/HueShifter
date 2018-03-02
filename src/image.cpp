#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#include <string>
#include <iostream>
#include "image.h"
#include "stb_image.h"
#include "stb_image_write.h"
using namespace std;

Image::Image(std::string imageName)
{
    data = stbi_load(imageName.c_str(), &width, &height, &comp, 0);
    if (data == NULL)
        cerr << "Error: Image failed to load: " << imageName << endl;
    
}

void Image::dumpImage()
{
    stbi_write_png("boop.png", width, height, comp, data, width);
}

Image::~Image()
{
    stbi_image_free(data);
}
