#ifndef TUNNELWALK_H
#define TUNNELWALK_H
#include "randomwalkstate.h"
#include "randomwalk.h"

class TunnelWalk : public RandomWalkState
{
private:
    /// Time to live
    int ttl;

    /// Current direction
    glm::vec3 direction;

public:
    TunnelWalk(Terrain*, glm::vec3);
    void step();
    sPtr<RandomWalkState> next();
};

#endif // TUNNELWALK_H
