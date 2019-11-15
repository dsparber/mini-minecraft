#include <scene/chunk.h>
#include <scene/cube.h>
#include <iostream>

//initaite global variables

std::vector<glm::vec4> poses =
{
    //front
    glm::vec4(0,1,0,1),
    glm::vec4(0,0,0,1),
    glm::vec4(1,0,0,1),
    glm::vec4(1,1,0,1),
    //right
    glm::vec4(1,1,0,1),
    glm::vec4(1,0,0,1),
    glm::vec4(1,0,1,1),
    glm::vec4(1,1,1,1),
    //back
    glm::vec4(0,1,1,1),
    glm::vec4(0,0,1,1),
    glm::vec4(1,0,1,1),
    glm::vec4(1,1,1,1),
    //left
    glm::vec4(0,1,0,1),
    glm::vec4(0,0,0,1),
    glm::vec4(0,0,1,1),
    glm::vec4(0,1,1,1),
    //bottom
    glm::vec4(0,0,1,1),
    glm::vec4(0,0,0,1),
    glm::vec4(1,0,0,1),
    glm::vec4(1,0,1,1),
    //top
    glm::vec4(0,1,1,1),
    glm::vec4(0,1,0,1),
    glm::vec4(1,1,0,1),
    glm::vec4(1,1,1,1)
};

std::vector<glm::vec4> nors =
{
    //in order of front, right, back, left, bottom, top
    glm::vec4(0,0,1,1),
    glm::vec4(1,0,0,1),
    glm::vec4(0,0,-1,1),
    glm::vec4(-1,0,0,1),
    glm::vec4(0,-1,0,1),
    glm::vec4(0,1,0,1)
};

//a vector of 1 unit offsets in order of : front, right, back, left, bottom, top
std::vector<glm::vec3> offsets =
{
    glm::vec3(0,0,-1),
    glm::vec3(-1,0,0),
    glm::vec3(0,0,1),
    glm::vec3(1,0,0),
    glm::vec3(0,1,0),
    glm::vec3(0,-1,0)
};

Chunk::Chunk(OpenGLContext* context) : Drawable(context), mp_context(context), pos(glm::vec4(0,0,0,0)), left(nullptr), right(nullptr),
    front(nullptr), back(nullptr), m_blocks()
{
    //std::fill(this->m_blocks.begin(), this->m_blocks.end(), EMPTY);
    setBlockAt(0,0,0,STONE);
}

Chunk::Chunk(OpenGLContext* context, glm::vec4 pos): Drawable(context), mp_context(context), pos(pos), left(nullptr), right(nullptr),
    front(nullptr), back(nullptr)
{
    //std::fill(this->m_blocks.begin(), this->m_blocks.end(), EMPTY);
}

BlockType Chunk::getBlockAt(int x, int y, int z) const
{
    if(x < 0 || y < 0 || z < 0
       || x > 16 || y > 256 || z > 16) {
        return EMPTY;
    }
    int idx = 16 * 256 * z + 16 * y + x;
    if(idx < 65536 && idx >= 0){
        return m_blocks[idx];
    }
}

BlockType& Chunk::getBlockAt(int x, int y, int z)
{
    if(x < 0 || y < 0 || z < 0
       || x > 16 || y > 256 || z > 16) {
//        throw std::out
    }
    int idx = 16 * 256 * z + 16 * y + x;
    if(idx < 65536 && idx >= 0){
        return m_blocks[idx];
    }
}

void Chunk::setBlockAt(int x, int y, int z, BlockType t)
{
    int idx = 16 * 256 * z + 16 * y + x;
    if(idx < 65536 && idx >= 0){
        m_blocks[idx] = t;
    }
}

//only create vertex data for block faces that lie on the boundary between an EMPTY block and a filled block.
void Chunk::create(){
    std::cout<<"a chunk is created!"<<std::endl;
    //handle setting up the VBOs for any arbitrary mesh
    std::vector<GLuint> idx;
    std::vector<glm::vec4> all; //all attributes organized as vertex position, normal, and color


    //loop through blocks by iterating through a 3d array that's 16 * 256 * 16
    for(int x = 0; x < 16; x++){
        for(int y = 0; y < 256; y++){
            for(int z = 0; z < 16; z++){

                BlockType b = getBlockAt(x,y,z);
                std::cout<<"blockType: " <<b<<"at"<<x<<y<<z<<std::endl;

                if(b != EMPTY){
                    std::cout<<"draw cube at "<<x<<y<<z<<std::endl;
                    //check all 6 sides
                    for(int i = 0; i < 6; i++){
                        //coordinate of the neighbor
                        glm::vec3 nc = glm::vec3(x,y,z) + offsets[i];
                        //check side, draw if side block is empty and not out of bound
                        if(nc.x >= 0 && nc.y >= 0 && nc.z >= 0 &&
                                nc.x < 16 && nc.y < 256 && nc.z < 16)
                        {
                            BlockType adj = getBlockAt(nc.x, nc.y,nc.z);
                            if(adj == EMPTY) {
                                Chunk::drawFace(glm::vec4(x,y,z,1.f), idx, all, i);
                            }
                        } else {
                            //check adjacent chunks and draw faces if adjacent block is empty
                            Chunk::drawOutFace(glm::vec4(x,y,z,1.f), idx, all, i);
                        }
                    }
                }

            }
        }
    }

    count = idx.size();

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    generateAll();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufAll);
    mp_context->glBufferData(GL_ARRAY_BUFFER, all.size() * sizeof(glm::vec4), all.data(), GL_STATIC_DRAW);
}

void Chunk::drawFace(glm::vec4 pos, std::vector<GLuint>& idx, std::vector<glm::vec4>& all, int faceNum){
    GLuint si; //start index
    if(idx.size()==0){
        si = 0;
    } else {
        si = idx[idx.size()-1] + 1;
    }
    //push back indices of a face
    idx.push_back(si);
    idx.push_back(si+1);
    idx.push_back(si+2);
    idx.push_back(si);
    idx.push_back(si+2);
    idx.push_back(si+3);

    //push back vertex attributes
    for(int i = 0; i < 4; i++){
        //position
        all.push_back(pos+poses[faceNum*4 + i]);
        //normal
        all.push_back(nors[faceNum]);
        //color
        all.push_back(glm::vec4(1,1,1,1));
    }
}

void Chunk::drawOutFace(glm::vec4 pos, std::vector<GLuint>& idx, std::vector<glm::vec4>& all, int faceNum){
    Chunk* adjC = nullptr;
    glm::vec4 adjPos;
    if(faceNum == 0){
        adjC = front;
        adjPos = glm::vec4(pos.x, pos.y, 16, 1);
    } else if(faceNum == 1){
        adjC = right;
        adjPos = glm::vec4(0, pos.y, pos.z, 1);
    } else if(faceNum == 2){
        adjC = back;
        adjPos = glm::vec4(pos.x, pos.y, 0, 1);
    } else if(faceNum == 3){
        adjC = left;
        adjPos = glm::vec4(16.f, pos.y, pos.z, 1);
    } else {
        //if it's on the edge of bottom and top, draw face
        drawFace(pos, idx, all, faceNum);
    }

    if(adjC != nullptr && adjC->getBlockAt(adjPos.x, adjPos.y, adjPos.z) == EMPTY){
        //draw the current face
        drawFace(pos, idx, all, faceNum);
    }

}
