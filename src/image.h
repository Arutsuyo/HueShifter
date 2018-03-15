#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "stb_image.h"
#include "stb_image_write.h"
#include "RenderObject.h"
#include "color/color.hpp"

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

typedef color::rgb<uint8_t> rgb_t;
typedef color::hsl<uint8_t> hsl_t;

class Image : public RenderObject
{
public:
    Image(std::string imageName);
    ~Image();

    // Static call to make sure we don't have a large window
    static void SetScreenDetails(int w, int h);

    // Image manipulation functions
    void generateHSL();
    void updateTexture();
    void setH(float h);
    void setS(float s);
    void setL(float l);

    // Create final image
    void dumpImage();

    // Standard image calls
    int getWidth();
    int getHeight();
    void scale(float xs, float ys);

    // Tools calls
    void getMaxImageWindowSize(int &w, int &h);
    void setInteractable(bool inter);
    char button();
    void resetImageData();

#ifndef NDEBUG
    // Only used in debug output
    std::string getName();
#endif

private:
    // Static variables to track max display size
    static int sWidth;
    static int sHeight;

    // Image info
    int iwidth;
    int iheight;
    int cmp;
    GLuint cmp_type;
    std::string iType;
    std::string iName;
    
    // main data and original data
    unsigned char *data;
    unsigned char *o_data;

    // Conversion vector
    std::vector< hsl_t > HSL_arr;
};
