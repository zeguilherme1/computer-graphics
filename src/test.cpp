#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <array>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include "../include/utils.hpp"
#include "../include/window.hpp"

int main(int argc, char *argv[]) {
    GLFWwindow *window = initWindow();
    glfwMakeContextCurrent(window);

    // loading glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    float vertices[] = {-0.2f, -0.2f, 0.2f,  0.2f,  -0.2f, 0.2f,
                        -0.2f, 0.2f,  0.2f,  0.2f,  0.2f,  0.2f,

                        0.2f,  -0.2f, 0.2f,  0.2f,  -0.2f, -0.2f,
                        0.2f,  0.2f,  0.2f,  0.2f,  0.2f,  -0.2f,

                        0.2f,  -0.2f, -0.2f, -0.2f, -0.2f, -0.2f,
                        0.2f,  0.2f,  -0.2f, -0.2f, 0.2f,  -0.2f,

                        -0.2f, -0.2f, -0.2f, -0.2f, -0.2f, 0.2f,
                        -0.2f, 0.2f,  -0.2f, -0.2f, 0.2f,  0.2f,

                        -0.2f, -0.2f, -0.2f, 0.2f,  -0.2f, -0.2f,
                        -0.2f, -0.2f, 0.2f,  0.2f,  -0.2f, 0.2f,

                        -0.2f, 0.2f,  0.2f,  0.2f,  0.2f,  0.2f,
                        -0.2f, 0.2f,  -0.2f, 0.2f,  0.2f,  -0.2f};

    unsigned int shader_program = initShaders();
    unsigned int VBO = createVBO(vertices, sizeof(vertices));
    unsigned int VAO = createVAO(VBO);

    int color_loc = glGetUniformLocation(shader_program, "color");

    glEnable(GL_DEPTH_TEST);

    float rotation_angle = 0.0f;
    // render loop

    while (!glfwWindowShouldClose(window)) {
        processInput(window); // input

        rotation_angle += 0.01f;
        float cos_d = std::cos(rotation_angle);
        float sin_d = std::sin(rotation_angle);

        float mat_rotation_z[16] = {cos_d, -sin_d, 0.0f, 0.0f, sin_d, cos_d,
                                    0.0f,  0.0f,   0.0f, 0.0f, 1.0f,  0.0f,
                                    0.0f,  0.0f,   0.0f, 1.0f};

        float mat_rotation_x[16] = {1.0f,   0.0f, 0.0f, 0.0f,  0.0f,  cos_d,
                                    -sin_d, 0.0f, 0.0f, sin_d, cos_d, 0.0f,
                                    0.0f,   0.0f, 0.0f, 1.0f};

        float mat_rotation_y[16] = {cos_d, 0.0f, sin_d,  0.0f, 0.0f,  1.0f,
                                    0.0f,  0.0f, -sin_d, 0.0f, cos_d, 0.0f,
                                    0.0f,  0.0f, 0.0f,   1.0f};

        float mat_transform[16];
        float temp[16];
        multMatrix(mat_rotation_z, mat_rotation_y, temp);
        multMatrix(mat_rotation_x, temp, mat_transform);

        int transformation_loc =
            glGetUniformLocation(shader_program, "mat_transformation");

        // rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(VAO);

        glUniformMatrix4fv(transformation_loc, 1, GL_TRUE, mat_transform);

        // Face 1
        glUniform4f(color_loc, 1.0f, 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // Face 2
        glUniform4f(color_loc, 0.0f, 0.0f, 1.0f, 1.0f);
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

        // Face 3
        glUniform4f(color_loc, 0.0f, 1.0f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);

        // Face 4
        glUniform4f(color_loc, 1.0f, 1.0f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);

        // Face 5
        glUniform4f(color_loc, 0.5f, 0.5f, 0.5f, 1.0f);
        glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);

        // Face 6
        glUniform4f(color_loc, 0.5f, 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
