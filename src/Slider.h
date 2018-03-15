#pragma once
#include <string>
#include <iostream>
#include "stb_image.h"
#include "stb_image_write.h"
#include "Image.h"
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

class Slider
{
public:
    Slider(Quad place, std::string barName, std::string pointerName);
    ~Slider();

    void render();
    bool hovering(double x, double y);
    void slide(int x, int y);
    void setx(int x);
    void reset();
    float getSliderValue();

private:
    Image bar;
    Image point;
};
