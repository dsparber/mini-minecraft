#pragma once
#include <QList>
#include <la.h>
#include <unordered_map>
#include <stdint.h>
#include "chunk.h"
#include "blocktype.h"

// C++ 11 allows us to define the size of an enum. This lets us use only one byte
// of memory to store our different block types. By default, the size of a C++ enum
// is that of an int (so, usually four bytes). This *does* limit us to only 256 different
// block types, but in the scope of this project we'll never get anywhere near that many.

class Terrain
{
public:
    Terrain(OpenGLContext* context);
    glm::ivec3 dimensions;
    BlockType m_blocks[64][256][64];                    // A 3D list of the blocks in the world.
    //std::unordered_map<int64_t, Chunk> chunkMap;        //a map of chunk's coordinate coded in 64 bits as key to the chunk
    glm::vec4 pos; //global position of the left most block
    void setMap();
    void CreateTestScene();
    OpenGLContext* context;

    BlockType getBlockAt(int x, int y, int z) const;   // Given a world-space coordinate (which may have negative
                                                           // values) return the block stored at that point in space.
    void setBlockAt(int x, int y, int z, BlockType t); // Given a world-space coordinate (which may have negative
                                                           // values) set the block at that point in space to the
                                                           // given type.
    int64_t getHashKey(int x, int z);
    glm::vec2 getCoordFromKey(int64_t key) const;
};
