#pragma once
#include <string>
#include <iostream>
#include "stb_image.h"
#include "stb_image_write.h"
#include "RenderObject.h"

#ifdef _WIN32
#define APIENTRY __stdcall
#pragma warning(disable: 4514)
#include "glad/glad.h"
extern "C"
{
#include "GLFW/glfw3.h"
}
#else // _WIN32
#include <glad/glad.h>
extern "C"
{
#include <GLFW/glfw3.h>
}
#endif // Unix

class Image : public RenderObject
{
public:
    Image(std::string imageName);
    ~Image();

    void dumpImage();
    int getWidth();
    int getHeight();
    void setInteractable(bool inter);
    void scale(float xs, float ys);
#ifndef NDEBUG
    std::string getName();
#endif
    char button();

    void resetImageData();
private:
    int iwidth;
    int iheight;
    int cmp;
    GLuint cmp_type;
    unsigned char *data;
    std::string iType;
    std::string iName;
};
