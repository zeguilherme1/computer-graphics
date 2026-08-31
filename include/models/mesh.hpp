#pragma once
#ifndef MESH_H
#define MESH_H

#include <vector>

class Mesh {
public:
    Mesh(
        const std::vector<float>& vertices,
        const std::vector<unsigned int>& indices
    );

    ~Mesh();

    void draw() const;
    void draw(int indexOffset, int indexCount) const;
    void draw(int vertexOffset, int vertexCount, unsigned int mode) const;

private:
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    unsigned int indexCount;
};

#endif