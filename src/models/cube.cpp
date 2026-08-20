#include "cube.hpp"
#include <vector>

std::vector<float> Cube::generateVertices() {
    std::vector<float> vertices = {-0.2f, -0.2f, 0.2f,  0.2f,  -0.2f, 0.2f,
                                   -0.2f, 0.2f,  0.2f,  0.2f,  0.2f,  0.2f,

                                   0.2f,  -0.2f, 0.2f,  0.2f,  -0.2f, -0.2f,
                                   0.2f,  0.2f,  0.2f,  0.2f,  0.2f,  -0.2f,

                                   0.2f,  -0.2f, -0.2f, -0.2f, -0.2f, -0.2f,
                                   0.2f,  0.2f,  -0.2f, -0.2f, 0.2f,  -0.2f,

                                   -0.2f, -0.2f, -0.2f, -0.2f, -0.2f, 0.2f,
                                   -0.2f, 0.2f,  -0.2f, -0.2f, 0.2f,  0.2f,

                                   -0.2f, -0.2f, -0.2f, 0.2f,  -0.2f, -0.2f,
                                   -0.2f, -0.2f, 0.2f,  0.2f,  -0.2f, 0.2f,

                                   -0.2f, 0.2f,  0.2f,  0.2f,  0.2f,  0.2f,
                                   -0.2f, 0.2f,  -0.2f, 0.2f,  0.2f,  -0.2f};

    return vertices;
}

Cube::Cube() { vertices = generateVertices(); }