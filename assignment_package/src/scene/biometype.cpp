#include "biometype.h"

BlockType getBlockType(BiomeType b) {
    if (b == WETLAND) {
        return WATER;
    }
    else if (b == DESERT)
    {
        return LAVA;
    }
    else if (b == MOUNTAINS)
    {
        return SNOW;
    }
    else
    {
        return GRASS;
    }
}
