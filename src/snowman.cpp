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

void addSphere(std::vector<float>& Vertices,
               std::vector<unsigned int>& Indices,
                float radius, float offsetY, 
                unsigned int sectorCount, unsigned int stackCount){
    unsigned int indexOffset = Vertices.size() / 3;
    Sphere temp_sphere(sectorCount, stackCount, radius);

    for(size_t i = 0; i < temp_sphere.vertices.size(); i += 3){
        Vertices.push_back(temp_sphere.vertices[i]);
        Vertices.push_back(temp_sphere.vertices[i + 1] + offsetY); //desloca as esfera em Y
        Vertices.push_back(temp_sphere.vertices[i + 2]);
    }
    for(unsigned int index : temp_sphere.indices){
        Indices.push_back(index + indexOffset);
    }
}

int main() {
    GLFWwindow *window = initWindow();

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    //Body
    std::vector<float> snowmanVertices;
    std::vector<unsigned int> snowmanIndices;

    addSphere(snowmanVertices, snowmanIndices, 0.35f, -0.40f, 32, 16); //Base
    addSphere(snowmanVertices, snowmanIndices, 0.25f, 0.05f, 32, 16); //Middle
    addSphere(snowmanVertices, snowmanIndices, 0.15f, 0.40f, 32, 16); //Head

    //Nose
    Cone carrot(16, 0.06f, 0.04f);
    
    std::cout << "vertices: " << (snowmanVertices.size() + carrot.vertices.size()) << std::endl;
    std::cout << "indices: " << (snowmanIndices.size() + carrot.indices.size()) << std::endl;

    unsigned int shaderProgram = initShaders();

    unsigned int snowmanVBO =
        createVBO(snowmanVertices.data(),
                  snowmanVertices.size() * sizeof(float));

    unsigned int snowmanVAO = createVAO(snowmanVBO);

    unsigned int snowmanEBO;
    glGenBuffers(1, &snowmanEBO);

    glBindVertexArray(snowmanVAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, snowmanEBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 snowmanIndices.size() * sizeof(unsigned int),
                 snowmanIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    unsigned int carrotVBO =
        createVBO(carrot.vertices.data(),
                  carrot.vertices.size() * sizeof(float));

    unsigned int carrotVAO = createVAO(carrotVBO);

    unsigned int carrotEBO;
    glGenBuffers(1, &carrotEBO);

    glBindVertexArray(carrotVAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, carrotEBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 carrot.indices.size() * sizeof(unsigned int),
                 carrot.indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    

    glEnable(GL_DEPTH_TEST);

    glUseProgram(shaderProgram);
    int colorLoc = glGetUniformLocation(shaderProgram, "color");
    int modelLoc = glGetUniformLocation(shaderProgram, "model");

    float snowmanModel[16];

    float angleY = -1.345f;

    while (!glfwWindowShouldClose(window)) {
        
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            angleY += 0.02f; 
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            angleY -= 0.02f; 
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        float rotMatrix[16];
        generateRotationYMatrix(angleY, rotMatrix);
        
        float snowmanTrans[16];
        generateTranslationMatrix(0.0f, 0.0f, 0.0f, snowmanTrans);

        float finalsnowmanModel[16];

        multMatrix(rotMatrix, snowmanTrans, finalsnowmanModel);
        
        glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, finalsnowmanModel);
        glBindVertexArray(snowmanVAO);
        glDrawElements(GL_TRIANGLES, snowmanIndices.size(), GL_UNSIGNED_INT, 0);

        float carrotTrans[16], finalCarrotModel [16];
        generateTranslationMatrix(0.0f, 0.40f, 0.16f, carrotTrans);
        multMatrix(rotMatrix, carrotTrans, finalCarrotModel);

        glUniform4f(colorLoc, 1.0f, 0.5f, 0.0f, 1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, finalCarrotModel);
        glBindVertexArray(carrotVAO);
        glDrawElements(GL_TRIANGLES, carrot.indices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteBuffers(1, &snowmanEBO);
    glDeleteBuffers(1, &snowmanVBO);
    glDeleteVertexArrays(1, &snowmanVAO);
    glDeleteBuffers(1, &carrotEBO);
    glDeleteBuffers(1, &carrotVBO);
    glDeleteVertexArrays(1, &carrotVAO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}