#ifndef TUNNELWALK_H
#define TUNNELWALK_H
#include "randomwalkstate.h"

class TunnelWalk : public RandomWalkState
{
private:
    /// Time to live
    unsigned int ttl;

    /// Current direction
    glm::vec3 direction;

public:
    TunnelWalk(Terrain*, glm::vec3);
    void step();
    sPtr<RandomWalkState> next();
};

#endif // TUNNELWALK_H
