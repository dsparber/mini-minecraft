#ifndef RANDOMWALK_H
#define RANDOMWALK_H

#include "smartpointerhelp.h"
#include "randomwalkstate.h"


class RandomWalk
{
private:
    sPtr<RandomWalkState> currentState;

public:
    RandomWalk(sPtr<RandomWalkState>);

    void start();
};

#endif // RANDOMWALK_H
