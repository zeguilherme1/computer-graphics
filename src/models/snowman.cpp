#include "../../include/models/snowman.hpp"
#include "../../include/utils.hpp"
#include "../../include/models/sphere.hpp"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <vector>

Snowman::Snowman() : noseData(16, 0.06f, 0.04f) {
    std::vector<float> bodyVertices;
    std::vector<unsigned int> bodyIndices;

    addSphereToBody(bodyVertices, bodyIndices, 0.35f, -0.40f, 32, 16);
    addSphereToBody(bodyVertices, bodyIndices, 0.25f,  0.05f, 32, 16); 
    addSphereToBody(bodyVertices, bodyIndices, 0.15f,  0.40f, 32, 16);

    bodyMesh = std::make_unique<Mesh>(bodyVertices, bodyIndices);
    noseMesh = std::make_unique<Mesh>(noseData.vertices, noseData.indices);
}

void Snowman::addSphereToBody(std::vector<float>& vertices, 
                              std::vector<unsigned int>& indices, 
                              float radius, float offsetY, 
                              unsigned int sectorCount, unsigned int stackCount) {
    unsigned int indexOffset = vertices.size() / 3;
    Sphere temp_sphere(sectorCount, stackCount, radius);

    for(size_t i = 0; i < temp_sphere.vertices.size(); i += 3){
        vertices.push_back(temp_sphere.vertices[i]);
        vertices.push_back(temp_sphere.vertices[i + 1] + offsetY);
        vertices.push_back(temp_sphere.vertices[i + 2]);
    }
    for(unsigned int index : temp_sphere.indices){
        indices.push_back(index + indexOffset);
    }
}

void Snowman::draw(unsigned int shaderProgram, float* baseModelMatrix, int modelLoc, int colorLoc) {
    glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, baseModelMatrix);
    bodyMesh->draw();

    float carrotTrans[16], finalCarrotModel[16];

    generateTranslationMatrix(0.0f, 0.40f, 0.16f, carrotTrans);
    multMatrix(baseModelMatrix, carrotTrans, finalCarrotModel);

    glUniform4f(colorLoc, 1.0f, 0.5f, 0.0f, 1.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, finalCarrotModel);
    noseMesh->draw();
}