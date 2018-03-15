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
#include "Image.h"
#include "Slider.h"
#include "color/color.hpp"
#define CUR_SIZE 24

using namespace std;

int IMAGE_WIN_WIDTH = 1;
int IMAGE_WIN_HEIGHT = 1;
int TOOL_WIN_WIDTH = 400;
int TOOL_WIN_HEIGHT = 200;

// Master Objects
vector<Slider*> toolRenders;
GLFWcursor* g_cursor = nullptr;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
static void esc_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void assertGLError()
{
    if (glGetError() == GL_NO_ERROR)
    {
        assert(true);
    }
    
}

void CreateWindows(GLFWwindow* &image_window, GLFWwindow* &tool_window)
{
	if (!glfwInit()) 
	{
		exit(EXIT_FAILURE);
	}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    image_window = glfwCreateWindow(IMAGE_WIN_HEIGHT, IMAGE_WIN_WIDTH, "HueShifter", NULL, NULL);
	tool_window = glfwCreateWindow(TOOL_WIN_WIDTH, TOOL_WIN_HEIGHT, "Tools", NULL, NULL);
    if (!image_window || !tool_window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(tool_window);
    glfwSetErrorCallback(error_callback);
    // FIX THIS
	glfwSetKeyCallback(tool_window, esc_key_callback);

    glfwMakeContextCurrent(image_window);
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(image_window, esc_key_callback);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1); // v-sync

    assertGLError();
}

void PreDraw(int w, int h)
{
    glViewport(0, 0, w, h);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //      left    right   bottom  top     near    far
    glOrtho(0.0f,   w,      0,      h,      -1.0f,  1.0f);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);

    assertGLError();
}

void newCursorColor(double x, double y, GLFWwindow* window)
{
	unsigned int color;
	glReadPixels(x, IMAGE_WIN_HEIGHT-y-1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &color);
	cout << "COLOR: " << hex << color << endl;
	unsigned int pixels[CUR_SIZE * CUR_SIZE];
    for (int i = 0; i < CUR_SIZE * CUR_SIZE; i += 4)
    {
        pixels[i] = pixels[i+1] = pixels[i+2] = pixels[i+3] = color;
    }
	GLFWimage image;
	image.width = CUR_SIZE;
	image.height = CUR_SIZE;
	image.pixels = (unsigned char*)pixels;
	g_cursor = glfwCreateCursor(&image, 0, 0);
	glfwSetCursor(window, g_cursor);
}

void deleteCursorColor()
{
	glfwDestroyCursor(g_cursor);
	g_cursor = nullptr;
}

void handleCursorColor(GLFWwindow* window)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_RELEASE)
	{
		if (g_cursor != nullptr)
			cout << "DEL CURSOR" << endl;
			deleteCursorColor();
		return;
	}
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
    cout << "DB:: Window: " << window << " | Cursor Pos: " << xpos << " " << ypos << endl;
    newCursorColor(xpos, ypos, window);
}

void handleSliders(GLFWwindow* window)
{
    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_RELEASE)
        return;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    ypos = TOOL_WIN_HEIGHT - ypos;
    for (int i = toolRenders.size() - 1; i >= 0; i--)
    {
        if (toolRenders[i]->hovering(xpos, ypos))
        {
            toolRenders[i]->setx(xpos);
            return;
        }
    }
}

int main(void)
{
    // Prompt user to choose a file

    // Standard Windows
	GLFWwindow* image_window;
	GLFWwindow* tool_window;
	CreateWindows(image_window, tool_window);

    // Make objects to render
	Image image("image.jpg");
	IMAGE_WIN_WIDTH = image.getWidth();
	IMAGE_WIN_HEIGHT = image.getHeight();

    // Slider loading
    glfwMakeContextCurrent(tool_window);
    Slider hSlide({ 50, 150, 5, 15 }, "img/H.png", "img/point.png");
    toolRenders.push_back(&hSlide);
    Slider lSlide({ 50, 150, 75, 85 }, "img/L.png", "img/point.png");
    toolRenders.push_back(&lSlide);
    Slider sSlide({ 50, 150, 145, 155 }, "img/S.png", "img/point.png");
    toolRenders.push_back(&sSlide);
    Image saveIcon("img/save.png");
    saveIcon.setInteractable(true);
    saveIcon.setQuad({ 175, 250, 50, 125 });
    Image refreshIcon("img/refresh.png");
    refreshIcon.setInteractable(true);
    refreshIcon.setQuad({ 275, 350, 50, 125 });

	glfwSetWindowSize(image_window, IMAGE_WIN_WIDTH, IMAGE_WIN_HEIGHT);
    glfwSetWindowPos(tool_window, 50, 100);
    glfwSetWindowPos(image_window, 500, 100);

    
    // Main processing loop
    while (!glfwWindowShouldClose(image_window) 
        || !glfwWindowShouldClose(tool_window))
    {
        // Image Window Processing
        glfwMakeContextCurrent(image_window);
        glfwPollEvents();
        
        // General
        PreDraw(IMAGE_WIN_WIDTH, IMAGE_WIN_HEIGHT);

        // Draw
        image.render();

		assertGLError();

		handleCursorColor(image_window);

        // Error check
        assertGLError();

        // Flush and swap buffer
        glFlush();
        glfwSwapBuffers(image_window);


        // Tool Window Processing
        glfwMakeContextCurrent(tool_window);
        glfwPollEvents();

        // General
        PreDraw(TOOL_WIN_WIDTH, TOOL_WIN_HEIGHT);

        handleSliders(tool_window);

        // Draw
        for (int i = toolRenders.size() - 1; i >= 0; i--)
            toolRenders[i]->render();
        saveIcon.render();
        refreshIcon.render();

        assertGLError();

        // Flush and swap buffer
        glFlush();
        glfwSwapBuffers(tool_window);
    }

    glfwDestroyWindow(tool_window);
    glfwDestroyWindow(image_window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

//void testDraws()
//{
//    glColor3f(0.0, 1.0, 0.0);
//    glBegin(GL_POLYGON);
//    glVertex3f(0.0, 0.0, 0.0);
//    glVertex3f(190.0, 0.0, 0.0);
//    glVertex3f(190.0, 190.0, 0.0);
//    glVertex3f(0.0, 190.0, 0.0);
//    glEnd();
//
//    glColor3f(1.0, 0.0, 0.0);
//    glPointSize(200);
//    glBegin(GL_POINTS);
//    glVertex2f(200.0, 200.0);
//    glEnd();
//    glPointSize(1);
//}
