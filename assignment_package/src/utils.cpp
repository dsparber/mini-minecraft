#include "utils.h"

//x is in lower 32 bits, z is in upper 32 bits
int64_t getHashKey(int x, int z) {
    int64_t nx = (int64_t)x;
    int64_t nz = (int64_t)z;
    nz = nz << 32;
    nx = nx & 0x00000000ffffffff;
    return nx | nz;
}

glm::vec2 getCoordFromKey(int64_t key) {
    int64_t x = key & 0x00000000ffffffff;
    //check if x is negative
    if(x & 2147483648 != 0){
        x = 0xffffffff00000000 | x;
    }
    int64_t z = key>>32;
    return(glm::vec2(x, z));
}
