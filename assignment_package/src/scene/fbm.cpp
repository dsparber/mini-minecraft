#include "fbm.h"


float rand(glm::vec2 n) {
    return (glm::fract(sin(glm::dot(n, glm::vec2(12.9898, 4.1414))) * 43758.5453));
}

float interpNoise2D(float x, float y) {
    float intX = glm::floor(x);
    float fractX = glm::fract(x);
    float intY = floor(y);
    float fractY = glm::fract(y);

    float v1 = rand(glm::vec2(intX, intY));
    float v2 = rand(glm::vec2(intX + 1, intY));
    float v3 = rand(glm::vec2(intX, intY + 1));
    float v4 = rand(glm::vec2(intX + 1, intY + 1));

    float i1 = glm::mix(v1, v2, fractX);
    float i2 = glm::mix(v3, v4, fractX);
    return glm::mix(i1, i2, fractY);
}

float fbm(float x, float z) {
    float total = 0;
    float persistence = 0.5f;
    int octaves = 8;

    for(int i = 1; i <= octaves; i++) {
        float freq = pow(2.f, i);
        float amp = pow(persistence, i);

        total += interpNoise2D(x * freq,
                               z * freq) * amp;
    }
    return total;
}

float noise1D(int x) {
    x = pow((x << 13), x);
    //return (1.0 - (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 10737741824.0;
}

glm::vec2 random2(glm::vec2 p) {
    glm::vec2 a = glm::sin(glm::vec2(glm::dot(p, glm::vec2(127.1,311.7)), glm::dot(p, glm::vec2(269.5,183.3))));
    a.x *= 43758.5453;
    a.y *= 43758.5453;
    return glm::fract(a);
}


float WorleyNoise(glm::vec2 uv) {
    //uv *= 2.0; // Now the space is 10x10 instead of 1x1. Change this to any number you want.
    glm::vec2 uvInt = glm::floor(uv);
    glm::vec2 uvFract = glm::fract(uv);
    float minDist = 1.0; // Minimum distance initialized to max.
    for(int y = -1; y <= 1; ++y) {
        for(int x = -1; x <= 1; ++x) {
            glm::vec2 neighbor = glm::vec2(float(x), float(y)); // Direction in which neighbor cell lies
            glm::vec2 point = random2(uvInt + neighbor); // Get the Voronoi centerpoint for the neighboring cell
            glm::vec2 diff = neighbor + point - uvFract; // Distance between fragment coord and neighbor’s Voronoi point
            float dist = glm::length(diff);
            minDist = glm::min(minDist, dist);
        }
    }
    return minDist;
}

//float WorleyNoise(glm::vec2 uv)
//{
//    // Tile the space
//    glm::vec2 uvInt = glm::floor(uv);
//    glm::vec2 uvFract = glm::fract(uv);

//    float minDist = 1.0; // Minimum distance initialized to max.

//    // Search all neighboring cells and this cell for their point
//    for(int y = -1; y <= 1; y++)
//    {
//        for(int x = -1; x <= 1; x++)
//        {
//            glm::vec2 neighbor = glm::vec2(float(x), float(y));

//            // Random point inside current neighboring cell
//            glm::vec2 point = random2(uvInt + neighbor);

//            // Compute the distance b/t the point and the fragment
//            // Store the min dist thus far
//            glm::vec2 diff = neighbor + point - uvFract;
//            float dist = glm::length(diff);
//            minDist = glm::min(minDist, dist);
//        }
//    }
//    return minDist;
//}

float worleyFBM(glm::vec2 uv) {
    float sum = 0;
    float freq = 4;
    float amp = 0.5;
    for(int i = 0; i < 8; i++) {
        sum += WorleyNoise(uv * freq) * amp;
        freq *= 2;
        amp *= 0.5;
    }
    return sum;
}

