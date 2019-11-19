#include "blocktype.h"

bool isSolid(BlockType type) {
    return type == STONE || type == GRASS || type == DIRT;
}
