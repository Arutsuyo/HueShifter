#pragma once
#include <string>
#include <iostream>
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

typedef struct 
{
    //  Left    Right   Bottom  Top
    int x1,     x2,     y1,     y2;
} Quad;

class RenderObject
{
public:
    RenderObject(Quad renderLocation);
    ~RenderObject();

    virtual void render();

protected:
    Quad loc;
    GLuint tex;
};