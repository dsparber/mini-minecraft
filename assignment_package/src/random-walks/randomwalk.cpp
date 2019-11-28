#include "randomwalk.h"

RandomWalk::RandomWalk(sPtr<RandomWalkState> startState)
{
    currentState = startState;
}


void RandomWalk::start(sPtr<RandomWalkState> startState) {
    RandomWalk* walk = new RandomWalk(startState);
    walk->setAutoDelete(true);
    QThreadPool::globalInstance()->start(walk);
}


void RandomWalk::run() {
    while (currentState != nullptr) {
        currentState->step();
        currentState = currentState->next();
    }
}
