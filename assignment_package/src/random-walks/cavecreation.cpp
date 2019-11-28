#include "cavecreation.h"
#include "../scene/terrainutils.h"

CaveCreation::CaveCreation(Terrain* terrain, glm::vec3 position) : RandomWalkState(terrain, position)
{}

void CaveCreation::step() {
    carveSphere(terrain, position, 10);
}

sPtr<RandomWalkState> CaveCreation::next() {
    return nullptr;
}
