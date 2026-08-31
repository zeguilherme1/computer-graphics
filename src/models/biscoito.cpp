#include "../../include/models/biscoito.hpp"
#include <cmath>

#define M_PI 3.14159265358979323846

void Biscoito::addCircle(std::vector<float>& v, float cx, float cy, float r, int segments, float z) {
    for (int i = 0; i < segments; ++i) {
        float ang = i * 2.0f * M_PI / segments;
        v.push_back(cx + r * std::cos(ang));
        v.push_back(cy + r * std::sin(ang));
        v.push_back(z);
    }
}

void Biscoito::addRectangle(std::vector<float>& v, float cx, float cy, float width, float height, float z) {
    float hw = width / 2.0f;
    float hh = height / 2.0f;

    v.push_back(cx - hw); v.push_back(cy + hh); v.push_back(z);
    v.push_back(cx + hw); v.push_back(cy + hh); v.push_back(z);
    v.push_back(cx + hw); v.push_back(cy - hh); v.push_back(z);
    v.push_back(cx - hw); v.push_back(cy - hh); v.push_back(z);
}

void Biscoito::addRoundedRectangle(std::vector<float>& v, float cx, float cy, float width, float height, float radius, int cornerSegments, float z) {
    float hw = width / 2.0f - radius;
    float hh = height / 2.0f - radius;

    float centers[4][2] = {
        {cx + hw, cy + hh},
        {cx - hw, cy + hh},
        {cx - hw, cy - hh},
        {cx + hw, cy - hh}
    };

    float startAngles[4] = { 0.0f, M_PI * 0.5f, M_PI, M_PI * 1.5f };

    for (int corner = 0; corner < 4; ++corner) {
        for (int i = 0; i <= cornerSegments; ++i) {
            float t = (float)i / cornerSegments;
            float ang = startAngles[corner] + t * (M_PI * 0.5f);
            v.push_back(centers[corner][0] + radius * std::cos(ang));
            v.push_back(centers[corner][1] + radius * std::sin(ang));
            v.push_back(z);
        }
    }
}

std::vector<float> Biscoito::generateBodyVertices() {
    std::vector<float> v;
    bodyPartCounts.clear();

    auto markPart = [&](size_t startSize) {
        int count = (v.size() - startSize) / 3;
        bodyPartCounts.push_back(count);
    };

    // Head
    size_t s = v.size();
    addCircle(v, 0.0f, 0.45f, 0.2f, 40);
    markPart(s);

    // Body
    s = v.size();
    addRectangle(v, 0.0f, 0.02f, 0.30f, 0.50f);
    markPart(s);

    // Left Arm
    s = v.size();
    addRoundedRectangle(v, -0.22f, 0.15f, 0.25f, 0.12f, 0.05f, 4);
    markPart(s);

    // Right Arm
    s = v.size();
    addRoundedRectangle(v, 0.22f, 0.15f, 0.25f, 0.12f, 0.05f, 4);
    markPart(s);

    // Left Leg
    s = v.size();
    addRoundedRectangle(v, -0.09f, -0.38f, 0.11f, 0.45f, 0.05f, 4);
    markPart(s);

    // RIght Leg
    s = v.size();
    addRoundedRectangle(v, 0.09f, -0.38f, 0.11f, 0.45f, 0.05f, 4);
    markPart(s);

    return v;
}

std::vector<float> Biscoito::generateEyesVertices() {
    std::vector<float> v;
    addCircle(v, -0.07f, 0.48f, 0.035f, 12, 0.01f);
    addCircle(v,  0.07f, 0.48f, 0.035f, 12, 0.01f);
    return v;
}

std::vector<float> Biscoito::generateButtonsVertices() {
    std::vector<float> v;
    addCircle(v, 0.0f,  0.20f, 0.025f, 10, 0.01f);
    addCircle(v, 0.0f,  0.10f, 0.025f, 10, 0.01f);
    addCircle(v, 0.0f, -0.00f, 0.025f, 10, 0.01f);
    return v;
}

std::vector<float> Biscoito::generateMouthVertices() {
    std::vector<float> v;
    int steps = 12;
    for (int i = 0; i <= steps; ++i) {
        float t = (float)i / steps;
        float ang = M_PI * 1.15f + t * (M_PI * 0.7f);
        v.push_back(0.09f * std::cos(ang));
        v.push_back(0.40f + 0.06f * std::sin(ang));
        v.push_back(0.01f);
    }
    return v;
}

std::vector<float> Biscoito::generateBiscuitVertices() {
    std::vector<float> allVertices;
    allVertices.insert(allVertices.end(), bodyVertices.begin(), bodyVertices.end());
    allVertices.insert(allVertices.end(), eyesVertices.begin(), eyesVertices.end());
    allVertices.insert(allVertices.end(), buttonsVertices.begin(), buttonsVertices.end());
    allVertices.insert(allVertices.end(), mouthVertices.begin(), mouthVertices.end());
    return allVertices;
}

Biscoito::Biscoito() {
    bodyVertices    = generateBodyVertices();
    eyesVertices    = generateEyesVertices();
    buttonsVertices = generateButtonsVertices();
    mouthVertices   = generateMouthVertices();
    biscuitVertices = generateBiscuitVertices();

    bodyCount    = bodyVertices.size() / 3;
    eyesCount    = eyesVertices.size() / 3;
    buttonsCount = buttonsVertices.size() / 3;
    mouthCount   = mouthVertices.size() / 3;

    bodyOffset    = 0;
    eyesOffset    = bodyOffset + bodyCount;
    buttonsOffset = eyesOffset + eyesCount;
    mouthOffset   = buttonsOffset + buttonsCount;
}