#include <scene/chunk.h>

#include <scene/cube.h>

Chunk::Chunk() : dimensions(64, 256, 64)
{}

BlockType Chunk::getBlockAt(int x, int y, int z) const
{
    int idx = x * (y * z) + y * z + z;
    return m_blocks[idx];
}

BlockType& Chunk::getBlockAt(int x, int y, int z)
{
    int idx = x * (y * z) + y * z + z;
    return m_blocks[idx];
}

void Chunk::create(){
    //only create vertex data for block faces that lie on the boundary between an EMPTY block and a filled block.
    BlockType left;
    if(left == EMPTY){

    }

}
