#include <scene/terrain.h>

#include <scene/cube.h>
#include <iostream>


Terrain::Terrain (OpenGLContext* context) : context(context), dimensions(64, 256, 64),chunkMap()
{
    setMap();
}

void Terrain::setMap(){
    /*
    Chunk c1 = Chunk(context, glm::vec4(0,0,0,0));
    Chunk c2 = Chunk(context, glm::vec4(0,16,16,0));
    Chunk c3 = Chunk(context, glm::vec4(0,16,0,0));
    Chunk c4 = Chunk(context, glm::vec4(0,0,16,0));
    c1.right = &c2;
    c1.back = &c4;
    c2.left = &c1;
    c2.back = &c3;
    c3.front = &c2;
    c3.left = &c4;
    c4.front = &c1;
    c4.right = &c3;
    //keys
    int64_t k1 = Terrain::getHashKey(0, 0);
    int64_t k2 = Terrain::getHashKey(0, 16);
    int64_t k3 = Terrain::getHashKey(16, 0);
    int64_t k4 = Terrain::getHashKey(16, 16);
    chunkMap.insert(std::make_pair(k1, c1));
    chunkMap.insert(std::make_pair(k2, c2));
    chunkMap.insert(std::make_pair(k3, c3));
    chunkMap.insert(std::make_pair(k4, c4));
    */
    Chunk c(context, glm::vec4(0,0,0,0));
    //c.setBlockAt(0,0,0,STONE);
    BlockType t = c.getBlockAt(0,0,0);
    int64_t k1 = Terrain::getHashKey(0, 0);
    chunkMap.insert({k1, c});
}

//x is in lower 32 bits, z is in upper 32 bits
int64_t Terrain::getHashKey(int x, int z){
    int64_t nx = (int64_t)x;
    int64_t nz = (int64_t)z;
    nz = nz << 32;
    nx = nx & 0x00000000ffffffff;
    //std::cout<<"getHashKey:"<<std::endl;
    //std::cout<<"key:"<<(int64_t)(nx | nz)<<" x:"<<x<<" z:"<<z<<std::endl;
    return nx | nz;
}

glm::vec2 Terrain::getCoordFromKey(int64_t key) const{
    int64_t x = key & 0x00000000ffffffff;
    //check if x is negative
    if(x & 2147483648 != 0){
        x = 0xffffffff00000000 | x;
    }
    int64_t z = key>>32;
    //std::cout<<"getCoordFromKey:"<<std::endl;
    //std::cout<<"key:"<<key<<" x:"<<x<<" z:"<<z<<std::endl;
    return(glm::vec2(x, z));
}

BlockType Terrain::getBlockAt(int x, int y, int z) const
{
    //locate which chunk it is in
    //check if the coordinate belong to the chunk
    for(auto entry : chunkMap){
        int64_t key = entry.first;
        glm::vec2 xzPos = getCoordFromKey(key);
        if(x>xzPos.x && x<xzPos.x+16 && z>xzPos.y && z<xzPos.y+16){
            return entry.second.getBlockAt(x, y, z);
        }
    }
}

void Terrain::setBlockAt(int x, int y, int z, BlockType t)
{
    //locate which chunk it is in
    //check if the coordinate belong to the chunk
    for(auto entry : chunkMap){
        int64_t key = entry.first;
        glm::vec2 xzPos = getCoordFromKey(key);
        if(x>xzPos.x && x<xzPos.x+16 && z>xzPos.y && z<xzPos.y+16){
            entry.second.setBlockAt(x, y, z, t);
            break;
        }
    }
}

void Terrain::CreateTestScene()
{
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
                            setBlockAt(x,y,z,STONE);
                        }
                        else
                        {
                            setBlockAt(x,y,z,DIRT);
                        }
                    }
                    else
                    {
                       setBlockAt(x,y,z,EMPTY);
                    }
                }
            }
        }
    //setBlockAt(0,0,0,STONE);
//    // Create the basic terrain floor
//    for(int x = 0; x < 64; ++x)
//    {
//        for(int z = 0; z < 64; ++z)
//        {
//            for(int y = 127; y < 256; ++y)
//            {
//                if(y <= 128)
//                {
//                    if((x + z) % 2 == 0)
//                    {
//                        setBlockAt(x,y,z,STONE);
//                    }
//                    else
//                    {
//                        setBlockAt(x,y,z,DIRT);
//                    }
//                }
//                else
//                {
//                   setBlockAt(x,y,z,EMPTY);
//                }
//            }
//        }
//    }
//    // Add "walls" for collision testing
//    for(int x = 0; x < 64; ++x)
//    {
//        setBlockAt(x,129,0,GRASS);
//        setBlockAt(x,130,0,GRASS);
//        setBlockAt(x,129,63,GRASS);
//        setBlockAt(x,130,x,GRASS);
//    }
//    for(int y = 129; y < 140; ++y)
//    {
//        setBlockAt(32,y,32,GRASS);
//    }
}
