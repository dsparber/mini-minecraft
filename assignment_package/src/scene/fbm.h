#ifndef FBM_H
#define FBM_H
#include "la.h"

float rand(glm::vec2 n);

float interpNoise2D(float x, float y);

float fbm(float x, float z);

#endif // FBM_H
