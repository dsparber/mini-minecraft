#ifndef BLOCKTYPE_H
#define BLOCKTYPE_H

enum BlockType: unsigned char
{
    EMPTY, GRASS, DIRT, STONE, LAVA, WATER
};

bool isSolid(BlockType);

#endif // BLOCKTYPE_H
