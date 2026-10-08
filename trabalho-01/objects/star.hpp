#pragma once
#ifndef STAR_H
#define STAR_H

#include <vector>

// Classe responsável por gerar a estrela em 2D
class Star {
public:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    Star(float outerRadius, float innerRadius);

private:
    void generate(float outerRadius, float innerRadius);
};

#endif