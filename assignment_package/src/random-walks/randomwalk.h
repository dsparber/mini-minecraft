#ifndef RANDOMWALK_H
#define RANDOMWALK_H

#include "smartpointerhelp.h"
#include "randomwalkstate.h"
#include "QRunnable"


class RandomWalk : public QRunnable
{
private:
    sPtr<RandomWalkState> currentState;
    RandomWalk(sPtr<RandomWalkState>);

public:
    static void start(sPtr<RandomWalkState>);
    void run();
};

#endif // RANDOMWALK_H
