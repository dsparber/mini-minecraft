#ifndef BLOCKTYPE_H
#define BLOCKTYPE_H

enum BlockType: unsigned char
{
    EMPTY,
    GRASS,
    HILLGRASS,
    DIRT,
    STONE,
    WOOD,
    LEAF,
    DARKLEAF,
    BEDROCK,
    LAVA,
    WATER,
    ICE,
    SNOW,
    SAND,
    DARKSTONE,
    VOLCANO,
    BARK
};

bool isSolid(BlockType);

bool isOpaque(BlockType);

bool isLiquid(BlockType);

#endif // BLOCKTYPE_H
