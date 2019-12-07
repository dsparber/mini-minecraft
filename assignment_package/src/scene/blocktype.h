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
    SAND,
    DARKSTONE,
    VOLCANO,
    BARK,
    HILLGRASS
};

bool isSolid(BlockType);

bool isOpaque(BlockType);

bool isLiquid(BlockType);

#endif // BLOCKTYPE_H
