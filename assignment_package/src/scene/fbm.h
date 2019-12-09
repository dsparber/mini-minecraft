#ifndef FBM_H
#define FBM_H
#include "la.h"
#include "vector"

float rand(glm::vec2 n);

float interpNoise2D(float x, float y);

float fbm(float x, float z);

glm::vec2 random2(glm::vec2 p);

#endif // FBM_H
