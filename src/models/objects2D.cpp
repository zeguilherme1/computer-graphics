#include "../../include/models/objects2D.hpp"
#include <cmath>

#define M_PI 3.14159265358979323846

void Objects2D::addCircle(
    std::vector<float>& v,
    std::vector<unsigned int>& indices,
    float cx,
    float cy,
    float r,
    int segments,
    float z
) {
    unsigned int center = v.size() / 3;

    v.push_back(cx);
    v.push_back(cy);
    v.push_back(z);

    unsigned int first = v.size() / 3;

    for (int i = 0; i < segments; ++i) {
        float ang = i * 2.0f * M_PI / segments;

        v.push_back(cx + r * std::cos(ang));
        v.push_back(cy + r * std::sin(ang));
        v.push_back(z);
    }

    for (int i = 0; i < segments; ++i) {
        unsigned int current = first + i;
        unsigned int next = first + ((i + 1) % segments);

        indices.push_back(center);
        indices.push_back(current);
        indices.push_back(next);
    }
}

void Objects2D::addRectangle(
    std::vector<float>& v,
    std::vector<unsigned int>& indices,
    float cx,
    float cy,
    float width,
    float height,
    float z
) {
    unsigned int start = v.size() / 3;

    float hw = width / 2.0f;
    float hh = height / 2.0f;

    v.push_back(cx - hw);
    v.push_back(cy + hh);
    v.push_back(z);

    v.push_back(cx + hw);
    v.push_back(cy + hh);
    v.push_back(z);

    v.push_back(cx + hw);
    v.push_back(cy - hh);
    v.push_back(z);

    v.push_back(cx - hw);
    v.push_back(cy - hh);
    v.push_back(z);

    indices.push_back(start + 0);
    indices.push_back(start + 1);
    indices.push_back(start + 2);

    indices.push_back(start + 0);
    indices.push_back(start + 2);
    indices.push_back(start + 3);
}

void Objects2D::addRoundedRectangle(
    std::vector<float>& v,
    std::vector<unsigned int>& indices,
    float cx,
    float cy,
    float width,
    float height,
    float radius,
    int cornerSegments,
    float z
) {
    unsigned int center = v.size() / 3;

    v.push_back(cx);
    v.push_back(cy);
    v.push_back(z);

    float hw = width / 2.0f - radius;
    float hh = height / 2.0f - radius;

    float centers[4][2] = {
        {cx + hw, cy + hh},
        {cx - hw, cy + hh},
        {cx - hw, cy - hh},
        {cx + hw, cy - hh}
    };

    float startAngles[4] = {
        0.0f,
        M_PI * 0.5f,
        M_PI,
        M_PI * 1.5f
    };

    unsigned int borderStart = v.size() / 3;

    for (int corner = 0; corner < 4; ++corner) {
        for (int i = 0; i < cornerSegments; ++i) {
            float t = static_cast<float>(i) / cornerSegments;
            float ang = startAngles[corner] + t * M_PI * 0.5f;

            v.push_back(
                centers[corner][0] +
                radius * std::cos(ang)
            );

            v.push_back(
                centers[corner][1] +
                radius * std::sin(ang)
            );

            v.push_back(z);
        }
    }

    unsigned int points = cornerSegments * 4;

    for (unsigned int i = 0; i < points; ++i) {
        unsigned int current = borderStart + i;
        unsigned int next = borderStart + ((i + 1) % points);

        indices.push_back(center);
        indices.push_back(current);
        indices.push_back(next);
    }
}

void Objects2D::addTriangle(
    std::vector<float>& v,
    std::vector<unsigned int>& indices,
    float cx,
    float cy,
    float width,
    float height,
    float z
) {
    unsigned int start = v.size() / 3;

    float halfWidth = width / 2.0f;

    v.push_back(cx);
    v.push_back(cy + height / 2.0f);
    v.push_back(z);

    v.push_back(cx - halfWidth);
    v.push_back(cy - height / 2.0f);
    v.push_back(z);

    v.push_back(cx + halfWidth);
    v.push_back(cy - height / 2.0f);
    v.push_back(z);

    indices.push_back(start + 0);
    indices.push_back(start + 1);
    indices.push_back(start + 2);
}