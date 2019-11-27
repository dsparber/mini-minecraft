#ifndef CAVECREATION_H
#define CAVECREATION_H
#include "randomwalkstate.h"

class CaveCreation : public RandomWalkState
{
public:
    CaveCreation(Terrain*, glm::vec3);
    void step();
    sPtr<RandomWalkState> next();
};

#endif // CAVECREATION_H
