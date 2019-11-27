#include "cavecreation.h"

CaveCreation::CaveCreation(Terrain* terrain, glm::vec3 position) : RandomWalkState(terrain, position)
{}

void CaveCreation::step() {
 // TODO
}

sPtr<RandomWalkState> CaveCreation::next() {
    return nullptr;
}
