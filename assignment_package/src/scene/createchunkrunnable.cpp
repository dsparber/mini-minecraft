#include "createchunkrunnable.h"
#include "fbm.h"
#include "biomes.h"
#include <iostream>
#include <QThreadPool>
#include "biometype.h"

CreateChunkRunnable::CreateChunkRunnable(OpenGLContext* context, Chunk* chunk, std::vector<Chunk*>* completedChunks, QMutex* mutex) :
    chunk(chunk),
    completedChunks(completedChunks),
    terrainMutex(mutex),
    context(context)
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
    if (requested.find(key) != requested.end()) {
        mutex.unlock();
        return;
    }
    requested.insert(key);
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
            int fbmX = chunk->pos.x + x - 40;
            int fbmZ = chunk->pos.z + z - 120;

            //float rawFBM = fbm(fbmX / 64.f, fbmZ / 64.f);
            //float fbmVal = 25.f * powf(rawFBM, 3.f);

            // Get moisture and bumpiness values
            glm::vec3 mb(mbNoise(fbmX / 256.f, fbmZ / 256.f));

            // Get the current biome we are in
            BiomeType currBiome = getCurrBiome(glm::vec2(mb.x, mb.y));

            // Get height values from each biome
            glm::vec4 biomeHeights(desertNoise(fbmX / 64.f, fbmZ / 64.f), wetlandNoise(fbmX / 64.f, fbmZ / 64.f),
                                  grasslandNoise(fbmX / 64.f, fbmZ / 64.f), mountainNoise(fbmX / 64.f, fbmZ / 64.f));

            glm::vec2 currPos(fbmX, fbmZ);

            //float fbmVal = interpolateBiomes(biomeHeights, currBiome, currPos);
            float fbmVal = 0.f;

            if (currBiome == DESERT) {
                fbmVal = glm::mix(biomeHeights.x, biomeHeights.z, glm::smoothstep(0.45f, 0.55f, mb.z));
            } else if (currBiome == WETLAND) {
                fbmVal = glm::mix(biomeHeights.y, biomeHeights.z, glm::smoothstep(0.45f, 0.55f, mb.z));
            } else if (currBiome == GRASSLAND) {
                fbmVal = glm::mix(biomeHeights.z, biomeHeights.z, glm::smoothstep(0.45f, 0.55f, mb.z));
            } else {
                fbmVal = glm::mix(biomeHeights.w, biomeHeights.z, glm::smoothstep(0.45f, 0.55f, mb.z));
            }

            int intFBM = 128 + glm::round(fbmVal);
            int random = rand() % 4;

            for (int i = 0; i < 128; i++) {
                chunk->setBlockAt(x, i, z, STONE);
            }

            BlockType currBlock = getBlockType(currBiome);

            for (int i = 128; i <= intFBM; i++) {
                if (i == intFBM) {
                    chunk->setBlockAt(x, intFBM, z, currBlock);
                } else {
                    chunk->setBlockAt(x, i, z, DIRT);
                }
            }
//            for (int i = 128; i <= intFBM; i++) {
//                if (i > 148 + random) {
//                    chunk->setBlockAt(x, i, z, SNOW);
//                } else if(i == intFBM){
//                    chunk->setBlockAt(x, intFBM, z, GRASS);

//                } else {
//                    chunk->setBlockAt(x, i, z, DIRT);
//                }
//            }

//            if (intFBM == 128) {
//                //fbmVal = 5 * rawFBM;
//                fbmVal = 5 * fbmVal;
//                for (int i = 0; i < fbmVal; i++) {
//                    chunk->setBlockAt(x, 128 - i, z, WATER);
//                }
//            }
        }
    }

    int x = chunk->pos.x;
    int z = chunk->pos.z;

    std::vector<Chunk*> modifiedChunks;
    modifiedChunks.push_back(chunk);

    // Link neighbors
    mutex.lock();
    created[getHashKey(x, z)] = chunk;

    // Left
    int64_t neighborKey = getHashKey(x - 16, z);
    if (chunk->left == nullptr && created.find(neighborKey) != created.end()) {
        chunk->left = created[neighborKey];
        chunk->left->right = chunk;
        modifiedChunks.push_back(chunk->left);
    }
    // Right
    neighborKey = getHashKey(x + 16, z);
    if (chunk->right == nullptr && created.find(neighborKey) != created.end()) {
        chunk->right = created[neighborKey];
        chunk->right->left = chunk;
        modifiedChunks.push_back(chunk->right);
    }
    // Front
    neighborKey = getHashKey(x, z - 16);
    if (chunk->front == nullptr && created.find(neighborKey) != created.end()) {
        chunk->front = created[neighborKey];
        chunk->front->back = chunk;
        modifiedChunks.push_back(chunk->front);
    }
    // Back
    neighborKey = getHashKey(x, z + 16);
    if (chunk->back == nullptr && created.find(neighborKey) != created.end()) {
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
