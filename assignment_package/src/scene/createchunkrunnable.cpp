#include "createchunkrunnable.h"
#include "fbm.h"
#include <iostream>
#include <QThreadPool>

CreateChunkRunnable::CreateChunkRunnable(OpenGLContext* context, Chunk* chunk, std::vector<Chunk*>* completedChunks, QMutex* mutex) :
    context(context),
    chunk(chunk),
    completedChunks(completedChunks),
    terrainMutex(mutex)

{ }

void CreateChunkRunnable::create(
        OpenGLContext* context, int x, int z, std::vector<Chunk*>* completedChunks, QMutex* terrainMutex) {

    // Create chunk at given position
    init(context, x, z, completedChunks, terrainMutex);

    // Also create surrounding for neighbor linking
    init(context, x + 16, z, completedChunks, terrainMutex);
    init(context, x - 16, z, completedChunks, terrainMutex);
    init(context, x, z + 16, completedChunks, terrainMutex);
    init(context, x, z - 16, completedChunks, terrainMutex);
}

void CreateChunkRunnable::init(
        OpenGLContext* context, int x, int z, std::vector<Chunk*>* completedChunks, QMutex* terrainMutex) {

    // Create chunk
    Chunk* chunk = new Chunk(context, glm::vec4(x, 0, z, 0));
    int64_t key = getHashKey(x, z);

    // Check if already requested
    mutex.lock();
    if (created.find(key) != created.end()) {
        mutex.unlock();
        return;
    }
    created[key] = chunk;
    mutex.unlock();

    // Start task
    CreateChunkRunnable* task = new CreateChunkRunnable(context, chunk, completedChunks, terrainMutex);
    task->setAutoDelete(true);
    QThreadPool::globalInstance()->start(task);
}

void CreateChunkRunnable::run() {


    // Generate blocks
    for(int x = 0; x < 16; ++x)
    {
        for(int z = 0; z < 16; ++z)
        {
            int fbmX = chunk->pos.x + x;
            int fbmZ = chunk->pos.z + z;

            float rawFBM = fbm(fbmX / 64.f, fbmZ / 64.f);
            float fbmVal = 64.f * powf(rawFBM, 4.f);
            int intFBM = 128 + (int) fbmVal;

            for (int i = 128; i < 132; i++) {
                chunk->setBlockAt(x, i, z, WATER);
            }

            for (int i = 0; i < 128; i++) {
                chunk->setBlockAt(x, i, z, STONE);
            }

            for (int i = 128; i < intFBM; i++) {
                if (intFBM > 150) {
                    chunk->setBlockAt(x, i, z, ICE);
                } else {
                    chunk->setBlockAt(x, i, z, DIRT);
                }
            }

            if (intFBM > 150) {
                chunk->setBlockAt(x, intFBM, z, ICE);
            } else {
                chunk->setBlockAt(x, intFBM, z, GRASS);
            }
        }
    }

    int x = chunk->pos.x;
    int z = chunk->pos.z;
    int64_t key = getHashKey(x, z);

    std::vector<Chunk*> modifiedChunks;
    modifiedChunks.push_back(chunk);

    // Link neighbors
    mutex.lock();

    // Left
    int64_t neighborKey = getHashKey(x - 16, z);
    if (created.find(neighborKey) != created.end()) {
        chunk->left = created[neighborKey];
        chunk->left->right = chunk;
        modifiedChunks.push_back(chunk->left);
    }
    // Right
    neighborKey = getHashKey(x + 16, z);
    if (created.find(neighborKey) != created.end()) {
        chunk->right = created[neighborKey];
        chunk->right->left = chunk;
        modifiedChunks.push_back(chunk->right);
    }
    // Front
    neighborKey = getHashKey(x, z - 16);
    if (created.find(neighborKey) != created.end()) {
        chunk->front = created[neighborKey];
        chunk->front->back = chunk;
        modifiedChunks.push_back(chunk->front);
    }
    // Back
    neighborKey = getHashKey(x, z + 16);
    if (created.find(neighborKey) != created.end()) {
        chunk->back = created[neighborKey];
        chunk->back->front = chunk;
        modifiedChunks.push_back(chunk->back);
    }
    mutex.unlock();

    for (Chunk* c : modifiedChunks) {
        if (c->left != nullptr && c->right != nullptr && c->back != nullptr && c->front != nullptr) {
            c->compute();
            // Push new chunk to completedChunks vector
            terrainMutex->lock();
            completedChunks->push_back(c);
            terrainMutex->unlock();
        }
    }
}
