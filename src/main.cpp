#ifdef _WIN32
#define APIENTRY __stdcall
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
    GLFWwindow* image_window;
	GLFWwindow* tool_window;

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    image_window = glfwCreateWindow(900, 600, "HueShifter", NULL, NULL);
	tool_window = glfwCreateWindow(300, 600, "Tools", NULL, NULL);

    if (!image_window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetKeyCallback(image_window, key_callback);
    glfwMakeContextCurrent(image_window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1); // v-sync

	if (!tool_window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetKeyCallback(image_window, key_callback);
    glfwMakeContextCurrent(image_window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1); // v-sync
    
    while (!glfwWindowShouldClose(image_window))
    {
        // General
        float ratio;
        int width, height;
        glfwGetFramebufferSize(image_window, &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Draw



        // Get events and swap
        glfwSwapBuffers(image_window);
        glfwPollEvents();
    }
    glfwDestroyWindow(image_window);
	glfwDestroyWindow(tool_window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}