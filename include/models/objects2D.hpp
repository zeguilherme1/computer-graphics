#pragma once
#ifndef OBJECTS2D_H
#define OBJECTS2D_H

#include <vector>

class Objects2D {
protected:
    void addCircle(
        std::vector<float>& v,
        std::vector<unsigned int>& indices,
        float cx,
        float cy,
        float r,
        int segments,
        float z = 0.0f
    );

    void addRectangle(
        std::vector<float>& v,
        std::vector<unsigned int>& indices,
        float cx,
        float cy,
        float width,
        float height,
        float z = 0.0f
    );

    void addRoundedRectangle(
        std::vector<float>& v,
        std::vector<unsigned int>& indices,
        float cx,
        float cy,
        float width,
        float height,
        float radius,
        int cornerSegments,
        float angle = 0.0f,
        float z = 0.0f
    );

    void addTriangle(
        std::vector<float>& v,
        std::vector<unsigned int>& indices,
        float cx,
        float cy,
        float width,
        float height,
        float z = 0.0f
    );
};

#endif