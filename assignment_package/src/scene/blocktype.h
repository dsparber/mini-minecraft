#ifndef BLOCKTYPE_H
#define BLOCKTYPE_H

enum BlockType: unsigned char
{
    EMPTY,
    GRASS,
    DIRT,
    STONE,
    WOOD,
    LEAF,
    BEDROCK,
    LAVA,
    WATER,
    ICE,
    SNOW,
    COAL,
    ORE,
};

bool isSolid(BlockType);

bool isOpaque(BlockType);

bool isLiquid(BlockType);

bool drawBorder(BlockType, BlockType);

#endif // BLOCKTYPE_H
