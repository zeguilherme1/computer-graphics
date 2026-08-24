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
    #include "../include/utils.hpp"
    #include "../include/window.hpp"
    #define PI M_PI


    int main() {
        GLFWwindow *window = initWindow();

        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        Cone meucone(64, 0.5f, 2.0f);
        std::cout << "vertices: " << meucone.vertices.size() << std::endl;
        std::cout << "indices: " << meucone.indices.size() << std::endl;

        unsigned int shaderProgram = initShaders();

        unsigned int VBO = createVBO(
            meucone.vertices.data(),
            meucone.vertices.size() * sizeof(float)
        );

        unsigned int VAO = createVAO(VBO);

        unsigned int EBO;

        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            meucone.indices.size() * sizeof(unsigned int),
            meucone.indices.data(),
            GL_STATIC_DRAW
        );

        glBindVertexArray(0);

        glEnable(GL_DEPTH_TEST);

        glUseProgram(shaderProgram);
        int colorLoc = glGetUniformLocation(shaderProgram, "color");
        glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f);
        while (!glfwWindowShouldClose(window)) {

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glUseProgram(shaderProgram);

            glBindVertexArray(VAO);

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawElements(GL_TRIANGLES, meucone.indices.size(), GL_UNSIGNED_INT, 0);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            glBindVertexArray(0);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glDeleteBuffers(1, &EBO);
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
        glDeleteProgram(shaderProgram);

        glfwDestroyWindow(window);
        glfwTerminate();

        return 0;
    }