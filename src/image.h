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

typedef color::rgb<std::uint8_t> rgb_t;
typedef color::hsl<std::uint8_t> hsl_t;

class Image: public RenderObject
{
public:
    Image(std::string imageName);
    ~Image();
	void generateHSL();
	void updateTexture();
	void setH(int dif);
	void setS(int dif);
	void setL(int dif);
    void dumpImage();
    int getWidth();
    int getHeight();
    void getImageWindowSize(int &w, int &h);
    void setInteractable(bool inter);
    void scale(float xs, float ys);
#ifndef NDEBUG
    std::string getName();
#endif
    char button();
    void resetImageData();

    static void SetScreenDetails(int w, int h);
private:
    static int sWidth;
    static int sHeight;
    int win_width;
    int win_height;
    int iwidth;
    int iheight;
    int cmp;
    GLuint cmp_type;
    unsigned char *data;
    std::string iType;
    std::string iName;
	std::vector< hsl_t > HSL_arr;
	std::vector< rgb_t > RGB_arr;
};
