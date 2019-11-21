#include "blocktype.h"

bool isSolid(BlockType type) {
    return type != WATER && type != EMPTY;
}
