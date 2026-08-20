#include "cilinder.hpp"
#include <cmath>
#include <vector>
#define PI M_PI

std::vector<float> Cilinder::generateVertices(unsigned int sectorCount,
                                              float radius, float height) {
    std::vector<float> vertices;
    float x, y, z;
    for (int i = 0; i < sectorCount; i++) {
        float theta = ((2 * PI) * (float)i) / sectorCount;

        x = radius * std::cos(theta);
        y = radius * std::sin(theta);
        z = height / 2;

        // top
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);

        // base
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(-z);
    }

    return vertices;
}

std::vector<unsigned int> Cilinder::generateIndices(unsigned int sectorCount) {
    std::vector<unsigned int> indices;

    for (int i = 0; i < sectorCount; i++) {
        unsigned int next = (i + 1) % sectorCount;

        unsigned int top_i = 2 * i;
        unsigned int base_i = 2 * i + 1;

        unsigned int top_next = 2 * next;
        unsigned int base_next = 2 * next + 1;

        indices.push_back(base_i);
        indices.push_back(base_next);
        indices.push_back(top_i);

        indices.push_back(top_i);
        indices.push_back(base_next);
        indices.push_back(top_next);
    }

    return indices;
}

Cilinder::Cilinder(unsigned int sectorCount, float radius, float height) {
    vertices = generateVertices(sectorCount, radius, height);
    indices = generateIndices(sectorCount);
}