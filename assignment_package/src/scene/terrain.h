#pragma once
#include <QList>
#include <la.h>
#include <unordered_map>
#include <stdint.h>
#include "chunk.h"
#include "blocktype.h"
#include <smartpointerhelp.h>

// C++ 11 allows us to define the size of an enum. This lets us use only one byte
// of memory to store our different block types. By default, the size of a C++ enum
// is that of an int (so, usually four bytes). This *does* limit us to only 256 different
// block types, but in the scope of this project we'll never get anywhere near that many.

class Terrain
{
public:
    Terrain(OpenGLContext* context);
    ~Terrain();
    glm::ivec3 dimensions;
    //BlockType m_blocks[64][256][64];                    // A 3D list of the blocks in the world.
    std::unordered_map<int64_t, Chunk*> chunkMap;        //a map of chunk's coordinate coded in 64 bits as key to the chunk
    glm::vec4 pos; //global position of the left most block
    OpenGLContext* context;

    void setMap();
    void CreateTestScene();

    BlockType getBlockOrEmpty(int x, int y, int z) const;

    BlockType& getBlockAt(int x, int y, int z) ;
    BlockType getBlockAt(int x, int y, int z) const;   // Given a world-space coordinate (which may have negative
                                                           // values) return the block stored at that point in space.
    void setBlockAt(int x, int y, int z, BlockType t); // Given a world-space coordinate (which may have negative
                                                           // values) set the block at that point in space to the
                                                           // given type.
    void addBlock(glm::vec3 eye, glm::vec3 look);
    void removeBlock(glm::vec3 eye, glm::vec3 look);
    glm::vec3 rayMarch(glm::vec3 eye, glm::vec3 look);
    void generateTerrain(glm::vec3 currPos);
                                                          // given type.
    void addChunk(glm::vec4 pos);
    int64_t getHashKey(int x, int z) const;
    glm::vec2 getCoordFromKey(int64_t key) const;
};
