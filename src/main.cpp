#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "../include/utils.hpp"
#include "../include/window.hpp"
#include "../include/models/tree.hpp"
#include "../include/models/snowman.hpp"

int main() {
    GLFWwindow *window = initWindow();
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Tree arvore;
    Snowman boneco;

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

        float treeTrans[16], treeModel[16];
        generateTranslationMatrix(-0.5f, 0.0f, 0.0f, treeTrans);
        multMatrix(globalRot, treeTrans, treeModel);
        
        glUniform4f(colorLoc, 0.1f, 0.6f, 0.2f, 1.0f);
        arvore.draw(shaderProgram, treeModel, modelLoc);


        float snowmanTrans[16], snowmanModel[16];
        generateTranslationMatrix(0.5f, 0.0f, 0.0f, snowmanTrans);
        multMatrix(globalRot, snowmanTrans, snowmanModel);
        
        boneco.draw(shaderProgram, snowmanModel, modelLoc, colorLoc);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}