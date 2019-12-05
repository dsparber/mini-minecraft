#include "biomes.h"
#include "fbm.h"
#include "biometype.h"

float desertNoise(float x, float z)
{
    float height = fbm(x, z);
    float temp = glm::smoothstep(0.4f, 0.8f, height);
    return pow(temp, 3);
}

float wetlandNoise(float x, float z)
{
    float height = fbm(x, z);
    return noise1D(sin(height) + sin(0.5 * height));
}

float grasslandNoise(float x, float z)
{
    float rawFBM = fbm(x, z);
    float height = 15.f * rawFBM;
    return height;
}

float mountainNoise(float x, float z)
{
    float rawFBM = fbm(x, z);
    float height = 120.f * pow(rawFBM, 2.5);
    return height;
}

glm::vec3 mbNoise(float x, float z)
{
    return WorleyNoise(glm::vec2(x, z));
}

BiomeType getCurrBiome(glm::vec2 mb)
{
    float m = mb.x;
    float b = mb.y;

    if (m >= 0.f && m < 0.5) {
        if (b >= 0.f && b < 0.5) {
            return DESERT;
        } else if (b >= 0.5 && b < 1.f) {
            return MOUNTAINS;
        }
    } else if (m >= 0.5 && m < 1.f) {
        if (b >= 0.f && b < 0.5) {
            return WETLAND;
        } else if (b >= 0.5 && b < 1.f) {
            return GRASSLAND;
        }
    }
}

float interpolateBiomes(glm::vec4 biomeHeights, BiomeType currBiome, glm::vec2 currPos)
{
    // BiomeHeights Order: desert, wetland, grassland, mountain

    if (currBiome == DESERT)
    {
        float l = glm::mix(biomeHeights.x, biomeHeights.y, glm::smoothstep(0.45f, 0.55f, glm::distance(currPos, glm::vec2(1.f, 0.f))));
        float m = glm::mix(biomeHeights.x, biomeHeights.z, glm::smoothstep(0.45f, 0.55f, glm::distance(currPos, glm::vec2(1.f, 1.f))));
        float n = glm::mix(biomeHeights.x, biomeHeights.w, glm::smoothstep(0.45f, 0.55f, glm::distance(currPos, glm::vec2(0.f, 1.f))));

        return (l + m + n) / 3.f;
    }
    else if (currBiome == WETLAND)
    {
        float l = glm::mix(biomeHeights.y, biomeHeights.x, glm::smoothstep(0.45f, 0.55f, glm::distance(currPos, glm::vec2(0.f, 0.f))));
        float m = glm::mix(biomeHeights.y, biomeHeights.z, glm::smoothstep(0.45f, 0.55f, glm::distance(currPos, glm::vec2(1.f, 1.f))));
        float n = glm::mix(biomeHeights.y, biomeHeights.w, glm::smoothstep(0.45f, 0.55f, glm::distance(currPos, glm::vec2(0.f, 1.f))));

        return (l + m + n) / 3.f;
    }
    else if (currBiome == GRASSLAND)
    {
        float l = glm::mix(biomeHeights.z, biomeHeights.x, glm::smoothstep(0.45f, 0.55f, glm::distance(currPos, glm::vec2(0.f, 0.f))));
        float m = glm::mix(biomeHeights.z, biomeHeights.y, glm::smoothstep(0.45f, 0.55f, glm::distance(currPos, glm::vec2(1.f, 0.f))));
        float n = glm::mix(biomeHeights.z, biomeHeights.w, glm::smoothstep(0.45f, 0.55f, glm::distance(currPos, glm::vec2(0.f, 1.f))));

        return (l + m + n) / 3.f;
    }
    else
    {
        float l = glm::mix(biomeHeights.w, biomeHeights.x, glm::smoothstep(0.45f, 0.55f, glm::distance(currPos, glm::vec2(0.f, 0.f))));
        float m = glm::mix(biomeHeights.w, biomeHeights.y, glm::smoothstep(0.45f, 0.55f, glm::distance(currPos, glm::vec2(1.f, 0.f))));
        float n = glm::mix(biomeHeights.w, biomeHeights.z, glm::smoothstep(0.45f, 0.55f, glm::distance(currPos, glm::vec2(1.f, 1.f))));

        return (l + m + n) / 3.f;
    }


    // Lecture Interpolation 2
//    float sum = 0.f;

//    for (int i = 0; i < 4; i++) {
//        float weight = -(abs((4 - 1) * x - i)) + 1;
//        sum += biomeHeights[i] * weight;
//    }
//    return sum;

    // Lecture Interpolation 1
//    int curr = currBiome.getNum();
//    float l = glm::mix(biomeHeights[curr], biomeHeights[curr + 1 / ], glm::smoothstep(0.45, 0.55, t));
//    float m = glm::mix(biomeHeights[curr], biomeHeights[curr + 2 / ], glm::smoothstep(0.45, 0.55, t));
//    float n = glm::mix(biomeHeights[curr], biomeHeights[curr + 3 / ], glm::smoothstep(0.45, 0.55, t));



    // Internet Interpolation
}

