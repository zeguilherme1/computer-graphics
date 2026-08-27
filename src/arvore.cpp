#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <array>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "../include/models/cube.hpp"
#include "../include/models/cone.hpp"
#include "../include/models/sphere.hpp"
#include "../include/models/cilinder.hpp"
#include "../include/utils.hpp"
#include "../include/window.hpp"

int main() {
    GLFWwindow *window = initWindow();

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // r  h
    Cone cone_arvore(64, 0.125f, 0.5f);
    Cilinder cilindro_arvore(64, 0.03f, 0.25f);
    std::cout << "vertices: " << cone_arvore.vertices.size() << std::endl;
    std::cout << "indices: " << cone_arvore.indices.size() << std::endl;

    unsigned int shaderProgram = initShaders();

    unsigned int coneVBO =
        createVBO(cone_arvore.vertices.data(),
                  cone_arvore.vertices.size() * sizeof(float));

    unsigned int coneVAO = createVAO(coneVBO);

    unsigned int coneEBO;
    glGenBuffers(1, &coneEBO);

    glBindVertexArray(coneVAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, coneEBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 cone_arvore.indices.size() * sizeof(unsigned int),
                 cone_arvore.indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    unsigned int cilindroVBO =
        createVBO(cilindro_arvore.vertices.data(),
                  cilindro_arvore.vertices.size() * sizeof(float));

    unsigned int cilindroVAO = createVAO(cilindroVBO);
    glBindVertexArray(cilindroVAO);
    unsigned int cilindroEBO;
    glGenBuffers(1, &cilindroEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cilindroEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 cilindro_arvore.indices.size() * sizeof(unsigned int),
                 cilindro_arvore.indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);

    glUseProgram(shaderProgram);
    int colorLoc = glGetUniformLocation(shaderProgram, "color");
    glUniform4f(colorLoc, 0.1f, 1.0f, 0.2f, 1.0f);
    int modelLoc = glGetUniformLocation(shaderProgram, "model");

    float coneModel[16];
    float cilindroModel[16];


    float angleX = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            angleX += 0.02f; 
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            angleX -= 0.02f; 
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        float rotMatrix[16];
        generateRotationXMatrix(angleX, rotMatrix);

        float coneTrans[16];
        float cilindroTrans[16];
        generateTranslationMatrix(0.0f, 0.0f, 0.0f, coneTrans);
        generateTranslationMatrix(0.0f, 0.0f, -0.3f, cilindroTrans);

        float finalConeModel[16];
        float finalCilindroModel[16];

        multMatrix(rotMatrix, coneTrans, finalConeModel);
        multMatrix(rotMatrix, cilindroTrans, finalCilindroModel);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, finalConeModel);
        glBindVertexArray(coneVAO);
        glDrawElements(GL_TRIANGLES, cone_arvore.indices.size(), GL_UNSIGNED_INT, 0);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, finalCilindroModel);
        glBindVertexArray(cilindroVAO);
        glDrawElements(GL_TRIANGLES, cilindro_arvore.indices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteBuffers(1, &coneEBO);
    glDeleteBuffers(1, &coneVBO);
    glDeleteVertexArrays(1, &coneVAO);
    glDeleteBuffers(1, &cilindroEBO);
    glDeleteBuffers(1, &cilindroVBO);
    glDeleteVertexArrays(1, &cilindroVAO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
