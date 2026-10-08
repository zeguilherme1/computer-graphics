#include "../../include/models/snowFloor.hpp"

SnowFloor::SnowFloor() {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    addCircle(
        vertices,
        indices,
        0.0f,
        -0.7f,
        1.0f,
        90
    );

    mesh = new Mesh(vertices, indices);
}

SnowFloor::~SnowFloor() {
    delete mesh;
}