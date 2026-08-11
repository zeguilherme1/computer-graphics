#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <array>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include "utils.hpp"
#include "window.hpp"

int main(int argc, char *argv[]) {
    GLFWwindow *window = initWindow();
    glfwMakeContextCurrent(window);

    // loading glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                        0.0f,  0.0f,  0.5f, 0.0f};

    unsigned int shaderProgram = initShaders();
    unsigned int VBO = createVBO(vertices, sizeof(vertices));
    unsigned int VAO = createVAO(VBO);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window); // input

        // rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
