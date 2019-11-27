#include "tunnelwalk.h"
#include "cavecreation.h"
#include "../scene/terrainutils.h"

TunnelWalk::TunnelWalk(Terrain* terrain, glm::vec3 position) : RandomWalkState(terrain, position), direction(0, -1, 0), ttl(100 + rand() % 50)
{}

void TunnelWalk::step() {
    carveHalfSphere(terrain, position, 3);

    // Update state
    position += direction;
    glm::vec3 random = glm::vec3(-.5f) + glm::vec3(rand() % 100, rand() % 100, rand() % 100) / 100.f;
    direction = direction + .5f * random;

    // Stay at same height
    if (position.y < 120) {
        direction.y = 0;
    }
    // Go down
    if (position.y > 120) {
        //direction.y = 0.05 * (-(rand() % 100) / 100.f);
    }

    direction = glm::normalize(direction);

    --ttl;
}

sPtr<RandomWalkState> TunnelWalk::next() {
    if (ttl <= 0) {
        return mkS<CaveCreation>(terrain, position);
    }
    return shared_from_this();
}
