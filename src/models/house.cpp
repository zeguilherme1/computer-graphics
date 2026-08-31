#include "../../include/models/house.hpp"

House::House() {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int start;

    start = indices.size();

    addRectangle(
        vertices,
        indices,
        0.0f,
        0.0f,
        0.3f,
        0.2f
    );

    wallIndexOffset = start;
    wallIndexCount = indices.size() - start;

    start = indices.size();

    addTriangle(
        vertices,
        indices,
        0.0f,
        0.15f,
        0.3f,
        0.1f
    );

    roofIndexOffset = start;
    roofIndexCount = indices.size() - start;

    start = indices.size();

    addRectangle(
        vertices,
        indices,
        -0.075f,
        0.025f,
        0.075f,
        0.06f
    );

    windowIndexOffset = start;
    windowIndexCount = indices.size() - start;

    start = indices.size();

    addRectangle(
        vertices,
        indices,
        0.075f,
        -0.035f,
        0.08f,
        0.13f
    );

    doorIndexOffset = start;
    doorIndexCount = indices.size() - start;

    start = indices.size();

    addCircle(
        vertices,
        indices,
        0.09f,
        -0.035f,
        0.01f,
        10
    );

    handleIndexOffset = start;
    handleIndexCount = indices.size() - start;

    mesh = new Mesh(vertices, indices);
}

House::~House() {
    delete mesh;
}