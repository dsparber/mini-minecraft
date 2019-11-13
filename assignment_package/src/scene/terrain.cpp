#include <scene/terrain.h>

#include <scene/cube.h>

Terrain::Terrain() : dimensions(64, 256, 64),chunks(), chunkMap()
{}

void Terrain::setMap(){
    for(Chunk c :chunks){
        int64_t key = Terrain::getHashKey((int)(c.pos.x), (int)(c.pos.y));
        chunkMap[key] = c;
    }

}
//x is in lower 32 bits, z is in upper 32 bits
int64_t Terrain::getHashKey(int x, int z){
    int64_t nx = (int64_t)x;
    int64_t nz = (int64_t)z;
    nz = nz << 32;
    nx = nx & 0x00000000ffffffff;
    return nx | nz;
}

glm::vec2 Terrain::getCoordFromKey(int64_t key){
    int64_t x = key & 0x00000000ffffffff;
    //check if x is negative
    if(x & 2147483648 != 0){
        x = 0xffffffff00000000 | x;
    }
    int64_t z = key>>32;
    return(glm::vec2(x, z));
}

BlockType Terrain::getBlockAt(int x, int y, int z) const
{
    Chunk c;
    //locate which chunk it is in
    //check if the coordinate belong to the chunk
    for(auto entry : chunkMap){
        int64_t key = entry.first;
        glm::vec2 xzPos = getCoordFromKey(key);
        if(x>xzPos.x && x<xzPos.x+16 && z>xzPos.y && z<xzPos.y+16){
            c = entry.second;
            break;
        }
    }
    return c.getBlockAt(x, y, z);
}

void Terrain::setBlockAt(int x, int y, int z, BlockType t)
{
    Chunk c;
    //locate which chunk it is in
    //check if the coordinate belong to the chunk
    for(auto entry : chunkMap){
        int64_t key = entry.first;
        glm::vec2 xzPos = getCoordFromKey(key);
        if(x>xzPos.x && x<xzPos.x+16 && z>xzPos.y && z<xzPos.y+16){
            c = entry.second;
            break;
        }
    }
    return c.setBlockAt(x, y, z, t);
}

void Terrain::CreateTestScene()
{
    // Create the basic terrain floor
    for(int x = 0; x < 64; ++x)
    {
        for(int z = 0; z < 64; ++z)
        {
            for(int y = 127; y < 256; ++y)
            {
                if(y <= 128)
                {
                    if((x + z) % 2 == 0)
                    {
                        m_blocks[x][y][z] = STONE;
                    }
                    else
                    {
                        m_blocks[x][y][z] = DIRT;
                    }
                }
                else
                {
                    m_blocks[x][y][z] = EMPTY;
                }
            }
        }
    }
    // Add "walls" for collision testing
    for(int x = 0; x < 64; ++x)
    {
        m_blocks[x][129][0] = GRASS;
        m_blocks[x][130][0] = GRASS;
        m_blocks[x][129][63] = GRASS;
        m_blocks[0][130][x] = GRASS;
    }
    for(int y = 129; y < 140; ++y)
    {
        m_blocks[32][y][32] = GRASS;
    }
}
