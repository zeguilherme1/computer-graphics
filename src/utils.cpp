#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <sstream>

void multMatrix(const float a[16], const float b[16], float result[16]) {
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            result[col * 4 + row] = 0.0f;

            for (int k = 0; k < 4; k++) {
                result[col * 4 + row] += a[k * 4 + row] * b[col * 4 + k];
            }
        }
    }
}

void generateTranslationMatrix(
    float tx,
    float ty,
    float tz,
    float result[16]
) {
    result[0]  = 1.0f;
    result[1]  = 0.0f;
    result[2]  = 0.0f;
    result[3]  = 0.0f;

    result[4]  = 0.0f;
    result[5]  = 1.0f;
    result[6]  = 0.0f;
    result[7]  = 0.0f;

    result[8]  = 0.0f;
    result[9]  = 0.0f;
    result[10] = 1.0f;
    result[11] = 0.0f;

    result[12] = tx;
    result[13] = ty;
    result[14] = tz;
    result[15] = 1.0f;
}

void generateScaleMatrix(
    float sx,
    float sy,
    float sz,
    float result[16]
) {
    result[0]  = sx;
    result[1]  = 0.0f;
    result[2]  = 0.0f;
    result[3]  = 0.0f;

    result[4]  = 0.0f;
    result[5]  = sy;
    result[6]  = 0.0f;
    result[7]  = 0.0f;

    result[8]  = 0.0f;
    result[9]  = 0.0f;
    result[10] = sz;
    result[11] = 0.0f;

    result[12] = 0.0f;
    result[13] = 0.0f;
    result[14] = 0.0f;
    result[15] = 1.0f;
}

void generateRotationXMatrix(float angle, float result[16]) {
    for (int i = 0; i < 16; i++) result[i] = 0.0f;

    
    result[0] = 1.0f;
    result[15] = 1.0f;

    float c = std::cos(angle);
    float s = std::sin(angle);

    result[5] = c;
    result[6] = s;
    result[9] = -s;
    result[10] = c;
} 

void generateRotationYMatrix(float angle, float result[16]) {
    for (int i = 0; i < 16; i++) result[i] = 0.0f;

    
    result[5] = 1.0f;
    result[15] = 1.0f;

    float c = std::cos(angle);
    float s = std::sin(angle);

    result[0] = c;
    result[2] = -s;
    result[8] = s;
    result[10] = c;
}


void checkCompilationStatus(unsigned int vertex_shader) {
    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        std::cerr << "ERROR" << infoLog << std::endl;
    }
}

unsigned int createVBO(float vertices[], size_t size) {

    // Vertex Buffer Object
    // creates vertex buffer object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    return VBO;
}

unsigned int createVAO(unsigned int VBO) {
    // creates vertex array object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    return VAO;
}

unsigned int initShaders() {
    // this function setup all shaders
    // including vertex shader and fragment shader
    
	// should refactor this filepath method
    std::ifstream vertex_shader_file("/home/lele/computer-graphics/shaders/vertex.glsl");
    std::stringstream vertex_buffer;
    vertex_buffer << vertex_shader_file.rdbuf();
    std::string vertex_shader_source = vertex_buffer.str();
    vertex_shader_file.close();

    const char* vertex_source = vertex_shader_source.c_str();
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);
    checkCompilationStatus(vertex_shader);

	// we should refactor this too
    std::ifstream fragment_shader_file("/home/lele/computer-graphics/shaders/fragment.glsl");
    std::stringstream fragment_buffer;
    fragment_buffer << fragment_shader_file.rdbuf();
    std::string fragment_shader_source = fragment_buffer.str();
    fragment_shader_file.close();

    const char* fragment_source = fragment_shader_source.c_str();
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader);
    checkCompilationStatus(fragment_shader);

    unsigned int shader_program;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}

