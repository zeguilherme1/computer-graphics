#pragma once
#ifndef SNOWMAN_H
#define SNOWMAN_H
#include "cone.hpp"
#include "cilinder.hpp"
#include "mesh.hpp"
#include <memory>

class Snowman {
    public:
        Snowman();
        void draw(unsigned int shaderProgram, float* baseModelMatrix, int modelLoc, int colorLoc);
    private:
    std::unique_ptr<Mesh> bodyMesh;
    std::unique_ptr<Mesh> noseMesh;
    Cone noseData;

    void addSphereToBody(std::vector<float>& vertices, 
                         std::vector<unsigned int>& indices, 
                         float radius, float offsetY, 
                         unsigned int sectorCount, unsigned int stackCount);

};


#endif