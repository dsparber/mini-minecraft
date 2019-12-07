#include "biomes.h"
#include "fbm.h"
#include "biometype.h"

float forestNoise(float x, float z)
{
    float rawFBM = fbm(x, z);
    float height = 15.f * rawFBM;
    return height;
}

float mordorNoise(float x, float z)
{
    float rawFBM = fbm(x, z);
    float height = 50.f * pow(rawFBM, 5);
    return height;
}

float shireNoise(float x, float z)
{
    float rawFBM = fbm(x, z);
    float height = 20.f * pow(rawFBM, 4);
    return height;
}

float snowyMtsNoise(float x, float z)
{
    float rawFBM = fbm(x, z);
    float height = 120.f * pow(rawFBM, 3);
    return height;
}

BiomeType getCurrBiome(glm::vec2 mb)
{
    float m = mb.x;
    float b = mb.y;

    if (m >= 0.f && m < 0.5f) {
        if (b >= 0.f && b < 0.5f) {
            return SHIRE;
        } else if (b >= 0.5f && b < 1.f) {
            return MORDOR;
        }
    } else if (m >= 0.5f && m < 1.f) {
        if (b >= 0.f && b < 0.5f) {
            return FOREST;
        } else if (b >= 0.5f && b < 1.f) {
            return SNOWYMTS;
        }
    }
}

float interpolateBiomes(glm::vec4 biomeHeights, glm::vec2 mb)
{
    float bilerp1 = glm::mix(biomeHeights.x, biomeHeights.w, mb.y);
    float bilerp2 = glm::mix(biomeHeights.z, biomeHeights.y, mb.y);
    return glm::mix(bilerp1, bilerp2, mb.x);
}

void createHobbitHole(int x, int z)
{
    for (int i = x - 5; i < x + 5; i++) {
        for (int j = z - 5; j < z + 5; j++) {
            for (int y = 128; y < 8; y++) {
                //setBlockAt(i, y, z, GRASS);
            }
        }
    }
}
