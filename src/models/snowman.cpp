#include "../../include/models/snowman.hpp"
#include "../../include/utils.hpp"
#include "../../include/models/sphere.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

Snowman::Snowman()
    : noseData(16, 0.034f, 0.035f),
      eyeData(12, 8, 0.028f),
      buttonData(12, 8, 0.035f),
      armData(12, 0.025f, 0.35f),
      hatData(32, 0.15f, 0.15f),
      hatTopData(12, 8, 0.025f) {

    std::vector<float> bodyVertices;
    std::vector<unsigned int> bodyIndices;

    addSphereToBody(bodyVertices, bodyIndices, 0.35f, -0.40f, 32, 16);
    addSphereToBody(bodyVertices, bodyIndices, 0.25f,  0.05f, 32, 16);
    addSphereToBody(bodyVertices, bodyIndices, 0.15f,  0.40f, 32, 16);

    bodyMesh   = std::make_unique<Mesh>(bodyVertices, bodyIndices);
    noseMesh   = std::make_unique<Mesh>(noseData.vertices, noseData.indices);
    eyeMesh    = std::make_unique<Mesh>(eyeData.vertices, eyeData.indices);
    buttonMesh = std::make_unique<Mesh>(buttonData.vertices, buttonData.indices);
    armMesh    = std::make_unique<Mesh>(armData.vertices, armData.indices);
    hatMesh    = std::make_unique<Mesh>(hatData.vertices, hatData.indices);
    hatTopMesh = std::make_unique<Mesh>(hatTopData.vertices, hatTopData.indices);
}

void Snowman::addSphereToBody(std::vector<float>& vertices,
    std::vector<unsigned int>& indices,
    float radius, float offsetY,
    unsigned int sectorCount, unsigned int stackCount) {

    unsigned int indexOffset = vertices.size() / 3;
    Sphere temp_sphere(sectorCount, stackCount, radius);

    for (size_t i = 0; i < temp_sphere.vertices.size(); i += 3) {
        vertices.push_back(temp_sphere.vertices[i]);
        vertices.push_back(temp_sphere.vertices[i + 1] + offsetY);
        vertices.push_back(temp_sphere.vertices[i + 2]);
    }
    for (unsigned int index : temp_sphere.indices) {
        indices.push_back(index + indexOffset);
    }
}

void Snowman::draw(unsigned int shaderProgram, float* baseModelMatrix, int modelLoc, int colorLoc) {
    glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, baseModelMatrix);
    bodyMesh->draw();

    float noseTrans[16], finalNoseModel[16];
    generateTranslationMatrix(0.0f, 0.40f, 0.17f, noseTrans);
    multMatrix(baseModelMatrix, noseTrans, finalNoseModel);

    glUniform4f(colorLoc, 1.0f, 0.5f, 0.0f, 1.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, finalNoseModel);
    noseMesh->draw();

    float eyeOffsets[2][3] = {
        {-0.065f, 0.44f, 0.125f},
        { 0.065f, 0.44f, 0.125f}
    };

    glUniform4f(colorLoc, 0.05f, 0.05f, 0.05f, 1.0f);
    for (int i = 0; i < 2; ++i) {
        float eyeTrans[16], finalEyeModel[16];
        generateTranslationMatrix(eyeOffsets[i][0], eyeOffsets[i][1], eyeOffsets[i][2], eyeTrans);
        multMatrix(baseModelMatrix, eyeTrans, finalEyeModel);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, finalEyeModel);
        eyeMesh->draw();
    }

    float buttonOffsets[2][3] = {
        {0.0f, 0.16f, 0.225f},
        {0.0f, 0.00f, 0.245f}
    };

    glUniform4f(colorLoc, 0.1f, 0.1f, 0.1f, 1.0f);
    for (int i = 0; i < 2; ++i) {
        float buttonTrans[16], finalButtonModel[16];
        generateTranslationMatrix(buttonOffsets[i][0], buttonOffsets[i][1], buttonOffsets[i][2], buttonTrans);
        multMatrix(baseModelMatrix, buttonTrans, finalButtonModel);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, finalButtonModel);
        buttonMesh->draw();
    }

    float armRotR[16], armRotL[16];
    generateRotationYMatrix( 1.5708f, armRotR);
    generateRotationYMatrix(-1.5708f, armRotL);

    float armTransR[16], armTransL[16];
    generateTranslationMatrix( 0.24f, 0.10f, 0.0f, armTransR);
    generateTranslationMatrix(-0.24f, 0.10f, 0.0f, armTransL);

    float armLocalR[16], armLocalL[16];
    multMatrix(armTransR, armRotR, armLocalR);
    multMatrix(armTransL, armRotL, armLocalL);

    float finalArmModelR[16], finalArmModelL[16];
    multMatrix(baseModelMatrix, armLocalR, finalArmModelR);
    multMatrix(baseModelMatrix, armLocalL, finalArmModelL);

    glUniform4f(colorLoc, 0.36f, 0.16f, 0.05f, 1.0f);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, finalArmModelR);
    armMesh->draw();

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, finalArmModelL);
    armMesh->draw();

    float hatRot[16], hatTrans[16], hatLocal[16], finalHatModel[16];
    generateRotationXMatrix(-1.5708f, hatRot);
    generateTranslationMatrix(0.0f, 0.55f, 0.0f, hatTrans);
    multMatrix(hatTrans, hatRot, hatLocal);
    multMatrix(baseModelMatrix, hatLocal, finalHatModel);

    glUniform4f(colorLoc, 0.6f, 0.05f, 0.1f, 1.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, finalHatModel);
    hatMesh->draw();

    float hatTopTrans[16], finalHatTopModel[16];
    generateTranslationMatrix(0.0f, 0.65f, 0.0f, hatTopTrans);
    multMatrix(baseModelMatrix, hatTopTrans, finalHatTopModel);

    glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, finalHatTopModel);
    hatTopMesh->draw();
}
