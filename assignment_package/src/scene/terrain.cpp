#include <scene/terrain.h>
#include <iostream>

#include <scene/cube.h>
#include <iostream>
#include "fbm.h"


Terrain::Terrain (OpenGLContext* context) : context(context), chunkMap(), chunksToDraw()
{ }

void Terrain::initialize(){
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            initializeChunk(i * 16, j * 16);
        }
    }

    for(Chunk* c : getChunksToDraw()){
        c->create();
    }
}

BlockType Terrain::getBlockOrEmpty(int x, int y, int z) const {

    if (getChunk(x, z) == nullptr || y < 0 || y >= 256) {
        return EMPTY;
    }
    return getBlockAt(x, y, z);
}

void Terrain::initializeChunk(int chunkX, int chunkZ) {

    // Ensure alignment to 16
    chunkX = (chunkX / 16) * 16;
    chunkZ = (chunkZ / 16) * 16;

    Chunk* c = new Chunk(context, glm::vec4(chunkX, 0, chunkZ, 0));
    int64_t key = getHashKey(chunkX, chunkZ);

    // Generate blocks
    for(int x = 0; x < 16; ++x)
    {
        for(int z = 0; z < 16; ++z)
        {
            for(int y = 0; y < 256; ++y)
            {
                int fbmX = chunkX + x;
                int fbmZ = chunkZ + z;

                if(y <= 128) {
                    c->setBlockAt(x, y, z, STONE);
                }
                else
                {
                    float rawFBM = fbm(fbmX / 64.f, fbmZ / 64.f);
                    float fbmVal = 32.f * powf(rawFBM, 4.f);
                    int intFBM = 128 + (int) fbmVal;

                    for (int i = 129; i < intFBM; i++) {
                        c->setBlockAt(x, i, z, DIRT);
                    }

                    c->setBlockAt(x, intFBM, z, GRASS);


                    for (int i = intFBM + 1; i < 134; i++) {
                        c->setBlockAt(x, i, z, WATER);
                    }
                }
            }
        }
    }

    // Link neighbors
    c->left = getChunk(chunkX - 16, chunkZ);
    if (c->left != nullptr) {
        c->left->right = c;
    }
    c->right = getChunk(chunkX + 16, chunkZ);
    if (c->right != nullptr) {
        c->right->left = c;
    }
    c->front = getChunk(chunkX, chunkZ - 16);
    if (c->front != nullptr) {
        c->front->back = c;
    }
    c->back = getChunk(chunkX, chunkZ + 16);
    if (c->back != nullptr) {
        c->back->front = c;
    }

    //c->create();
    chunkMap[key] = c;
    chunksToDraw.push_back(c);
}


//x is in lower 32 bits, z is in upper 32 bits
int64_t Terrain::getHashKey(int x, int z) const {
    int64_t nx = (int64_t)x;
    int64_t nz = (int64_t)z;
    nz = nz << 32;
    nx = nx & 0x00000000ffffffff;
    return nx | nz;
}

glm::vec2 Terrain::getCoordFromKey(int64_t key) const{
    int64_t x = key & 0x00000000ffffffff;
    //check if x is negative
    if(x & 2147483648 != 0){
        x = 0xffffffff00000000 | x;
    }
    int64_t z = key>>32;
    return(glm::vec2(x, z));
}

BlockType Terrain::getBlockAt(glm::vec3 pos) const
{
    return getBlockAt((int)pos.x, (int)pos.y, (int)pos.z);
}

BlockType Terrain::getBlockAt(int x, int y, int z) const
{
    // Get coordinates aligned to chunk
    int chunkX = (x / 16) * 16;
    int chunkZ = (z / 16) * 16;

    // Get local block coordinates
    int blockX = x - chunkX;
    int blockZ = z - chunkZ;

    // Get key for chunk map
    int64_t chunkKey = Terrain::getHashKey(chunkX, chunkZ);

    // Get chunk
    Chunk* chunk = chunkMap.at(chunkKey);

    // Get block of chunk
    return chunk->getBlockAt(blockX, y, blockZ);
}

BlockType& Terrain::getBlockAt(int x, int y, int z)
{
    // Get coordinates aligned to chunk
    int chunkX = (x / 16) * 16;
    int chunkZ = (z / 16) * 16;

    // Get local block coordinates
    int blockX = x - chunkX;
    int blockZ = z - chunkZ;

    // Get key for chunk map
    int64_t chunkKey = Terrain::getHashKey(chunkX, chunkZ);

    // Get chunk reference
    Chunk* chunk = chunkMap[chunkKey];

    // Get block of chunk
    return chunk->getBlockAt(blockX, y, blockZ);
}

Chunk* Terrain::getChunk(int x, int z) const {
    // Get coordinates aligned to chunk
    int chunkX = (x / 16) * 16;
    int chunkZ = (z / 16) * 16;

    // Get key for chunk map
    int64_t chunkKey = Terrain::getHashKey(chunkX, chunkZ);

    if (chunkMap.find(chunkKey) == chunkMap.end()) {
        return nullptr;
    }

    // Get chunk
    return chunkMap.at(chunkKey);
}

void Terrain::setBlockAt(int x, int y, int z, BlockType t)
{
    // Get coordinates aligned to chunk
    int chunkX = (x / 16) * 16;
    int chunkZ = (z / 16) * 16;

    // Get local block coordinates
    int blockX = x - chunkX;
    int blockZ = z - chunkZ;

    // Get key for chunk map
    int64_t chunkKey = Terrain::getHashKey(chunkX, chunkZ);
    if(chunkMap.count(chunkKey) > 0){
        // Get chunk reference
        Chunk* chunk = chunkMap[chunkKey];

        // Set block
        chunk->setBlockAt(blockX, y, blockZ, t);
    }
}

void Terrain::addBlock(glm::vec3 eye, glm::vec3 look)
{
    glm::vec3 addPos = rayMarch(eye, look) - 0.75f * look;
    glm::vec3 coords(glm::floor(addPos));
    setBlockAt(coords.x, coords.y, coords.z, LAVA);
    Chunk* c = getChunk((int)coords.x, (int)coords.z);
    c->destroy();
    c->create();
}

void Terrain::removeBlock(glm::vec3 eye, glm::vec3 look)
{
    glm::vec3 coords(glm::floor(rayMarch(eye, look)));
    setBlockAt(coords.x, coords.y, coords.z, EMPTY);
    Chunk* c = getChunk((int)coords.x, (int) coords.z);
    c->destroy();
    c->create();
    // TODO update neighbor if block deleted at border
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

    for (int i = 0; i < 8; i++) {

        glm::vec3 delta = glm::mat3(glm::rotate(glm::mat4(), i * glm::pi<float>() / 4, glm::vec3(0, 0, 1))) * glm::vec3(16, 0, 0);

        glm::vec3 testPos = playerPosition + delta;

        Chunk* testChunk = getChunk(testPos.x, testPos.z);

        if (testChunk == nullptr) {
            initializeChunk(testPos.x, testPos.z);
        }
    }
}

std::vector<Chunk*> Terrain::getChunksToDraw() const {
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
