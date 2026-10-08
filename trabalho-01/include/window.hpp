#pragma once
#ifndef WINDOW_H
#define WINDOW_H
#include <GLFW/glfw3.h>

// Declarações das funções da janela
GLFWwindow *initWindow();

void processInput(GLFWwindow *window);

#endif
