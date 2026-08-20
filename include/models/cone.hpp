#pragma once
#ifndef CONE_H
#include <vector>

class Cone {
    public:
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        Cone::Cone(unsigned int sectorCount, float radius, float height);
    private:
        std::vector<float> generateVertices(unsigned int sectorCount, float radius, float height);
        std::vector<unsigned int> generateIndices(unsigned int sectorCount);
        
};

#endif