#pragma once
#include <vector>

std::vector<float> generateCube() {
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