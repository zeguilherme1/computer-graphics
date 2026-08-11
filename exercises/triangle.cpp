#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <array>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include "utils.hpp"
#include "window.hpp"

int main(int argc, char *argv[]) {
    GLFWwindow *window = initWindow();
    glfwMakeContextCurrent(window);

    // loading glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    float vertices[] = {
		-0.5f, -0.5f, 0.0f, 
		0.5f, -0.5f, 0.0f, 
		0.0f, 0.5f, 0.0f
	};

    const char *vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkCompilationStatus(vertexShader);
    
    const char *fragmentShaderSource = 
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkCompilationStatus(fragmentShader);

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	
	unsigned int VBO = createAndSetBuffer(vertices, sizeof(vertices));
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window); // input

        // rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);			
	
        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

	
    glfwTerminate();
    return 0;
}
