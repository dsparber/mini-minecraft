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

