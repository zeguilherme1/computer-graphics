#include "../../include/models/sphere.hpp"
#include <cmath>
#include <vector>
#define PI M_PI

std::vector<float> Sphere::generateVertices(float sectorCount, float stackCount,
                                            float radius) {
    std::vector<float> vertices;
    float x, y, z, xy;

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stack_angle;

    for (int i = 0; i <= stackCount; i++) {
        stack_angle = PI / 2 - i * stackStep;
        xy = radius * cosf(stack_angle);
        z = radius * sinf(stack_angle);

        for (int j = 0; j < sectorCount; j++) {
            sectorAngle = j * sectorStep;

            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    return vertices;
}

std::vector<unsigned int> Sphere::generateIndices(float sectorCount,
                                                  float stackCount) {
    int k1, k2;
    std::vector<unsigned int> indices;

    for (int i = 0; i < stackCount; i++) {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; j++, k1++, k2++) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
    return indices;
}

std::vector<unsigned int> Sphere::generateLineIndices(float sectorCount,
                                                      float stackCount) {
    int k1, k2;
    std::vector<unsigned int> lineIndices;

    for (int i = 0; i < stackCount; i++) {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; j++, k1++, k2++) {

            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            if (i != 0) {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }
    return lineIndices;
}

Sphere::Sphere(float sectorCount, float stackCount, float radius) {
    vertices = generateVertices(sectorCount, stackCount, radius);
    indices = generateIndices(sectorCount, stackCount);
    lineIndices = generateLineIndices(sectorCount, stackCount);
}