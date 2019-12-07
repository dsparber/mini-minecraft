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

glm::vec2 random2(glm::vec2 p) {
    glm::vec2 a = glm::sin(glm::vec2(glm::dot(p, glm::vec2(127.1,311.7)), glm::dot(p, glm::vec2(269.5,183.3))));
    a.x *= 43758.5453;
    a.y *= 43758.5453;
    return glm::fract(a);
}

float surflet(glm::vec2 p, glm::vec2 gridPoint) {
    // Compute the distance between p and the grid point along each axis, and warp it with a
    // quintic function so we can smooth our cells
    glm::vec2 diff = glm::abs(p - gridPoint);

    glm::vec2 pow5(glm::pow(diff.x, 5.f), glm::pow(diff.y, 5.f));
    glm::vec2 pow4(glm::pow(diff.x, 4.f), glm::pow(diff.y, 4.f));
    glm::vec2 pow3(glm::pow(diff.x, 3.f), glm::pow(diff.y, 3.f));

    glm::vec2 t = glm::vec2(1.f) - 6.f * pow5 - 15.f * pow4 + 10.f * pow3;
    // Get the random vector for the grid point (assume we wrote a function random2)
    glm::vec2 gradient = random2(gridPoint);
    // Get the vector from the grid point to P
    diff = p - gridPoint;
    // Get the value of our height field by dotting grid->P with our gradient
    float height = glm::dot(diff, gradient);
    // Scale our height field (i.e. reduce it) by our polynomial falloff function
    return height * t.x * t.y;
}

float perlinNoise(glm::vec2 uv) {
    float surfletSum = 0.f;
    // Iterate over the four integer corners surrounding uv
    for (int dx = 0; dx <= 1; ++dx)
    {
        for(int dy = 0; dy <= 1; ++dy)
        {
            surfletSum += surflet(uv, glm::floor(uv) + glm::vec2(uv.x, uv.y));
        }
    }
    return surfletSum;
}

