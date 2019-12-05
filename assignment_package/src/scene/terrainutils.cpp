#include "terrainutils.h"

void carveHalfSphere(Terrain* terrain, glm::vec3 position, float radius) {

    int start = glm::floor(-radius);
    int end = start * (-1);

    for (int dx = start; dx < end; dx++) {
        for (int dy = 0; dy < end; dy++) {
            for (int dz = start; dz < end; dz++) {
                glm::vec3 delta = glm::vec3(dx, dy, dz);
                if (glm::length(delta) <= radius)
                {
                    terrain->setBlockAt(position + delta, EMPTY);
                }
            }
        }
    }
}

void carveSphere(Terrain* terrain, glm::vec3 position, float radius) {

    int start = glm::floor(-radius);
    int end = start * (-1);

    for (int dx = start; dx < end; dx++) {
        for (int dy = start; dy < end; dy++) {
            for (int dz = start; dz < end; dz++) {
                glm::vec3 delta = glm::vec3(dx, dy, dz);
                if (glm::length(delta) <= radius)
                {
                    terrain->setBlockAt(position + delta, EMPTY);
                }
            }
        }
    }
}
