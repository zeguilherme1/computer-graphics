#pragma once
#ifndef SNOWMAN_H
#define SNOWMAN_H
#include "cone.hpp"
#include "cilinder.hpp"
#include "sphere.hpp"
#include "mesh.hpp"
#include <memory>

class Snowman {
public:
    Snowman();
    void draw(unsigned int shaderProgram, float* baseModelMatrix, int modelLoc, int colorLoc);

private:
    Cone noseData;
    Sphere eyeData;
    Sphere buttonData;
    Cilinder armData;
    Cone hatData;
    Sphere hatTopData;

    std::unique_ptr<Mesh> bodyMesh;
    std::unique_ptr<Mesh> noseMesh;
    std::unique_ptr<Mesh> eyeMesh;
    std::unique_ptr<Mesh> buttonMesh;
    std::unique_ptr<Mesh> armMesh;
    std::unique_ptr<Mesh> hatMesh;
    std::unique_ptr<Mesh> hatTopMesh;

    void addSphereToBody(std::vector<float>& vertices,
        std::vector<unsigned int>& indices,
        float radius, float offsetY,
        unsigned int sectorCount, unsigned int stackCount);
};

#endif
