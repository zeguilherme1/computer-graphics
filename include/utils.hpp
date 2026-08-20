#pragma once
#ifndef UTILS_H
#define UTILS_H

void multMatrix(const float a[16], const float b[16], float result[16]);

void generateTranslationMatrix(
    float tx,
    float ty,
    float tz,
    float result[16]
);

void checkCompilationStatus(unsigned int vertex_shader);

unsigned int createVBO(float vertices[], size_t size);

unsigned int createVAO(unsigned int VBO);

unsigned int initShaders();

#endif