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

int WIN_WIDTH = 1;
int WIN_HEIGHT = 1;

// Master Objects
vector<RenderObject*> renderTargets;
GLFWcursor* g_cursor = nullptr;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
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

GLFWwindow* CreateWindows(GLFWwindow* &image_window, GLFWwindow* &tool_window)
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) 
	{
		exit(EXIT_FAILURE);
	}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    image_window = glfwCreateWindow(WIN_HEIGHT, WIN_WIDTH, "HueShifter", NULL, NULL);
	tool_window = glfwCreateWindow(400, 200, "Tools", NULL, NULL);
    if (!image_window || !tool_window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetKeyCallback(image_window, key_callback);
	glfwSetKeyCallback(tool_window, key_callback);
    glfwMakeContextCurrent(image_window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1); // v-sync

    assertGLError();
}

void PreDraw()
{
    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //      left    right       bottom  top         near    far
    glOrtho(0.0f,   WIN_WIDTH,  0,      WIN_HEIGHT, -1.0f,  1.0f);
    assertGLError();
    //glDisable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
}

void newCursor(double x, double y, GLFWwindow* window)
{
	unsigned int color;
	glReadPixels(x, WIN_HEIGHT-y-1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &color);
	memset(pixels, color, sizeof(pixels));
	cout << "COLOR: " << hex << color << endl;
	cout << "COLOR: " << hex << color << endl;
	unsigned int pixels[16 * 16];
    for (int i = 0; i < 16*16; i += 4)
    {
        pixels[i] = pixels[i+1] = pixels[i+2] = pixels[i+3] = color;
    }
	GLFWimage image;
	image.width = 16;
	image.height = 16;
	image.pixels = (unsigned char*)pixels;
	g_cursor = glfwCreateCursor(&image, 0, 0);
	glfwSetCursor(window, g_cursor);
}

void deleteCursor()
{
	glfwDestroyCursor(g_cursor);
	g_cursor = nullptr;
}

void handleCursor(GLFWwindow* window)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_RELEASE)
	{
		if (g_cursor != nullptr)
			cout << "DEL CURSOR" << endl;
			deleteCursor();
		return;
	}
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	cout << "DB:: Window: " << window << " | Cursor Pos: " << xpos << " " << ypos << endl;
	for (int i = renderTargets.size() - 1; i >= 0; i--)
	{
		if (renderTargets[i]->hovering(xpos, ypos))
		{
			newCursor(xpos, ypos, window);
			return;
		}
	}
}

void testDraws()
{
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(190.0, 0.0, 0.0);
    glVertex3f(190.0, 190.0, 0.0);
    glVertex3f(0.0, 190.0, 0.0);
    glEnd();

    glColor3f(1.0, 0.0, 0.0);
    glPointSize(200);
    glBegin(GL_POINTS);
    glVertex2f(200.0, 200.0);
    glEnd();
    glPointSize(1);
}

int main(void)
{
	GLFWwindow* image_window;
	GLFWwindow* tool_window;
	CreateWindows(image_window, tool_window);
	// Prompt user to choose a file
	Image image("image.png");
	WIN_WIDTH = image.getWidth();
	WIN_HEIGHT = image.getHeight();
	renderTargets.push_back(&image);
	glfwSetWindowSize(image_window, WIN_WIDTH, WIN_HEIGHT);
    while (!glfwWindowShouldClose(image_window))
    {
        // General
        PreDraw();

        // Draw
        for (int i = renderTargets.size() - 1; i >= 0; i--)
            renderTargets[i]->render();

		assertGLError();

		handleCursor(image_window);

        //testDraws();

        // Error check
        assertGLError();

        // Get events and swap
        glFlush();
        glfwSwapBuffers(image_window);
        glfwPollEvents();
    }

    glfwDestroyWindow(image_window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}