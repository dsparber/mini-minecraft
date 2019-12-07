#ifndef FBM_H
#define FBM_H
#include "la.h"
#include "vector"

float rand(glm::vec2 n);

float interpNoise2D(float x, float y);

float fbm(float x, float z);

glm::vec2 random2(glm::vec2 p);

float surflet(glm::vec2 p, glm::vec2 gridPoint);

float perlinNoise(glm::vec2 uv);

#endif // FBM_H
