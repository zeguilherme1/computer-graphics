#pragma once
#ifndef TREE_H
#define TREE_H
#include "cone.hpp"
#include "cilinder.hpp"
#include "mesh.hpp"
#include <memory>

class Tree {
  public:
    Tree();
    void draw(unsigned int shaderProgram, float* baseModelMatrix, int modelLoc);
  private:
    Cilinder troncoData;
    Cone folhasData;
    std::unique_ptr<Mesh> troncoMesh;
    std::unique_ptr<Mesh> folhasMesh;
};

#endif