#pragma once
#ifndef CONE_H
#define CONE_H
#include <vector>

class Cone {
  public:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    Cone(unsigned int sectorCount, float height, float radius);

  private:
    std::vector<float> generateVertices(unsigned int sectorCount, float height, float radius);
    std::vector<unsigned int> generateIndices(unsigned int sectorCount);
};

#endif