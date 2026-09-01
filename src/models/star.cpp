#include "../../include/models/star.hpp"
#include <cmath>

#define M_PI 3.14159265358979323846

Star::Star(float outerRadius, float innerRadius) {
    generate(outerRadius, innerRadius);
}

void Star::generate(float outerRadius, float innerRadius) {
    const int points = 10;

    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    for (int i = 0; i < points; ++i) {
        float angle = M_PI / 2.0f + i * 2.0f * M_PI / points;
        float radius = (i % 2 == 0) ? outerRadius : innerRadius;

        vertices.push_back(radius * std::cos(angle));
        vertices.push_back(radius * std::sin(angle));
        vertices.push_back(0.0f);
    }

    for (int i = 0; i < points; ++i) {
        int next = (i + 1) % points;

        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back(next + 1);
    }
}