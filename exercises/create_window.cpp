#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include "window.hpp"

int main(int argc, char *argv[]) {
	GLFWwindow* window = initWindow();
	glfwMakeContextCurrent(window);

	// loading glad	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
	

	// render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window); // input 
		
	
		// rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// check and call events and swap the buffers
		glfwPollEvents();	
		glfwSwapBuffers(window);	
	}


	glfwTerminate();
	return 0;	
}
