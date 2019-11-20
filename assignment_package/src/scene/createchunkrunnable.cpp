#include "createchunkrunnable.h"
#include "fbm.h"

CreateChunkRunnable::CreateChunkRunnable(
        Chunk* c,
        std::vector<Chunk*>* createdChunks,
        QMutex* mutex) :
    chunk(c),
    createdChunks(createdChunks),
    createdChunksMutex(mutex)

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

            for (int i = intFBM + 1; i < 132; i++) {
                chunk->setBlockAt(x, i, z, WATER);
            }
        }
    }
    chunk->compute();

    // Push new chunk to createdChunks vector
    createdChunksMutex->lock();
    createdChunks->push_back(chunk);
    createdChunksMutex->unlock();
}
