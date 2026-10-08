#pragma once
#ifndef CLOUD_H
#define CLOUD_H

#include <vector>
#include "./objects2D.hpp"
#include "./mesh.hpp"

// Classe responsável por gerar as nuvems em 2D
class Cloud : public Objects2D {
public:
    Mesh* mesh;

    Cloud();
    ~Cloud();

private:
    std::vector<float> generateCloudVertices();
};

#endif