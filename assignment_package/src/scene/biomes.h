#ifndef BIOMES_H
#define BIOMES_H
#include "la.h"
#include "biometype.h"

float desertNoise(float x, float z);

float wetlandNoise(float x, float z);

float grasslandNoise(float x, float z);

float mountainNoise(float x, float z);

float moistNoise(float x, float z);

float bumpNoise(float x, float z);

BiomeType getCurrBiome(glm::vec2 mb);

float interpolateBiomes(glm::vec4 biomeHeights, BiomeType currBiome, glm::vec2 currPos);

#endif // BIOMES_H
