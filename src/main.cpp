#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <array>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "../include/models/cube.hpp"
#include "../include/models/sphere.hpp"
#include "../include/utils.hpp"
#include "../include/window.hpp"
#define PI M_PI

int main(int argc, char *argv[])
{
    GLFWwindow *window = initWindow();
    glfwMakeContextCurrent(window);

    // loading glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::vector<float> cubeVertices = generateCube();
    unsigned int shaderProgram = initShaders();
    unsigned int cubeVBO = createVBO(cubeVertices.data(), cubeVertices.size() * sizeof(float));
    unsigned int cubeVAO = createVAO(cubeVBO);

    std::vector<float> sphereVertices = generateSphere();
    std::vector<unsigned int> sphereIndices = generateSphereIndices();
    std::vector<unsigned int> sphereLineIndices = generateSphereLines();

    unsigned int sphereVBO = createVBO(sphereVertices.data(), sphereVertices.size() * sizeof(float));
    unsigned int sphereVAO = createVAO(sphereVBO);
    unsigned int sphereEBO;
    glGenBuffers(1, &sphereEBO);
    glBindVertexArray(sphereVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int), sphereIndices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
    int colorLoc = glGetUniformLocation(shaderProgram, "color");

    glEnable(GL_DEPTH_TEST);

    float rotationAngle = 0.0f;
    // render loop

    while (!glfwWindowShouldClose(window))
    {
        processInput(window); // input

        rotationAngle += 0.01f;
        float cos_d = std::cos(rotationAngle);
        float sin_d = std::sin(rotationAngle);

        float translation[16];
        generateTranslationMatrix(0.5, 0.0, 0.0, translation);

        float mat_rotation_z[16] = {cos_d, -sin_d, 0.0f, 0.0f, sin_d, cos_d,
                                    0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f};

        float mat_rotation_x[16] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, cos_d,
                                    -sin_d, 0.0f, 0.0f, sin_d, cos_d, 0.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f};

        float mat_rotation_y[16] = {cos_d, 0.0f, sin_d, 0.0f, 0.0f, 1.0f,
                                    0.0f, 0.0f, -sin_d, 0.0f, cos_d, 0.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f};

        float mat_transform[16];
        float temp[16];
        multMatrix(mat_rotation_z, mat_rotation_y, temp);
        multMatrix(mat_rotation_x, temp, mat_transform);

        float firstSphereTransform[16];
        multMatrix(translation, mat_transform, firstSphereTransform);

        generateTranslationMatrix(0.0, 0.5, 0.0, translation);
        float secondSphereTransform[16];
        multMatrix(translation, mat_transform, secondSphereTransform);

        generateTranslationMatrix(0.0, -0.5, 0.0, translation);
        float thirdSphereTransform[16];
        multMatrix(translation, mat_transform, thirdSphereTransform);

        generateTranslationMatrix(-0.5, 0.0, 0.0, translation);
        float fourthSphereTransform[16];
        multMatrix(translation, mat_transform, fourthSphereTransform);


        int transformationLoc =
            glGetUniformLocation(shaderProgram, "mat_transformation");

        // rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(sphereVAO);

        glUniformMatrix4fv(
            transformationLoc,
            1,
            GL_TRUE,
            firstSphereTransform);

        glDrawElements(
            GL_LINES,
            sphereIndices.size(),
            GL_UNSIGNED_INT,
            0);

        glUniformMatrix4fv(
        transformationLoc,
        1,
        GL_TRUE,
        secondSphereTransform);

        glDrawElements(
            GL_LINES,
            sphereIndices.size(),
            GL_UNSIGNED_INT,
            0);

        glUniformMatrix4fv(
        transformationLoc,
        1,
        GL_TRUE,
        thirdSphereTransform);

        glDrawElements(
            GL_LINES,
            sphereIndices.size(),
            GL_UNSIGNED_INT,
            0);

            glUniformMatrix4fv(
        transformationLoc,
        1,
        GL_TRUE,
        fourthSphereTransform);

        glDrawElements(
            GL_LINES,
            sphereIndices.size(),
            GL_UNSIGNED_INT,
            0);
        glUseProgram(shaderProgram);
        glBindVertexArray(cubeVAO);

        glUniformMatrix4fv(transformationLoc, 1, GL_TRUE, mat_transform);

        // Face 1
        glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // Face 2
        glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 1.0f);
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

        // Face 3
        glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);

        // Face 4
        glUniform4f(colorLoc, 1.0f, 1.0f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);

        // Face 5
        glUniform4f(colorLoc, 0.5f, 0.5f, 0.5f, 1.0f);
        glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);

        // Face 6
        glUniform4f(colorLoc, 0.5f, 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
