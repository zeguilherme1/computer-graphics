#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "../include/utils.hpp"
#include "../include/window.hpp"
#include "../include/models/tree.hpp"
#include "../include/models/snowman.hpp"
#include "../include/models/cloud.hpp"
#include "../include/models/house.hpp"
#include "../include/models/biscoito.hpp"
int main() {
    GLFWwindow *window = initWindow();
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Tree arvore;
    Snowman boneco;
    House house;
    Biscoito biscoito;
    Cloud cloud;

    unsigned int shaderProgram = initShaders();
    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    int colorLoc = glGetUniformLocation(shaderProgram, "color");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.54f, 0.69f, 0.90f, 1.0f); 

    float angleY = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) angleY += 0.02f; 
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) angleY -= 0.02f; 
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        float globalRot[16];
        generateRotationYMatrix(angleY, globalRot);


        float treeRotX[16];
        generateRotationXMatrix(-1.5708f, treeRotX); 

        float treeTrans[16];
        generateTranslationMatrix(-0.5f, 0.0f, 0.0f, treeTrans);
        
        float treeModel[16];

        multMatrix(treeTrans, treeRotX, treeModel);
        
        glUniform4f(colorLoc, 0.1f, 0.6f, 0.2f, 1.0f);
        arvore.draw(shaderProgram, treeModel, modelLoc);

        float snowmanTrans[16], snowmanModel[16];
        generateTranslationMatrix(0.5f, 0.0f, 0.0f, snowmanTrans);

        multMatrix(snowmanTrans, globalRot, snowmanModel);
        
        boneco.draw(shaderProgram, snowmanModel, modelLoc, colorLoc);

        //House
        glUniform4f(colorLoc, 0.95f, 0.65f, 0.8f, 1.0f);
        house.mesh->draw(house.wallIndexOffset, house.wallIndexCount);

        glUniform4f(colorLoc, 0.86f, 0.070f, 0.95f, 1.0f);
        house.mesh->draw(house.roofIndexOffset, house.roofIndexCount);

        glUniform4f(colorLoc, 0.62f, 0.87f, 0.96f, 1.0f);
        house.mesh->draw(house.windowIndexOffset, house.windowIndexCount);

        glUniform4f(colorLoc, 1.0f, 0.90f, 0.45f, 1.0f);
        house.mesh->draw(house.doorIndexOffset,house.doorIndexCount);

        glUniform4f(colorLoc, 0.86f, 0.070f, 0.95f, 1.0f);
        house.mesh->draw(house.handleIndexOffset, house.handleIndexCount);

        //Biscuit
        glUniform4f(colorLoc, 0.65f, 0.38f, 0.18f, 1.0f);
        biscoito.mesh->draw(biscoito.bodyIndexOffset, biscoito.bodyIndexCount);

        glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
        biscoito.mesh->draw(biscoito.eyesIndexOffset, biscoito.eyesIndexCount);

        glUniform4f(colorLoc, 0.5f, 0.1f, 0.7f, 1.0f);
        biscoito.mesh->draw(biscoito.buttonsIndexOffset, biscoito.buttonsIndexCount);

        glUniform4f(colorLoc, 0.9f, 0.1f, 0.2f, 1.0f);
        glLineWidth(2.5f);
        biscoito.mesh->draw(biscoito.mouthVertexOffset, biscoito.mouthVertexCount, GL_LINE_STRIP);

        //Cloud 
        glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
        cloud.mesh->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}