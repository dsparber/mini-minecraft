#include "createchunkrunnable.h"
#include "fbm.h"
#include <iostream>

CreateChunkRunnable::CreateChunkRunnable(
        Chunk* c,
        std::vector<Chunk*>* completedChunks,
        QMutex* mutex) :
    chunk(c),
    completedChunks(completedChunks),
    terrainMutex(mutex)

{ }

void CreateChunkRunnable::run() {

    // Generate blocks
    for(int x = 0; x < 16; ++x)
    {
        for(int z = 0; z < 16; ++z)
        {
            int fbmX = chunk->pos.x + x;
            int fbmZ = chunk->pos.z + z;

            float rawFBM = fbm(fbmX / 64.f, fbmZ / 64.f);
            float fbmVal = 32.f * powf(rawFBM, 4.f);
            int intFBM = 128 + (int) fbmVal;

            for (int i = 0; i < 128; i++) {
                chunk->setBlockAt(x, i, z, STONE);
            }

            for (int i = 128; i < intFBM; i++) {
                chunk->setBlockAt(x, i, z, DIRT);
            }

            chunk->setBlockAt(x, intFBM, z, GRASS);

            for (int i = 128; i < 132; i++) {
                chunk->setBlockAt(x, i, z, WATER);
            }
        }
    }

    int x = chunk->pos.x;
    int z = chunk->pos.z;
    int64_t key = getHashKey(x, z);

    std::vector<Chunk*> computeNeeded;
    computeNeeded.push_back(chunk);

    // Link neighbors
    mutex.lock();
    created[key] = chunk;

    // Left
    int64_t neighborKey = getHashKey(x - 16, z);
    if (created.find(neighborKey) != created.end()) {
        chunk->left = created[neighborKey];
        chunk->left->right = chunk;
        computeNeeded.push_back(chunk->left);
    }
    // Right
    neighborKey = getHashKey(x + 16, z);
    if (created.find(neighborKey) != created.end()) {
        chunk->right = created[neighborKey];
        chunk->right->left = chunk;
        computeNeeded.push_back(chunk->right);
    }
    // Front
    neighborKey = getHashKey(x, z - 16);
    if (created.find(neighborKey) != created.end()) {
        chunk->front = created[neighborKey];
        chunk->front->back = chunk;
        computeNeeded.push_back(chunk->front);
    }
    // Back
    neighborKey = getHashKey(x, z + 16);
    if (created.find(neighborKey) != created.end()) {
        chunk->back = created[neighborKey];
        chunk->back->front = chunk;
        computeNeeded.push_back(chunk->back);
    }
    mutex.unlock();

    for (Chunk* c : computeNeeded) {
        c->mutex.lock();
        c->compute();
        c->mutex.unlock();
    }

    // Push new chunk to completedChunks vector
    terrainMutex->lock();
    completedChunks->push_back(chunk);
    terrainMutex->unlock();
}
