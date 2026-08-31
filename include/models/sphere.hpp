#pragma once
#ifndef SPHERE_H
#define SPHERE_H
#include <vector>

std::vector<float> generateSphere();
std::vector<unsigned int> generateSphereIndices();
std::vector<unsigned int> generateSphereLines();

class Sphere {
  public:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> lineIndices;
    Sphere(unsigned int sectorCount, unsigned int stackCount, float radius);

  private:
    std::vector<float> generateVertices(unsigned int sectorCount, unsigned int stackCount,
                                        float radius);

    std::vector<unsigned int> generateIndices(unsigned int sectorCount,
                                              unsigned int stackCount);

    std::vector<unsigned int> generateLineIndices(unsigned int sectorCount,
                                                  unsigned int stackCount);
};

#endif
