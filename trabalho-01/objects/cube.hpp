#pragma once
#ifndef CUBE_H
#define CUBE_H
#include <vector>

// Classe responsável por gerar o cubo em 3D
class Cube {
  public:
    std::vector<float> vertices;
    Cube();

  private:
    std::vector<float> generateVertices();
};

#endif