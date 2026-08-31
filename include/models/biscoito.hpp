#pragma once
#ifndef BISCOITO_H
#define BISCOITO_H

#include "./objects2D.hpp"
#include "./mesh.hpp"

#include <vector>

class Biscoito : public Objects2D {
public:
    int bodyIndexOffset;
    int bodyIndexCount;

    int eyesIndexOffset;
    int eyesIndexCount;

    int buttonsIndexOffset;
    int buttonsIndexCount;

    int mouthVertexOffset;
    int mouthVertexCount;

    Mesh* mesh;

    Biscoito();
    ~Biscoito();
};

#endif