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
#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include "RenderObject.h"
#include "image.h"
using namespace std;

#define WIN_WIDTH 1600
#define WIN_HEIGHT 900

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void)
{
    // Creation
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1); // v-sync
    assert(glGetError() == GL_NO_ERROR);

    vector<RenderObject*> renderTargets;

    Quad q1 = { 0, 200, 0, 200 };
    Image tester1(q1, "wetsuit.jpg");
    Quad q2 = { 0, 400, 0, 400 };
    Image tester2(q2, "wetsuit.jpg");
    Quad q3 = { 0, 600, 0, 600 };
    Image tester3(q3, "wetsuit.jpg");
    assert(glGetError() == GL_NO_ERROR);

    renderTargets.push_back(&tester1);
    renderTargets.push_back(&tester2);
    renderTargets.push_back(&tester3);

    while (!glfwWindowShouldClose(window))
    {
        // General
        float ratio;
        int width, height;
        glViewport(0, 0, WIN_HEIGHT, WIN_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        //      left    right       bottom      top     near    far
        glOrtho(0.0f,   WIN_WIDTH,  WIN_HEIGHT, 0,      -1.0f,   1.0f);
        /*glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;*/
        assert(glGetError() == GL_NO_ERROR);
        glDisable(GL_CULL_FACE);
        // Draw
        for (int i = renderTargets.size() - 1; i >= 0; i--)
            renderTargets[i]->render();

        // Get events and swap
        assert(glGetError() == GL_NO_ERROR);
        glPopMatrix();
        glFlush();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}