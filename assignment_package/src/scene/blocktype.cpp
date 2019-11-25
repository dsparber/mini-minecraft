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

bool isOpaque(BlockType type) {
    return type != WATER && type != ICE && type != EMPTY;
}

bool isLiquid(BlockType type) {
    return type == WATER || type == LAVA;
}
