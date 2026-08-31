#pragma once
#ifndef HOUSE_H
#define HOUSE_H

#include <vector>
#include "./objects2D.hpp"
#include "./mesh.hpp"

class House : public Objects2D {
public:
    std::vector<float> wallVertices;
    std::vector<float> roofVertices;
    std::vector<float> windowVertices;
    std::vector<float> doorVertices;
    std::vector<float> handleVertices;

    std::vector<unsigned int> houseIndices;

    int wallIndexCount;
    int roofIndexCount;
    int windowIndexCount;
    int doorIndexCount;
    int handleIndexCount;

    int wallIndexOffset;
    int roofIndexOffset;
    int windowIndexOffset;
    int doorIndexOffset;
    int handleIndexOffset;

    Mesh* mesh;

    House();
    ~House();

private:
    std::vector<float> generateHouseVertices();
};

#endif