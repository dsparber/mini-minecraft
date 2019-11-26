#ifndef CREATECHUNKRUNNABLE_H
#define CREATECHUNKRUNNABLE_H

#include <QRunnable>
#include <QMutex>
#include <vector>
#include <set>
#include <unordered_map>
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
    inline static std::set<int64_t> requested;

    /// Mutex for the created map
    inline static QMutex requestedMutex = QMutex();

    /// Coordinates of new chunk
    Chunk* chunk;

    /// Used to pass chunks to terrain on completion
    std::vector<Chunk*>* completedChunks;

    /// Mutex to avoid race conditions on modifying collections
    QMutex* terrainMutex;

    OpenGLContext* context;

    CreateChunkRunnable(OpenGLContext*, Chunk*, std::vector<Chunk*>*, QMutex*);

    static void init(OpenGLContext*, int, int, std::vector<Chunk*>*, QMutex*);

public:
    static void create(OpenGLContext*, int, int, std::vector<Chunk*>*, QMutex*);

    void run();
};

#endif // CREATECHUNKRUNNABLE_H
