#include "blocktype.h"

bool isSolid(BlockType type) {
    return type == STONE
            || type == GRASS
            || type == DIRT
            || type == WOOD
            || type == LEAF
            || type == BEDROCK
            || type == ICE;
}

bool isLiquid(BlockType type) {
    return type == WATER || type == LAVA;
}
