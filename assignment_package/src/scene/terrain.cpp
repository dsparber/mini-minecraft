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
    Chunk* c1 = new Chunk(context, glm::vec4(0,0,0,0));
    Chunk* c2 = new Chunk(context, glm::vec4(16,0,0,0));
    Chunk* c3 = new Chunk(context, glm::vec4(16,0,16,0));
    Chunk* c4 = new Chunk(context, glm::vec4(0,0,16,0));

    Chunk* c5 = new Chunk(context, glm::vec4(32,0,0,0));
    Chunk* c6 = new Chunk(context, glm::vec4(48,0,0,0));
    Chunk* c7 = new Chunk(context, glm::vec4(48,0,16,0));
    Chunk* c8 = new Chunk(context, glm::vec4(32,0,16,0));

    Chunk* c9 = new Chunk(context, glm::vec4(32,0,32,0));
    Chunk* c10 = new Chunk(context, glm::vec4(48,0,32,0));
    Chunk* c11 = new Chunk(context, glm::vec4(48,0,48,0));
    Chunk* c12 = new Chunk(context, glm::vec4(32,0,48,0));

    Chunk* c13 = new Chunk(context, glm::vec4(0,0,32,0));
    Chunk* c14 = new Chunk(context, glm::vec4(16,0,32,0));
    Chunk* c15 = new Chunk(context, glm::vec4(16,0,48,0));
    Chunk* c16 = new Chunk(context, glm::vec4(0,0,48,0));

    c1->right = c2;
    c1->back = c4;
    c2->left = c1;
    c2->back = c3;
    c2->right = c5;
    c3->front = c2;
    c3->left = c4;
    c4->front = c1;
    c4->right = c3;
    //keys
    int64_t k1 = Terrain::getHashKey(0, 0);
    int64_t k2 = Terrain::getHashKey(16, 0);
    int64_t k3 = Terrain::getHashKey(16, 16);
    int64_t k4 = Terrain::getHashKey(0, 16);

    int64_t k5 = Terrain::getHashKey(32, 0);
    int64_t k6 = Terrain::getHashKey(48, 0);
    int64_t k7 = Terrain::getHashKey(48, 16);
    int64_t k8 = Terrain::getHashKey(32, 16);

    int64_t k9 = Terrain::getHashKey(32, 32);
    int64_t k10 = Terrain::getHashKey(48, 32);
    int64_t k11 = Terrain::getHashKey(48, 48);
    int64_t k12 = Terrain::getHashKey(32, 48);

    int64_t k13 = Terrain::getHashKey(0, 32);
    int64_t k14 = Terrain::getHashKey(16, 32);
    int64_t k15 = Terrain::getHashKey(16, 48);
    int64_t k16 = Terrain::getHashKey(0, 48);

    chunkMap.insert(std::make_pair(k1, c1));
    chunkMap.insert(std::make_pair(k2, c2));
    chunkMap.insert(std::make_pair(k3, c3));
    chunkMap.insert(std::make_pair(k4, c4));

    chunkMap.insert(std::make_pair(k5, c5));
    chunkMap.insert(std::make_pair(k6, c6));
    chunkMap.insert(std::make_pair(k7, c7));
    chunkMap.insert(std::make_pair(k8, c8));

    chunkMap.insert(std::make_pair(k9, c9));
    chunkMap.insert(std::make_pair(k10, c10));
    chunkMap.insert(std::make_pair(k11, c11));
    chunkMap.insert(std::make_pair(k12, c12));

    chunkMap.insert(std::make_pair(k13, c13));
    chunkMap.insert(std::make_pair(k14, c14));
    chunkMap.insert(std::make_pair(k15, c15));
    chunkMap.insert(std::make_pair(k16, c16));

    //    //CreateTestScene();
    //    Chunk* c = new Chunk(context, glm::vec4(0,0,0,0));
    //    int64_t k1 = Terrain::getHashKey(0, 0);
    //    chunkMap.insert({k1, c});
    //    setBlockAt(0,0,0,STONE);
    //    //getBlockAt(0,0,0) = STONE;
    //    //Chunk c1 = chunkMap[(int64_t)0];
    //    //BlockType test = c1.m_blocks[0];
    //    BlockType t = getBlockAt(0,0,0);
}

void Terrain::addChunk(glm::vec4 pos){
    Chunk* c = new Chunk(context, pos);
    int64_t k = Terrain::getHashKey(pos.x, pos.z);
    chunkMap.insert({k, c});
}


//x is in lower 32 bits, z is in upper 32 bits
int64_t Terrain::getHashKey(int x, int z){
    int64_t nx = (int64_t)x;
    int64_t nz = (int64_t)z;
    nz = nz << 32;
    nx = nx & 0x00000000ffffffff;
    //std::cout<<"getHashKey:"<<std::endl;
    //std::cout<<"key:"<<(int64_t)(nx | nz)<<" x:"<<x<<" z:"<<z<<std::endl;
    return nx | nz;
}

glm::vec2 Terrain::getCoordFromKey(int64_t key) const{
    int64_t x = key & 0x00000000ffffffff;
    //check if x is negative
    if(x & 2147483648 != 0){
        x = 0xffffffff00000000 | x;
    }
    int64_t z = key>>32;
    //std::cout<<"getCoordFromKey:"<<std::endl;
    //std::cout<<"key:"<<key<<" x:"<<x<<" z:"<<z<<std::endl;
    return(glm::vec2(x, z));
}

BlockType Terrain::getBlockAt(int x, int y, int z) const
{
    //    locate which chunk it is in
    //    check if the coordinate belong to the chunk

    for(auto entry : chunkMap){
        int64_t key = entry.first;
        Chunk* c = entry.second;
        glm::vec2 xzPos = getCoordFromKey(key);
        int newx = x - xzPos.x;
        int newz = z - xzPos.y;
        if(x>=xzPos.x && x<xzPos.x+16 && z>=xzPos.y && z<xzPos.y+16){
            int idx = 16 * 256 * newx + 16 * y + newz;
            BlockType t = c->m_blocks[idx];
            return c->m_blocks[idx];
        }
        return EMPTY;
    }
}

BlockType& Terrain::getBlockAt(int x, int y, int z)
{
    //    locate which chunk it is in
    //    check if the coordinate belong to the chunk
    for(auto entry : chunkMap){
        int64_t key = entry.first;
        Chunk* c = entry.second;
        glm::vec2 xzPos = getCoordFromKey(key);
        int newx = x - xzPos.x;
        int newz = z - xzPos.y;
        if(x>=xzPos.x && x<xzPos.x+16 && z>=xzPos.y && z<xzPos.y+16){
            int idx = 16 * 256 * newx + 16 * y + newz;
            BlockType t = c->m_blocks[idx];
            return c->m_blocks[idx];
        }
    }
}

void Terrain::setBlockAt(int x, int y, int z, BlockType t)
{
    //locate which chunk it is in
    //check if the coordinate belong to the chunk

    for(auto entry : chunkMap){
        Chunk* c = entry.second;
        int64_t key = entry.first;
        glm::vec2 xzPos = getCoordFromKey(key);
        int newx = x - xzPos.x;
        int newz = z - xzPos.y;
        if(x>=xzPos.x && x<xzPos.x+16 && z>=xzPos.y && z<xzPos.y+16){
            int idx = 16 * 256 * newx + 16 * y + newz;
            c->m_blocks[idx] = t;
            BlockType t1 = c->m_blocks[0];
            break;
        }
    }
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

void Terrain::CreateTestScene()
{
    // Amelia has a Chunk::getBlockAt(x, y, z) in Chunk class
    // loop through every chunk
    // for each chunk go through every block and get its WORLD (x,z) coord
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
                    // Amelia will have a map going from chunk to world
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
                            setBlockAt(newX, intFBM, newZ, DIRT);
                        }

                        setBlockAt(newX, intFBM, newZ, GRASS);
                    }
                }
            }
        }
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


// Generating new terrain
// How/when/where to check characters position to call this function
// need consistent checking, checking in every direction, put it in the game loop (paintGL)
    // every time timer ticks, check what area of world the character is in, if that location is close to the edge of that part of the world,
    // then check if the blocks on the edge have neighbors, if not generate new terrain
    // add new chunks to map of chunks
// get current coordinates from camera, check if camera's (x,z) is in the map or use getBlockAt to see if a block exists there (could create a
//      block type called NO_BLOCK (different than empty)

// Need a function from Amelia to generate a new chunk, each time the player is
//      within ___ of the edge, generate a new chunk
void Terrain::generateTerrain(glm::vec3 currPos) {
    // get current chunk position
    // position of new chunk =
//void createChunk(position of this chunk);
}

//    // Add "walls" for collision testing
//    for(int x = 0; x < 64; ++x)
//    {
//        m_blocks[x][129][0] = GRASS;
//        m_blocks[x][130][0] = GRASS;
//        m_blocks[x][129][63] = GRASS;
//        m_blocks[0][130][x] = GRASS;
//    }
//    for(int y = 129; y < 140; ++y)
//    {
//        m_blocks[32][y][32] = GRASS;
//    }


Terrain::~Terrain(){

    chunkMap = std::unordered_map<int64_t, Chunk*>();

}
