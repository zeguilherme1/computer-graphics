#pragma once
#ifndef CUBE_H
#define CUBE_H
#include <vector>

class Cube {
  public:
    std::vector<float> vertices;
    Cube();

  private:
    std::vector<float> generateVertices();
};

#endif