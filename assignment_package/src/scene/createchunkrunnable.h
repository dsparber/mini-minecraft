#ifndef CREATECHUNKRUNNABLE_H
#define CREATECHUNKRUNNABLE_H

#include <QRunnable>
#include <QMutex>
#include <vector>
#include "chunk.h"
#include "utils.h"


class CreateChunkRunnable : public QRunnable
{
private:
    Chunk* chunk;

    std::vector<Chunk*>* createdChunks;
    QMutex* createdChunksMutex;

public:

    CreateChunkRunnable(Chunk*, std::vector<Chunk*>*, QMutex*);

    void run();
};

#endif // CREATECHUNKRUNNABLE_H
