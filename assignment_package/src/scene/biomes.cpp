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
            return SNOWYMTS;
        }
    } else if (m >= 0.5f && m < 1.f) {
        if (b >= 0.f && b < 0.5f) {
            return FOREST;
        } else if (b >= 0.5f && b < 1.f) {
            return MORDOR;
        }
    }
}

int currBiomeNumb(BiomeType b) {
    if (b == FOREST) {
        return 0;
    } else if (b == MORDOR) {
        return 1;
    } else if (b == SHIRE) {
        return 2;
    } else {
        return 3;
    }
}

float interpolateBiomes(glm::vec4 biomeHeights, glm::vec2 mb)
{
    float bilerp1 = glm::mix(biomeHeights.x, biomeHeights.w, glm::smoothstep(0.45f, 0.55f, mb.y));
    float bilerp2 = glm::mix(biomeHeights.z, biomeHeights.y, glm::smoothstep(0.45f, 0.55f, mb.y));
    return glm::mix(bilerp1, bilerp2, glm::smoothstep(0.45f, 0.55f, mb.x));
}
