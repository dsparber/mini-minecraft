#ifndef BIOMES_H
#define BIOMES_H
#include "la.h"
#include "biometype.h"

/// Noise function for forest biome
float forestNoise(float x, float z);

/// Noise function for mordor biome
float mordorNoise(float x, float z);

/// Noise function for shire biome
float shireNoise(float x, float z);

/// Noise function for mountain biome
float snowyMtsNoise(float x, float z);

/// Returns the biome the player is currently in
BiomeType getCurrBiome(glm::vec2 mb);

/// Returns the integer identifier for the current biome
int currBiomeNumb(BiomeType b);

/// Interpolates the height values between the four biomes
float interpolateBiomes(glm::vec4 biomeHeights, glm::vec2 mb);

#endif // BIOMES_H
