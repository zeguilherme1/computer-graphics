#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "../include/models/biscoito.hpp"
#include "../include/utils.hpp"
#include "../include/window.hpp"

int main() {
    GLFWwindow *window = initWindow();
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Falha ao carregar GLAD\n";
        return -1;
    }

    Biscoito biscoito;

    unsigned int shaderProgram = initShaders();

    unsigned int VBO = createVBO(biscoito.biscuitVertices.data(), biscoito.biscuitVertices.size() * sizeof(float));

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    float identityModel[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    glClearColor(0.12f, 0.12f, 0.15f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, identityModel);

        int colorLoc = glGetUniformLocation(shaderProgram, "color");

        glBindVertexArray(VAO);

        // Body
        glUniform4f(colorLoc, 0.65f, 0.38f, 0.18f, 1.0f);
        glLineWidth(2.5f);

        int currentOffset = biscoito.bodyOffset;
        for (int count : biscoito.bodyPartCounts) {
            glDrawArrays(GL_LINE_LOOP, currentOffset, count);
            currentOffset += count;
        }

        // 2 Eyes
        glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
        for (int i = 0; i < 2; ++i) {
            glDrawArrays(GL_LINE_LOOP, biscoito.eyesOffset + (i * 12), 12);
        }

        // 3 Buttons
        glUniform4f(colorLoc, 0.5f, 0.1f, 0.7f, 1.0f);
        for (int i = 0; i < 3; ++i) {
            glDrawArrays(GL_LINE_LOOP, biscoito.buttonsOffset + (i * 10), 10);
        }

        // Mouth
        glUniform4f(colorLoc, 0.9f, 0.1f, 0.2f, 1.0f);
        glLineWidth(2.5f);
        glDrawArrays(GL_LINE_STRIP, biscoito.mouthOffset, biscoito.mouthCount);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}