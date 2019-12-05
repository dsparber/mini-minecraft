#ifndef FBM_H
#define FBM_H
#include "la.h"

float rand(glm::vec2 n);

float interpNoise2D(float x, float y);

float fbm(float x, float z);

float noise1D(int x);

glm::vec2 random2(glm::vec2 p);

glm::vec3 WorleyNoise(glm::vec2 uv);

#endif // FBM_H
