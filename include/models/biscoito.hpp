#pragma once
#ifndef BISCOITO_H
#define BISCOITO_H

#include <vector>

class Biscoito {
  public:
    std::vector<float> bodyVertices;
    std::vector<float> eyesVertices;
    std::vector<float> buttonsVertices;
    std::vector<float> mouthVertices;
    std::vector<float> biscuitVertices;

    std::vector<int> bodyPartCounts;
    
    int bodyCount;
    int eyesCount;
    int buttonsCount;
    int mouthCount;

    int bodyOffset;
    int eyesOffset;
    int buttonsOffset;
    int mouthOffset;

    Biscoito();

  private:
    void addCircle(std::vector<float>& v, float cx, float cy, float r, int segments, float z = 0.0f);
    void addRectangle(std::vector<float>& v, float cx, float cy, float width, float height, float z = 0.0f);
    void addRoundedRectangle(std::vector<float>& v, float cx, float cy, float width, float height, float radius, int cornerSegments, float z = 0.0f);

    std::vector<float> generateBodyVertices();
    std::vector<float> generateEyesVertices();
    std::vector<float> generateButtonsVertices();
    std::vector<float> generateMouthVertices();
    std::vector<float> generateBiscuitVertices();
};

#endif