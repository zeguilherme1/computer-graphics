#include "../../include/models/tree.hpp"
#include "../../include/utils.hpp"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <vector>
Tree::Tree() : folhasData(64, 0.125f, 0.5f), troncoData(64, 0.03f, 0.25f) { 
    troncoMesh = std::make_unique<Mesh>(troncoData.vertices, troncoData.indices);
    folhasMesh = std::make_unique<Mesh>(folhasData.vertices, folhasData.indices);
};

void Tree::draw(unsigned int shaderProgram, float* baseModelMatrix, int modelLoc) {
    float coneTrans[16], cilindroTrans[16];
    float finalConeModel[16], finalCilindroModel[16];

    generateTranslationMatrix(0.0f, 0.0f, 0.0f, coneTrans);
    generateTranslationMatrix(0.0f, 0.0f, -0.3f, cilindroTrans);

    multMatrix(baseModelMatrix, coneTrans, finalConeModel);
    multMatrix(baseModelMatrix, cilindroTrans, finalCilindroModel);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, finalConeModel);
    folhasMesh->draw();

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, finalCilindroModel);
    troncoMesh->draw();
}