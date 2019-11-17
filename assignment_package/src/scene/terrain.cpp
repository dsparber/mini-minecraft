#include <scene/terrain.h>
#include <iostream>

#include <scene/cube.h>
#include <iostream>


BlockType Terrain::getBlockOrEmpty(int x, int y, int z) const {
    if (x < 0 || x >= dimensions.x ||
            y < 0 || y >= dimensions.y ||
            z < 0 || z >= dimensions.z) {
        return EMPTY;
    }
    return getBlockAt(x, y, z);
}

Terrain::Terrain (OpenGLContext* context) : context(context), dimensions(64, 256, 64),chunkMap()
{
    setMap();
}

void Terrain::setMap(){
    generateTerrain(glm::vec3(0, 0, 0));
}

Chunk* Terrain::addChunk(glm::vec4 pos){
    Chunk* c = new Chunk(context, pos);
    int64_t k = Terrain::getHashKey(pos.x, pos.z);
    chunkMap[k] = c;
    return c;
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

    // Get chunk reference
    Chunk* chunk = chunkMap[chunkKey];

    // Set block
    chunk->setBlockAt(blockX, y, blockZ, t);
}

float rand(glm::vec2 n) {
    return (glm::fract(sin(glm::dot(n, glm::vec2(12.9898, 4.1414))) * 43758.5453));
}


float interpNoise2D(float x, float y) {
    float intX = glm::floor(x);
    float fractX = glm::fract(x);
    float intY = floor(y);
    float fractY = glm::fract(y);

    float v1 = rand(glm::vec2(intX, intY));
    float v2 = rand(glm::vec2(intX + 1, intY));
    float v3 = rand(glm::vec2(intX, intY + 1));
    float v4 = rand(glm::vec2(intX + 1, intY + 1));

    float i1 = glm::mix(v1, v2, fractX);
    float i2 = glm::mix(v3, v4, fractX);
    return glm::mix(i1, i2, fractY);
}

float fbm(float x, float z) {
    float total = 0;
    float persistence = 0.5f;
    int octaves = 8;

    for(int i = 1; i <= octaves; i++) {
        float freq = pow(2.f, i);
        float amp = pow(persistence, i);

        total += interpNoise2D(x * freq,
                               z * freq) * amp;
    }
    return total;
}

void Terrain::create()
{
    for(auto entry : chunkMap){
        Chunk* c = entry.second;
        int64_t key = entry.first;
        glm::vec2 chunkPos = getCoordFromKey(key);

        for(int x = 0; x < 16; ++x)
        {
            for(int z = 0; z < 16; ++z)
            {
                for(int y = 0; y < 256; ++y)
                {
                    int newX = chunkPos.x + x;
                    int newY = y;
                    int newZ = chunkPos[1] + z;

                    if(y <= 128) {
                        setBlockAt(newX, newY, newZ, STONE);
                    }
                    else
                    {
                        float rawFBM = fbm(newX / 64.f, newZ / 64.f);
                        float fbmVal = 85.f * powf(rawFBM, 4.f);
                        int intFBM = 128 + (int) fbmVal;

                        for (int i = 129; i < intFBM; i++) {
                            setBlockAt(newX, i, newZ, DIRT);
                        }

                        setBlockAt(newX, intFBM, newZ, GRASS);
                    }
                }
            }
        }
        c->create();
    }
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

void Terrain::checkAndCreate(glm::vec3 playerPosition) {

    for (int i = 0; i < 8; i++) {

        glm::vec3 delta = glm::mat3(glm::rotate(glm::mat4(), i * glm::pi<float>() / 4, glm::vec3(0, 0, 1))) * glm::vec3(16, 0, 0);

        glm::vec3 testPos = playerPosition + delta;

        Chunk* testChunk = getChunk(testPos.x, testPos.z);

        if (testChunk == nullptr) {
            generateTerrain(testPos);

        }
    }
}

void Terrain::generateTerrain(glm::vec3 currPos) {

    // Align coordinates
    glm::ivec3 requestedPos = (glm::ivec3) currPos;
    requestedPos = (requestedPos / 64) * 64;

    for (int i = 0; i < 64; i += 16)
    {
        for (int j = 0; j < 64; j += 16)
        {
            int x = requestedPos.x + i;
            int z = requestedPos.z + j;
            addChunk(glm::vec4(x, 0, z, 1.f));
        }
    }

    for (int i = 0; i < 64; i += 16)
    {
        for (int j = 0; j < 64; j += 16)
        {
            int x = requestedPos.x + i;
            int z = requestedPos.z + j;

            Chunk* c = getChunk(x, z);

            c->left = getChunk(x - 16, z);
            c->right = getChunk(x + 16, z);
            c->front = getChunk(x, z - 16);
            c->back = getChunk(x, z + 16);
        }
    }

    create();
}

Terrain::~Terrain(){

    chunkMap = std::unordered_map<int64_t, Chunk*>();

}
