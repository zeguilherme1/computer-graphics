#pragma once
#ifndef TREE_H
#define TREE_H

#include "cone.hpp"
#include "cilinder.hpp"
#include "mesh.hpp"
#include "sphere.hpp"
#include "star.hpp"

#include <memory>
#include <vector>

class Tree {
  public:
    Tree();
    void draw(unsigned int shaderProgram, float* baseModelMatrix, int modelLoc, int colorLoc);

  private:
    Cilinder troncoData;
    Cone folhasData;
    Star estrelaData;

    std::unique_ptr<Mesh> troncoMesh;
    std::unique_ptr<Mesh> folhasMesh;
    std::unique_ptr<Mesh> estrelaMesh;

    std::vector<std::unique_ptr<Mesh>> bolasMesh;
    std::vector<std::vector<float>> bolasPositions;
};

#endif