#include "../../include/models/tree.hpp"
#include "../../include/utils.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

Tree::Tree()
    : folhasData(64, 0.125f, 0.5f),
      troncoData(64, 0.03f, 0.25f),
      estrelaData(0.05f, 0.02f) {

    troncoMesh = std::make_unique<Mesh>(
        troncoData.vertices,
        troncoData.indices
    );

    folhasMesh = std::make_unique<Mesh>(
        folhasData.vertices,
        folhasData.indices
    );

    estrelaMesh = std::make_unique<Mesh>(
        estrelaData.vertices,
        estrelaData.indices
    );

    Sphere sphereData(16, 8, 0.018f);

    // Posições das bolas reescaladas para ~90% do raio da superfície do
    // cone na respectiva altura Z, para que fiquem "penduradas" na casca
    // da árvore em vez de enterradas dentro do sólido.
    bolasPositions = {
        {-0.0945f,  0.0000f, -0.15f},
        { 0.0339f,  0.0883f, -0.15f},
        { 0.0795f, -0.0511f, -0.15f},

        {-0.0588f, -0.0375f, -0.06f},
        { 0.0000f,  0.0698f, -0.06f},
        { 0.0688f,  0.0115f, -0.06f},
        { 0.0129f, -0.0686f, -0.06f},

        {-0.0363f,  0.0227f,  0.06f},
        { 0.0263f,  0.0338f,  0.06f},
        { 0.0000f, -0.0428f,  0.06f},

        {-0.0201f,  0.0101f,  0.15f},
        { 0.0176f, -0.0141f,  0.15f}
    };

    for (size_t i = 0; i < bolasPositions.size(); ++i) {
        bolasMesh.push_back(
            std::make_unique<Mesh>(
                sphereData.vertices,
                sphereData.indices
            )
        );
    }
}

void Tree::draw(unsigned int shaderProgram, float* baseModelMatrix, int modelLoc, int colorLoc) {
    float coneTrans[16];
    float cilindroTrans[16];
    float estrelaTrans[16];
    float estrelaRot[16];
    float estrelaLocal[16];

    float finalConeModel[16];
    float finalCilindroModel[16];
    float finalEstrelaModel[16];

    generateTranslationMatrix(
        0.0f,
        0.0f,
        0.0f,
        coneTrans
    );

    generateTranslationMatrix(
        0.0f,
        0.0f,
        -0.3f,
        cilindroTrans
    );

    generateTranslationMatrix(
        0.0f,
        0.0f,
        0.27f,
        estrelaTrans
    );

    generateRotationXMatrix(
        1.5708f,
        estrelaRot
    );

    multMatrix(
        estrelaTrans,
        estrelaRot,
        estrelaLocal
    );

    multMatrix(
        baseModelMatrix,
        coneTrans,
        finalConeModel
    );

    multMatrix(
        baseModelMatrix,
        cilindroTrans,
        finalCilindroModel
    );

    multMatrix(
        baseModelMatrix,
        estrelaLocal,
        finalEstrelaModel
    );

    glUniformMatrix4fv(
        modelLoc,
        1,
        GL_FALSE,
        finalConeModel
    );

    glUniform4f(
        colorLoc,
        0.02f,
        0.25f,
        0.05f,
        1.0f
    );

    folhasMesh->draw();

    glUniformMatrix4fv(
        modelLoc,
        1,
        GL_FALSE,
        finalCilindroModel
    );

    glUniform4f(
        colorLoc,
        0.36f,
        0.16f,
        0.05f,
        1.0f
    );

    troncoMesh->draw();

    for (size_t i = 0; i < bolasMesh.size(); ++i) {
        float bolaTrans[16];
        float bolaModel[16];

        generateTranslationMatrix(
            bolasPositions[i][0],
            bolasPositions[i][1],
            bolasPositions[i][2],
            bolaTrans
        );

        multMatrix(
            baseModelMatrix,
            bolaTrans,
            bolaModel
        );

        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            bolaModel
        );

        switch (i % 4) {
            case 0:
                glUniform4f(
                    colorLoc,
                    0.9f,
                    0.05f,
                    0.05f,
                    1.0f
                );
                break;

            case 1:
                glUniform4f(
                    colorLoc,
                    0.95f,
                    0.75f,
                    0.05f,
                    1.0f
                );
                break;

            case 2:
                glUniform4f(
                    colorLoc,
                    0.05f,
                    0.25f,
                    0.9f,
                    1.0f
                );
                break;

            case 3:
                glUniform4f(
                    colorLoc,
                    0.8f,
                    0.05f,
                    0.75f,
                    1.0f
                );
                break;
        }

        bolasMesh[i]->draw();
    }

    glUniformMatrix4fv(
        modelLoc,
        1,
        GL_FALSE,
        finalEstrelaModel
    );

    glUniform4f(
        colorLoc,
        1.0f,
        0.75f,
        0.05f,
        1.0f
    );

    estrelaMesh->draw();
}