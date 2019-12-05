#ifndef RANDOMWALKSTATE_H
#define RANDOMWALKSTATE_H
#include "la.h"
#include "smartpointerhelp.h"
#include "../scene/terrain.h"

class RandomWalkState : public std::enable_shared_from_this<RandomWalkState>
{
protected:
    glm::vec3 position;
    Terrain* terrain;

    RandomWalkState(Terrain*, glm::vec3);

public:
    virtual void step() = 0;
    virtual sPtr<RandomWalkState> next() = 0;
};

#endif // RANDOMWALKSTATE_H
