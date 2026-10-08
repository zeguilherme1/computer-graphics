#pragma once
#ifndef MESH_H
#define MESH_H

#include <vector>

/* Classe Mesh para gerenciar as manipulações envolvendo renderizações
e controle de buffers do OpenGL
*/
class Mesh {
public:
    // O construtor recebe apenas o array de vertices e indices
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