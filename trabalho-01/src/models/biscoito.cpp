#include "../../include/models/biscoito.hpp"
#include <cmath>

#define M_PI 3.14159265358979323846

Biscoito::Biscoito() {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int start;

    start = indices.size();

    addCircle(
        vertices,
        indices,
        0.0f,
        0.45f,
        0.2f,
        40
    );

    addRectangle(
        vertices,
        indices,
        0.0f,
        0.02f,
        0.30f,
        0.50f
    );

    float armLength    = 0.3f;
    float armThickness = 0.12f;
    float armRadius    = 0.05f;
    int   armSegments  = 4;

    float rightShoulderX = 0.13f;
    float rightShoulderY = 0.23f;
    float rightAngle     = -0.55f; 
    float leftShoulderX = -0.13f;
    float leftShoulderY =  0.23f;
    float leftAngle     = static_cast<float>(M_PI) - rightAngle; 

    float rightCenterX = rightShoulderX + (armLength * 0.5f) * std::cos(rightAngle);
    float rightCenterY = rightShoulderY + (armLength * 0.5f) * std::sin(rightAngle);

    float leftCenterX = leftShoulderX + (armLength * 0.5f) * std::cos(leftAngle);
    float leftCenterY = leftShoulderY + (armLength * 0.5f) * std::sin(leftAngle);

    addRoundedRectangle(
        vertices,
        indices,
        leftCenterX,
        leftCenterY,
        armLength,
        armThickness,
        armRadius,
        armSegments,
        leftAngle
    );

    addRoundedRectangle(
        vertices,
        indices,
        rightCenterX,
        rightCenterY,
        armLength,
        armThickness,
        armRadius,
        armSegments,
        rightAngle
    );

    addRoundedRectangle(
        vertices,
        indices,
        -0.09f,
        -0.38f,
        0.11f,
        0.45f,
        0.05f,
        4
    );

    addRoundedRectangle(
        vertices,
        indices,
        0.09f,
        -0.38f,
        0.13f,
        0.45f,
        0.05f,
        4
    );

    bodyIndexOffset = start;
    bodyIndexCount = indices.size() - start;

    start = indices.size();

    addCircle(
        vertices,
        indices,
        -0.07f,
        0.48f,
        0.035f,
        12,
        0.01f
    );

    addCircle(
        vertices,
        indices,
        0.07f,
        0.48f,
        0.035f,
        12,
        0.01f
    );

    eyesIndexOffset = start;
    eyesIndexCount = indices.size() - start;

    start = indices.size();

    addCircle(
        vertices,
        indices,
        0.0f,
        0.20f,
        0.025f,
        10,
        0.01f
    );

    addCircle(
        vertices,
        indices,
        0.0f,
        0.10f,
        0.025f,
        10,
        0.01f
    );

    addCircle(
        vertices,
        indices,
        0.0f,
        0.0f,
        0.025f,
        10,
        0.01f
    );

    buttonsIndexOffset = start;
    buttonsIndexCount = indices.size() - start;

    mouthVertexOffset = vertices.size() / 3;

    int steps = 12;

    for (int i = 0; i <= steps; ++i) {
        float t = static_cast<float>(i) / steps;
        float ang = M_PI * 1.15f + t * (M_PI * 0.7f);

        vertices.push_back(
            0.09f * std::cos(ang)
        );

        vertices.push_back(
            0.40f + 0.06f * std::sin(ang)
        );

        vertices.push_back(0.01f);
    }

    mouthVertexCount = steps + 1;

    mesh = new Mesh(vertices, indices);
}

Biscoito::~Biscoito() {
    delete mesh;
}
