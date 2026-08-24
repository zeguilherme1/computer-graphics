#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

GLFWwindow *initWindow() {
    GLFWwindow *window;

    if (!glfwInit()) {
        std::cerr << "Error trying to initialize the window" << std::endl;
    }

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    if (!window) {
        glfwTerminate();
        std::cerr << "Window was not initialized sucessfully" << std::endl;
    }
    
    return window;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}