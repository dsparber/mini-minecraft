#include "biometype.h"

BlockType getBlockType(BiomeType b) {
    if (b == WETLAND) {
        return WATER;
    }
    else if (b == DESERT)
    {
        return SAND;
    }
    else if (b == MOUNTAINS)
    {
        return STONE;
    }
    else
    {
        return GRASS;
    }
}
