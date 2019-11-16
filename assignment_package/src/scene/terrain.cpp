#include <scene/terrain.h>
#include <iostream>

#include <scene/cube.h>

Terrain::Terrain() : dimensions(64, 256, 64)
{}

BlockType Terrain::getBlockAt(int x, int y, int z) const
{
    // TODO: Make this work with your new block storage!
    return m_blocks[x][y][z];
}

void Terrain::setBlockAt(int x, int y, int z, BlockType t)
{
    // take in a world value
    // change to chunk space
    // set block within the chunk --> call the chunks setBlockAt function
    // TODO: Make this work with your new block storage!
    m_blocks[x][y][z] = t;
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

// Base code implementation
void Terrain::CreateTestScene()
{
    // Create the basic terrain floor
    for(int x = 0; x < 64; ++x)
    {
        for(int z = 0; z < 64; ++z)
        {
            for(int y = 127; y < 256; ++y)
            {
                if(y <= 128)
                {
                        m_blocks[x][y][z] = STONE;
                }
                else
                {
                    // Terrain generation
                    // multiple by 32 and add 128
                    float rawFBM = fbm(x / 64.f, z / 64.f);
                    float fbmVal = 85.f * powf(rawFBM, 4.f);
                    int intFBM = 128 + (int) fbmVal;

                    for (int i = 129; i < intFBM; i++) {
                        m_blocks[x][i][z] = DIRT;
                    }

                    m_blocks[x][intFBM][z] = GRASS;
                }
            }
        }
    }
}

//// New implementation with chunking
//void Terrain::CreateTestScene()
//{
//    // Amelia has a Chunk::getBlockAt(x, y, z) in Chunk class
//    // loop through every chunk
//    // for each chunk go through every block and get its WORLD (x,z) coord
//    for(/* each chunk c*/)
//    {
//        for(int x = 0; x < 16; ++x)
//        {
//            for(int z = 0; z < 16; ++z)
//            {
//                for(int y = 0; y < 256; ++y)
//                {
//                    // Amelia will have a map going from chunk to world
//                    int newX = /*change to world*/;
//                    int newY = /*change to world*/;
//                    int newZ = /*change to world*/;

//                    if(y <= 128) {
//                        c.setBlockAt(newX, newY, newZ, STONE);
//                    }
//                    else
//                    {
//                        float rawFBM = fbm(newX / 64.f, newZ / 64.f);
//                        float fbmVal = 85.f * powf(rawFBM, 4.f);
//                        int intFBM = 128 + (int) fbmVal;

//                        for (int i = 129; i < intFBM; i++) {
//                            c.setBlockAt(newX, intFBM, newZ, DIRT);
//                        }

//                        c.setBlockAt(newX, intFBM, newZ, GRASS);
//                    }
//                }
//            }
//        }
//    }
//}

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
