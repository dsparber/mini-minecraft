#include "tunnelwalk.h"
#include "cavecreation.h"
#include "../scene/terrainutils.h"

TunnelWalk::TunnelWalk(Terrain* terrain, glm::vec3 position) : RandomWalkState(terrain, position), direction(0, -1, 0), ttl(100 + rand() % 50)
{}

void TunnelWalk::step() {
    // Set blocks
    float radius = 4;
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

    // Update state
    position += direction;
    glm::vec3 random = glm::vec3(-.5f) + glm::vec3(rand() % 100, rand() % 100, rand() % 100) / 100.f;
    direction = direction + .5f * random;
    direction = glm::normalize(direction);

    if (position.y < 120) {
        direction.y = -.1f;
    }

    direction = glm::normalize(direction);
    --ttl;

    // Span new walk at random
    if ((rand() % 50) == 0) {
        sPtr<TunnelWalk> sideTunnel = mkS<TunnelWalk>(terrain, position);
        sideTunnel->direction = glm::vec3(direction.z, 0, -direction.y);
        sideTunnel->ttl = ttl * (rand() % 100) / 100.f + 50;
        RandomWalk::start(sideTunnel);
    }
}

sPtr<RandomWalkState> TunnelWalk::next() {
    if (ttl <= 0) {
        return mkS<CaveCreation>(terrain, position);
    }
    return shared_from_this();
}
