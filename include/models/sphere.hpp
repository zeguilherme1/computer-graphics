#pragma once
#ifndef SPHERE_H
#define SPHERE_H
#include <cmath>
#include <vector>
#define PI M_PI

std::vector<float> generateSphere();
std::vector<unsigned int> generateSphereIndices();
std::vector<unsigned int> generateSphereLines();

class Sphere {
  public:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> lineIndices;
    Sphere(float sectorCount, float stackCount, float radius);

  private:
    std::vector<float> generateVertices(float sectorCount, float stackCount,
                                        float radius);

    std::vector<unsigned int> generateIndices(float sectorCount,
                                              float stackCount);

    std::vector<unsigned int> generateLineIndices(float sectorCount,
                                                  float stackCount);
};

#endif
