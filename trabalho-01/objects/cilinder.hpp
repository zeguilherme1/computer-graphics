#pragma once
#ifndef CILINDER_H
#define CILINDER_H
#include <vector>

/*
Classe base do cilindro conta com métodos para gerar os vertices e indíces
*/
class Cilinder {
  public:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    Cilinder(unsigned int sectorCount, float radius, float height);

  private:
    std::vector<float> generateVertices(unsigned int sectorCount,
                                                  float radius, float height);

    std::vector<unsigned int> generateIndices(unsigned int sectorCount);
};

#endif