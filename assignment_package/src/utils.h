#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <cmath>
#include "la.h"


static const float PI = 3.14159265358979323846f;

/// Float approximate-equality comparison
template<typename T>
inline bool fequal(T a, T b, T epsilon = 0.0001){
    if (a == b) {
        // Shortcut
        return true;
    }

    const T diff = std::abs(a - b);
    if (a * b == 0) {
        // a or b or both are zero; relative error is not meaningful here
        return diff < (epsilon * epsilon);
    }

    return diff / (std::abs(a) + std::abs(b)) < epsilon;
}

/// Helper functions for converting x,z to int64
int64_t getHashKey(int x, int z);
glm::vec2 getCoordFromKey(int64_t key);

#endif // UTILS_H
