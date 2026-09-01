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
#include "../include/models/snowFloor.hpp"

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
    Cloud cloudLeft;
    Cloud cloudMiddle;
    Cloud cloudRight;
    SnowFloor floor;

    unsigned int shaderProgram = initShaders();
    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    int colorLoc = glGetUniformLocation(shaderProgram, "color");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.024f, 0.118f, 0.412f, 1.0f); 

    float angleY = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) angleY += 0.02f; 
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) angleY -= 0.02f; 
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        // Tree
        float treeTrans[16], treeScale[16], treeRotX[16], treeRotY[16], treeModel[16], tempTree[16], tempTree1[16];

        generateTranslationMatrix(-0.16f, -0.06f, 0.0f, treeTrans);
        generateScaleMatrix(1.7f, 1.7f, 1.7f, treeScale);
        generateRotationXMatrix(-1.5708f, treeRotX);
        generateRotationYMatrix(angleY, treeRotY);

        multMatrix(treeRotX, treeScale, tempTree);
        multMatrix(treeRotY, tempTree, tempTree1);
        multMatrix(treeTrans, tempTree1, treeModel);

        arvore.draw(shaderProgram, treeModel, modelLoc, colorLoc);
        
        //Snowman
        float snowmanTrans[16], snowmanScale[16], snowmanRot[16], snowmanYRot[16], snowmanModel[16], tempSnowman[16], tempSnowman1[16];
        generateTranslationMatrix(-0.7f, -0.4f, 0.0f, snowmanTrans);
        generateScaleMatrix(0.5f, 0.5f, 0.5f, snowmanScale);
        generateRotationYMatrix(2.7f, snowmanYRot);
        generateRotationYMatrix(angleY, snowmanRot);

        multMatrix(snowmanYRot, snowmanScale, tempSnowman);
        multMatrix(snowmanRot, tempSnowman, tempSnowman1);
        multMatrix(snowmanTrans, tempSnowman1, snowmanModel);
        
        boneco.draw(shaderProgram, snowmanModel, modelLoc, colorLoc);

        //Biscuit
        float biscuitTrans[16], biscuitScale[16], biscuitRot[16], biscuitModel[16], biscuitTemp[16];
        generateTranslationMatrix(0.15f, -0.72f, 0.0f, biscuitTrans);
        generateScaleMatrix(0.2f, 0.2f, 0.2f, biscuitScale);
        generateRotationYMatrix(2.7, biscuitRot);

        multMatrix(biscuitRot, biscuitScale, biscuitTemp);
        multMatrix(biscuitTrans, biscuitTemp, biscuitModel);
        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, biscuitModel);

        glUniform4f(colorLoc, 0.85f, 0.639f, 0.392f, 1.0f);
        biscoito.mesh->draw(biscoito.bodyIndexOffset, biscoito.bodyIndexCount);

        glUniform4f(colorLoc, 0.96f, 0.918f, 0.855f, 1.0f);
        biscoito.mesh->draw(biscoito.eyesIndexOffset, biscoito.eyesIndexCount);

        glUniform4f(colorLoc, 0.5f, 0.1f, 0.7f, 1.0f);
        biscoito.mesh->draw(biscoito.buttonsIndexOffset, biscoito.buttonsIndexCount);

        glUniform4f(colorLoc, 0.95f, 0.3f, 0.227f, 1.0f);
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

        glUniform4f(colorLoc, 0.96f, 0.9f, 0.58f, 1.0f);
        house.mesh->draw(house.wallIndexOffset, house.wallIndexCount);

        glUniform4f(colorLoc, 0.86f, 0.27f, 0.153f, 1.0f);
        house.mesh->draw(house.roofIndexOffset, house.roofIndexCount);

        glUniform4f(colorLoc, 0.62f, 0.87f, 0.96f, 1.0f);
        house.mesh->draw(house.windowIndexOffset, house.windowIndexCount);

        glUniform4f(colorLoc, 0.78f, 0.573f, 0.412f, 1.0f);
        house.mesh->draw(house.doorIndexOffset,house.doorIndexCount);

        glUniform4f(colorLoc, 0.522f, 0.318f, 0.16f, 1.0f);
        house.mesh->draw(house.handleIndexOffset, house.handleIndexCount);

        //CloudLeft
        float cloudTrans[16], cloudScale[16], cloudModel[16];
        generateTranslationMatrix(-0.6f, 0.6f, 0.0f, cloudTrans);
        generateScaleMatrix(0.4f, 0.4f, 0.4f, cloudScale);

        multMatrix(cloudTrans, cloudScale, cloudModel);
        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, cloudModel);

        glUniform4f(colorLoc, 0.9f, 0.9f, 0.9f, 1.0f);
        cloudLeft.mesh->draw();

        //CloudMiddle
        generateTranslationMatrix(0.0f, 0.7f, 0.0f, cloudTrans);
        generateScaleMatrix(0.5f, 0.5f, 0.5f, cloudScale);
        multMatrix(cloudTrans, cloudScale, cloudModel);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, cloudModel);
        glUniform4f(colorLoc, 0.9f, 0.9f, 0.9f, 1.0f);
        cloudLeft.mesh->draw();

        //CloudRight
        generateTranslationMatrix(0.6f, 0.6f, 0.0f, cloudTrans);
        generateScaleMatrix(0.4f, 0.4f, 0.4f, cloudScale);
        multMatrix(cloudTrans, cloudScale, cloudModel);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, cloudModel);
        glUniform4f(colorLoc, 0.9f, 0.9f, 0.9f, 1.0f);
        cloudLeft.mesh->draw();
        
        //SnowFloor
        float floorTrans[16], floorScale[16], floorModel[16];
        generateTranslationMatrix(0.0f, -0.85f, 0.9f, floorTrans);
        generateScaleMatrix(1.6f, 0.8f, 1.6f, floorScale);

        multMatrix(floorTrans, floorScale, floorModel);
        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, floorModel);

        glUniform4f(colorLoc, 0.9f, 0.9f, 0.9f, 1.0f);
        floor.mesh->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}