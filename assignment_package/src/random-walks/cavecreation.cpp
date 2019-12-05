#include "cavecreation.h"
#include "../scene/terrainutils.h"

CaveCreation::CaveCreation(Terrain* terrain, glm::vec3 position) : RandomWalkState(terrain, position)
{}

void CaveCreation::step() {
    int radius = 10;
    int start = -radius;
    int end = start * (-1);

    for (int dx = start; dx < end; dx++) {
        for (int dy = start; dy < end; dy++) {
            for (int dz = start; dz < end; dz++) {
                glm::vec3 delta = glm::vec3(dx, dy, dz);
                if (glm::length(delta) <= radius) {
                    if (dy < (start + end) * .33f) {
                        terrain->setBlockAt(position + delta, LAVA);
                    } else {
                        terrain->setBlockAt(position + delta, EMPTY);
                    }
                }
            }
        }
    }
}

sPtr<RandomWalkState> CaveCreation::next() {
    return nullptr;
}
