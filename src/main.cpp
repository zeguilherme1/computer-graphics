#include <glad/glad.h>
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

        //Tree
        float treeTrans[16], treeScale[16], treeRot[16], treeModel[16], temp[16]; 
        generateTranslationMatrix(-0.1f, -0.06f, 0.0f, treeTrans);
        generateScaleMatrix(1.7f, 1.7f, 1.7f, treeScale);
        generateRotationXMatrix(-1.5708f, treeRot);

        multMatrix(treeRot, treeScale, temp);
        multMatrix(treeTrans, temp, treeModel);
    
        glUniform4f(colorLoc, 0.1f, 0.6f, 0.2f, 1.0f);
        arvore.draw(shaderProgram, treeModel, modelLoc);

        //Snowman
        float snowmanTrans[16], snowmanScale[16], snowmanRot[16], snowmanModel[16], temp1[16];
        generateTranslationMatrix(-0.7f, -0.4f, 0.0f, snowmanTrans);
        generateScaleMatrix(0.5f, 0.5f, 0.5f, snowmanScale);
        generateRotationYMatrix(angleY, snowmanRot);

        multMatrix(snowmanRot, snowmanScale, temp1);
        multMatrix(snowmanTrans, temp1, snowmanModel);
        
        boneco.draw(shaderProgram, snowmanModel, modelLoc, colorLoc);

        //Biscuit
        float biscuitTrans[16], biscuitScale[16], biscuitRot[16], biscuitModel[16], biscuitTemp[16];
        generateTranslationMatrix(0.4f, -0.5f, 0.0f, biscuitTrans);
        generateScaleMatrix(0.4f, 0.4f, 0.4f, biscuitScale);
        generateRotationYMatrix(2.7, biscuitRot);

        multMatrix(biscuitRot, biscuitScale, biscuitTemp);
        multMatrix(biscuitTrans, biscuitTemp, biscuitModel);
        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, biscuitModel);

        glUniform4f(colorLoc, 0.65f, 0.38f, 0.18f, 1.0f);
        biscoito.mesh->draw(biscoito.bodyIndexOffset, biscoito.bodyIndexCount);

        glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
        biscoito.mesh->draw(biscoito.eyesIndexOffset, biscoito.eyesIndexCount);

        glUniform4f(colorLoc, 0.5f, 0.1f, 0.7f, 1.0f);
        biscoito.mesh->draw(biscoito.buttonsIndexOffset, biscoito.buttonsIndexCount);

        glUniform4f(colorLoc, 0.9f, 0.1f, 0.2f, 1.0f);
        glLineWidth(2.5f);
        biscoito.mesh->draw(biscoito.mouthVertexOffset, biscoito.mouthVertexCount, GL_LINE_STRIP);

        //House
        float houseTrans[16], houseScale[16], houseRot[16], houseModel[16], houseTemp[16];
        generateTranslationMatrix(0.6f, -0.5f, 0.4f, houseTrans);
        generateScaleMatrix(3.4f, 3.4f, 3.4f, houseScale);
        generateRotationYMatrix(2.338, houseRot);

        multMatrix(houseRot, houseScale, houseTemp);
        multMatrix(houseTrans, houseTemp, houseModel);
        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, houseModel);

        glUniform4f(colorLoc, 0.95f, 0.65f, 0.8f, 1.0f);
        house.mesh->draw(house.wallIndexOffset, house.wallIndexCount);

        glUniform4f(colorLoc, 0.86f, 0.50f, 0.95f, 1.0f);
        house.mesh->draw(house.roofIndexOffset, house.roofIndexCount);

        glUniform4f(colorLoc, 0.62f, 0.87f, 0.96f, 1.0f);
        house.mesh->draw(house.windowIndexOffset, house.windowIndexCount);

        glUniform4f(colorLoc, 1.0f, 0.90f, 0.45f, 1.0f);
        house.mesh->draw(house.doorIndexOffset,house.doorIndexCount);

        glUniform4f(colorLoc, 0.86f, 0.50f, 0.95f, 1.0f);
        house.mesh->draw(house.handleIndexOffset, house.handleIndexCount);

        //Cloud 
        float cloudTrans[16], cloudScale[16], cloudModel[16];
        generateTranslationMatrix(-0.4f, 0.7f, 0.0f, cloudTrans);
        generateScaleMatrix(0.5f, 0.5f, 0.5f, cloudScale);

        multMatrix(cloudTrans, cloudScale, cloudModel);
        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, cloudModel);

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