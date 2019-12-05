#ifndef BIOMETYPE_H
#define BIOMETYPE_H
#include "blocktype.h"

enum BiomeType: unsigned char
{
    WETLAND,
    GRASSLAND,
    DESERT,
    MOUNTAINS
};

BlockType getBlockType(BiomeType);

#endif // BIOMETYPE_H

