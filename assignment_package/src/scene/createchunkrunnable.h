#ifndef CREATECHUNKRUNNABLE_H
#define CREATECHUNKRUNNABLE_H

#include <QRunnable>
#include <QMutex>
#include <vector>
#include <set>
#include "chunk.h"
#include "utils.h"
#include <unordered_map>


class CreateChunkRunnable : public QRunnable
{
private:
    /// Keep track of created chunks to link them
    inline static std::unordered_map<int64_t, Chunk*> created;

    /// Mutex for the created map
    inline static QMutex mutex = QMutex();

    /// Keeps track of what chunks were requested to compute
    inline static std::set<Chunk*> requested;

    /// Mutex for the created map
    inline static QMutex requestedMutex = QMutex();

    /// Pointer to new chunk
    Chunk* chunk;

    /// Used to pass chunks to terrain on completion
    std::vector<Chunk*>* completedChunks;

    /// Mutex to avoid race conditions on modifying collections
    QMutex* terrainMutex;

public:

    CreateChunkRunnable(Chunk*, std::vector<Chunk*>*, QMutex*);

    void run();
};

#endif // CREATECHUNKRUNNABLE_H
