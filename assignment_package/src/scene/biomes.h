#ifndef BIOMES_H
#define BIOMES_H
#include "la.h"
#include "biometype.h"

float forestNoise(float x, float z);

float mordorNoise(float x, float z);

float shireNoise(float x, float z);

float snowyMtsNoise(float x, float z);

BiomeType getCurrBiome(glm::vec2 mb);

float interpolateBiomes(glm::vec4 biomeHeights, glm::vec2 mb);

#endif // BIOMES_H
