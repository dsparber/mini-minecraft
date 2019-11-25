#include <scene/terrain.h>
#include <iostream>

#include <scene/cube.h>
#include <iostream>
#include "fbm.h"
#include "createchunkrunnable.h"


Terrain::Terrain (OpenGLContext* context) :
    context(context),
    chunkMap(),
    chunksToDraw(),
    requestedChunks(),
    createdChunks(),
    lastPlayerChunk(-1),
    waitingForChunks(false),
    newChunksAvailable(false)
{ }

int Terrain::chunksToRender = 5;

void Terrain::initialize(){
    playerMoved(glm::vec3(0, 0, 0));
    QThreadPool::globalInstance()->waitForDone();
}


BlockType Terrain::getBlockOrEmpty(glm::vec3 pos) const {
    return getBlockOrEmpty(pos.x, pos.y, pos.z);
}

BlockType Terrain::getBlockOrEmpty(int x, int y, int z) const {

    if (getChunk(x, z) == nullptr || y < 0 || y >= 256) {
        return EMPTY;
    }
    return getBlockAt(x, y, z);
}

void Terrain::updateChunkMap() {
    createdMutex.lock();

    newChunksAvailable = (createdChunks.size() > 0);

    // Insert chunks
    for (Chunk* chunk : createdChunks) {
        int64_t key = getHashKey(chunk->pos.x, chunk->pos.z);
        chunkMap[key] = chunk;
    }

    createdChunks.clear();

    createdMutex.unlock();
}

void Terrain::initializeChunk(int chunkX, int chunkZ) {

    int64_t key = getHashKey(chunkX, chunkZ);

    // Already requested
    if (requestedChunks.find(key) != requestedChunks.end()) {
        return;
    }

    requestedChunks.insert(key);

    Chunk* c = new Chunk(context, glm::vec4(chunkX, 0, chunkZ, 0));
    CreateChunkRunnable* create = new CreateChunkRunnable(c, &createdChunks, &createdMutex);
    create->setAutoDelete(true);
    QThreadPool::globalInstance()->start(create);
}

BlockType Terrain::getBlockAt(glm::vec3 pos) const
{
    return getBlockAt((int)pos.x, (int)pos.y, (int)pos.z);
}

BlockType Terrain::getBlockAt(int x, int y, int z) const
{
    // Get coordinates aligned to chunk
    int chunkX = glm::floor(x / 16.f) * 16;
    int chunkZ = glm::floor(z / 16.f) * 16;

    // Get local block coordinates
    int blockX = x - chunkX;
    int blockZ = z - chunkZ;

    // Get key for chunk map
    int64_t chunkKey = getHashKey(chunkX, chunkZ);

    // Get chunk
    Chunk* chunk = chunkMap.at(chunkKey);

    // Get block of chunk
    return chunk->getBlockAt(blockX, y, blockZ);
}

BlockType& Terrain::getBlockAt(int x, int y, int z)
{
    // Get coordinates aligned to chunk
    int chunkX = glm::floor(x / 16.f) * 16;
    int chunkZ = glm::floor(z / 16.f) * 16;

    // Get local block coordinates
    int blockX = x - chunkX;
    int blockZ = z - chunkZ;

    // Get key for chunk map
    int64_t chunkKey = getHashKey(chunkX, chunkZ);

    // Get chunk reference
    Chunk* chunk = chunkMap[chunkKey];

    // Get block of chunk
    return chunk->getBlockAt(blockX, y, blockZ);
}

Chunk* Terrain::getChunk(int x, int z) const {
    // Get coordinates aligned to chunk
    int chunkX = glm::floor(x / 16.f) * 16;
    int chunkZ = glm::floor(z / 16.f) * 16;

    // Get key for chunk map
    int64_t chunkKey = getHashKey(chunkX, chunkZ);

    if (chunkMap.find(chunkKey) == chunkMap.end()) {
        return nullptr;
    }

    // Get chunk
    return chunkMap.at(chunkKey);
}

void Terrain::setBlockAt(int x, int y, int z, BlockType t)
{
    // Get coordinates aligned to chunk
    int chunkX = glm::floor(x / 16.f) * 16;
    int chunkZ = glm::floor(z / 16.f) * 16;

    // Get local block coordinates
    int blockX = x - chunkX;
    int blockZ = z - chunkZ;

    // Get key for chunk map
    int64_t chunkKey = getHashKey(chunkX, chunkZ);

    if (chunkMap.find(chunkKey) == chunkMap.end()) {
        return;
    }

    // Get chunk reference
    Chunk* chunk = chunkMap[chunkKey];

    // Set block
    chunk->setBlockAt(blockX, y, blockZ, t);

    // Recreate chunk
    chunk->destroy();
    chunk->compute();
    chunk->create();

    // TODO check neighbors and update
}

void Terrain::addBlock(glm::vec3 eye, glm::vec3 look)
{
    glm::vec3 addPos = rayMarch(eye, look) - 0.75f * look;
    glm::vec3 coords(glm::floor(addPos));
    setBlockAt(coords.x, coords.y, coords.z, LAVA);
}

void Terrain::removeBlock(glm::vec3 eye, glm::vec3 look)
{
    glm::vec3 coords(glm::floor(rayMarch(eye, look)));
    setBlockAt(coords.x, coords.y, coords.z, EMPTY);
}

glm::vec3 Terrain::rayMarch(glm::vec3 eye, glm::vec3 look)
{
    float maxLen = 8.f;
    bool hit = false;
    glm::vec3 currPos = eye;
    float step = 0.75;

    while(!hit && glm::length(currPos - eye) < maxLen)
    {
        currPos = currPos + step * look;
        if(getBlockAt(glm::floor(currPos.x), glm::floor(currPos.y), glm::floor(currPos.z)) != EMPTY)
        {
            hit = true;
        }
    }
    return currPos;
}

void Terrain::playerMoved(glm::vec3 playerPosition) {

    updateChunkMap();

    int x = playerPosition.x;
    int z = playerPosition.z;
    x = glm::floor(x / 16.f) * 16;
    z = glm::floor(z / 16.f) * 16;

    int64_t playerChunk = getHashKey(x, z);

    if (playerChunk == lastPlayerChunk && (!waitingForChunks || !newChunksAvailable)) {
        return;
    }

    // Chunks that were previously drawn
    std::set<Chunk*> prevChunks;
    for (Chunk* c : chunksToDraw) {
        prevChunks.insert(c);
    }

    // Create chunks around player
    bool allChunksAvailable = true;
    for (int i = -chunksToRender; i <= chunksToRender; ++i) {
        for (int j = -chunksToRender; j <= chunksToRender; ++j) {
            int chunkX = x + 16 * i;
            int chunkZ = z + 16 * j;

            Chunk* chunk = getChunk(chunkX, chunkZ);

            // Initialize chunk if not existing
            if (chunk == nullptr) {
                initializeChunk(chunkX, chunkZ);
                allChunksAvailable = false;
                continue;
            }

            // Add chunk to draw set if not in set
            if (chunksToDraw.find(chunk) == chunksToDraw.end()) {
                chunksToDraw.insert(chunk);
            } else {
                // Chunk is also used in future
                prevChunks.erase(chunk);
            }
            chunk->create();
        }
    }
    waitingForChunks = !allChunksAvailable;

    // Destroy chunks that are no longer used
    for (Chunk* c : prevChunks) {
        chunksToDraw.erase(c);
        c->destroy();
    }
}

std::set<Chunk*> Terrain::getChunksToDraw() const {
    return chunksToDraw;
}

Terrain::~Terrain(){
    for(auto entry : chunkMap){
        entry.second->destroy();
        delete entry.second;
        entry.second = nullptr;
    }
    chunkMap = std::unordered_map<int64_t, Chunk*>();
}
