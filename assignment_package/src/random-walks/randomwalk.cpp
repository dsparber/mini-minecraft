#include "randomwalk.h"

RandomWalk::RandomWalk(sPtr<RandomWalkState> startState)
{
    currentState = startState;
}

void RandomWalk::start() {

    while (currentState != nullptr) {
        currentState->step();
        currentState = currentState->next();
    }
}
