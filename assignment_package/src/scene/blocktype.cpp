#include "blocktype.h"

bool isSolid(BlockType type) {
    return type != EMPTY && !isLiquid(type);
}


bool isOpaque(BlockType type) {
    return type != WATER && type != ICE && type != EMPTY && type != GLASS;
}

bool isLiquid(BlockType type) {
    return type == WATER || type == LAVA;
}

bool drawBorder(BlockType t1, BlockType t2) {
    if ((t1 == EMPTY && t2 != EMPTY) || (t1 != EMPTY && t2 == EMPTY)) {
        return true;
    }
    if (isOpaque(t1) && !isOpaque(t2)) {
        return true;
    }
    if (t1 != LAVA && t2 == LAVA) {
        return true;
    }
    return false;
}
