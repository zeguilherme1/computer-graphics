#pragma once
#ifndef SNOWFLOOR_H
#define SNOWFLOOR_H

#include <vector>
#include "./objects2D.hpp"
#include "./mesh.hpp"

// Classe responsável por gerar o chão de neve
class SnowFloor : public Objects2D {
public:
    Mesh* mesh;

    SnowFloor();
    ~SnowFloor();

private:
    std::vector<float> generateSnowFloorVertices();
};

#endif