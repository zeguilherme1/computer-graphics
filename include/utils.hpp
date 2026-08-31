#pragma once
#ifndef UTILS_H
#define UTILS_H

void multMatrix(const float a[16], const float b[16], float result[16]);

void generateTranslationMatrix(float tx, float ty, float tz, float result[16]);
void generateScaleMatrix(float sx, float sy, float sz, float result[16]);
void generateRotationXMatrix(float angle, float result[16]);
void generateRotationYMatrix(float angle, float result[16]);
void generateIdentityMatrix(float result[16]);

void checkCompilationStatus(unsigned int vertex_shader);

unsigned int initShaders();

#endif