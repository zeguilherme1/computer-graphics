#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "../../include/models/mesh.hpp"

Mesh::Mesh(
    const std::vector<float>& vertices,
    const std::vector<unsigned int>& indices
) {
    indexCount = indices.size();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int),
        indices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Mesh::draw() const {
    glBindVertexArray(vao);

    glDrawElements(
        GL_TRIANGLES,
        indexCount,
        GL_UNSIGNED_INT,
        0
    );

    glBindVertexArray(0);
}

void Mesh::draw(int indexOffset, int count) const {
    glBindVertexArray(vao);

    glDrawElements(
        GL_TRIANGLES,
        count,
        GL_UNSIGNED_INT,
        (void*)(indexOffset * sizeof(unsigned int))
    );

    glBindVertexArray(0);
}

void Mesh::draw(int vertexOffset, int vertexCount, unsigned int mode) const {
    glBindVertexArray(vao);

    glDrawArrays(
        mode,
        vertexOffset,
        vertexCount
    );

    glBindVertexArray(0);
}