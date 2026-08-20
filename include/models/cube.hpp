#pragma once
#ifndef CUBE_H
#define CUBE_H
#include <vector>

class Cube {
  public:
    std::vector<float> vertices;

  private:
    std::vector<float> generateVertices();
};

#endif