#include <vector>
#include <cmath>
#include "cone.hpp"
#define PI M_PI

// x = r cos theta
// y = r sin theta
// z = -(h * 0.5)
// theta = 2pi * i / sectorCount

std::vector<float> Cone::generateVertices(unsigned int sectorCount, float radius, float height) {
    float x, y, z;

    std::vector<float> vertices;

    for (int i = 0; i <= sectorCount; i++) {
        float theta = (2 * PI) * float(i) / sectorCount;
        x = radius * std::cos(theta);
        y = radius * std::sin(theta);
        z = (-height * 0.5);

        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
    }
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(height * 0.5f);

    return vertices;
};

std::vector<unsigned int> Cone::generateIndices(unsigned int sectorCount) {
    std::vector<unsigned int> indices;

    for (int i = 0; i <= sectorCount; i++) {
        unsigned int nextIndice = (i + 1) % sectorCount;
        indices.push_back(i);
        indices.push_back(nextIndice);
        indices.push_back(sectorCount);
    }

    return indices;
}

Cone::Cone(unsigned int sectorCount, float radius, float height) {
    vertices = generateVertices(sectorCount, radius, height);
    indices = generateIndices(sectorCount);
}