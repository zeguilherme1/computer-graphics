#include "../../include/models/cloud.hpp"

Cloud::Cloud() {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    addCircle(
        vertices,
        indices,
        -0.25f,
        -0.12f,
        0.15f,
        40
    );

    addCircle(
        vertices,
        indices,
        0.0f,
        0.0f,
        0.25f,
        40
    );

    addCircle(
        vertices,
        indices,
        0.25f,
        -0.12f,
        0.15f,
        40
    );

    mesh = new Mesh(vertices, indices);
}

Cloud::~Cloud() {
    delete mesh;
}