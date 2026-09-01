#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

GLFWwindow *initWindow() {
    GLFWwindow *window;

    if (!glfwInit()) {
        std::cerr << "Error trying to initialize the window" << std::endl;
    }

    window = glfwCreateWindow(1322, 768, "Trabalho 01 - Uma noite de Natal", NULL, NULL);

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