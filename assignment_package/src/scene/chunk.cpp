#include <scene/chunk.h>
#include <scene/cube.h>
#include <iostream>

//initaite global variables

std::vector<glm::vec4> poses =
{
    //front
    glm::vec4(0,1,0,0),
    glm::vec4(0,0,0,0),
    glm::vec4(1,0,0,0),
    glm::vec4(1,1,0,0),
    //right
    glm::vec4(1,1,0,0),
    glm::vec4(1,0,0,0),
    glm::vec4(1,0,1,0),
    glm::vec4(1,1,1,0),
    //back
    glm::vec4(1,1,1,0),
    glm::vec4(1,0,1,0),
    glm::vec4(0,0,1,0),
    glm::vec4(0,1,1,0),
    //left
    glm::vec4(0,1,1,0),
    glm::vec4(0,0,1,0),
    glm::vec4(0,0,0,0),
    glm::vec4(0,1,0,0),
    //bottom
    glm::vec4(0,0,0,0),
    glm::vec4(0,0,1,0),
    glm::vec4(1,0,1,0),
    glm::vec4(1,0,0,0),
    //top
    glm::vec4(0,1,1,0),
    glm::vec4(0,1,0,0),
    glm::vec4(1,1,0,0),
    glm::vec4(1,1,1,0)
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
    glm::vec3(1,0,0),
    glm::vec3(0,0,1),
    glm::vec3(-1,0,0),
    glm::vec3(0,-1,0),
    glm::vec3(0,1,0)
};

//a vector of 1 unit offsets in order of : front, right, back, left, bottom, top
std::vector<glm::vec2> offsetUVs =
{
    glm::vec2(0,1),
    glm::vec2(0,0),
    glm::vec2(1,0),
    glm::vec2(1,1)

};

Chunk::Chunk(OpenGLContext* context) : Drawable(context), mp_context(context), pos(glm::vec4(0,0,0,0)), left(nullptr), right(nullptr),
    front(nullptr), back(nullptr), m_blocks(), created(false), mutex()
{
    std::fill(this->m_blocks.begin(), this->m_blocks.end(), EMPTY);
}

Chunk::Chunk(OpenGLContext* context, glm::vec4 pos): Drawable(context), mp_context(context), pos(pos), left(nullptr), right(nullptr),
    front(nullptr), back(nullptr), m_blocks(), created(false), mutex()
{
    std::fill(this->m_blocks.begin(), this->m_blocks.end(), EMPTY);
}

Chunk::Chunk(OpenGLContext* context, const Chunk& c): Drawable(context), mp_context(context), pos(c.pos), left(c.left), right(c.right),
    front(c.front), back(c.back), m_blocks(c.m_blocks), created(false), mutex()
{
    std::fill(this->m_blocks.begin(), this->m_blocks.end(), EMPTY);
}

BlockType Chunk::getBlockAt(int x, int y, int z) const
{
    if(x < 0 || y < 0 || z < 0
            || x >= 16 || y >= 256 || z >= 16) {
        return EMPTY;
    }
    int idx = 16 * 256 * x + 16 * y + z;
    return m_blocks[idx];
}

BlockType& Chunk::getBlockAt(int x, int y, int z)
{
    int idx = 16 * 256 * x + 16 * y + z;
    return m_blocks[idx];
}

void Chunk::setBlockAt(int x, int y, int z, BlockType t)
{
    int idx = 16 * 256 * x + 16 * y + z;
    if(idx < 65536 && idx >= 0){
        m_blocks[idx] = t;
    }
}

//only create vertex data for block faces that lie on the boundary between an EMPTY block and a filled block.
void Chunk::compute(){

    //handle setting up the VBOs for any arbitrary mesh
    std::vector<GLuint> idx;
    std::vector<GLuint> nIdx;
    std::vector<glm::vec4> op; //opaque attributes organized as vertex position, normal, and color
    std::vector<glm::vec4> nonOp; //non-opaque attributes organized as vertex position, normal, and color

    int siOp = 0;
    int siNonOp = 0;
    //loop through blocks by iterating through a 3d array that's 16 * 256 * 16
    for(int x = 0; x < 16; x++){
        for(int y = 0; y < 256; y++){
            for(int z = 0; z < 16; z++){

                BlockType b = getBlockAt(x,y,z);
                if(b != EMPTY){

                    //check all 6 sides
                    for(int i = 0; i < 6; i++){
                        //coordinate of the neighbor
                        glm::vec3 nc = glm::vec3(x,y,z) + offsets[i];
                        //check side, draw if side block is empty and not out of bound
                        if(nc.x >= 0 && nc.y >= 0 && nc.z >= 0 &&
                                nc.x < 16 && nc.y < 256 && nc.z < 16)
                        {
                            BlockType adj = getBlockAt(nc.x, nc.y,nc.z);
                            if(isOpaque(b) && (!isOpaque(adj))) {
                                Chunk::drawFace(glm::vec4(x,y,z,1.f), idx, op, i, siOp);
                                //printf("solid face drawn!\n");
                            } else if (!isOpaque(b) && adj == EMPTY){
                                Chunk::drawFace(glm::vec4(x,y,z,1.f), nIdx, nonOp, i, siNonOp);
                                //printf("opaque face drawn!\n");
                            }
                        } else {
                            if (isOpaque(b)){
                                Chunk::drawOutFace(glm::vec4(x,y,z,1.f), idx, op, i, siOp, true);
                                //printf("outface drawn!\n");
                            } else {
                                //check adjacent chunks and draw faces if adjacent block is empty
                                Chunk::drawOutFace(glm::vec4(x,y,z,1.f), nIdx, nonOp, i, siNonOp, false);
                            }
                        }
                    }
                }

            }
        }
    }

    // Update buffer
    mutex.lock();
    this->idx = idx;
    this->nIdx = nIdx;
    this->op = op;
    this->nonOp = nonOp;
    count = idx.size();
    nCount = nIdx.size();
    mutex.unlock();
    created = false;
}

void Chunk::create() {
    if (created) {
        return;
    }

    mutex.lock();
    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    generateNIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufNIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIdx.size() * sizeof(GLuint), nIdx.data(), GL_STATIC_DRAW);

    generateOp();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufOp);
    mp_context->glBufferData(GL_ARRAY_BUFFER, op.size() * sizeof(glm::vec4), op.data(), GL_STATIC_DRAW);

    generateNonOp();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufNonOp);
    mp_context->glBufferData(GL_ARRAY_BUFFER, nonOp.size() * sizeof(glm::vec4), nonOp.data(), GL_STATIC_DRAW);
    mutex.unlock();

    created = true;
}

void Chunk::destroy() {
    Drawable::destroy();
    created = false;

}

void Chunk::drawFace(glm::vec4 pos, std::vector<GLuint>& idx, std::vector<glm::vec4>& all, int faceNum, int& si){
    //    GLuint si; //start index
    //    if(idx.size()==0){
    //        si = 0;
    //    } else {
    //        si = idx[idx.size()-1] + 1;
    //    }
    //push back indices of a face
    idx.push_back(si);
    idx.push_back(si+1);
    idx.push_back(si+2);
    idx.push_back(si);
    idx.push_back(si+2);
    idx.push_back(si+3);
    si += 4;

    //push back vertex attributes
    for(int i = 0; i < 4; i++){
        //position
        all.push_back(pos+poses[faceNum*4 + i]);
        //normal
        all.push_back(nors[faceNum]);
        //a vec4 of uv(vec2), cosine power(float), animatable(float, 1 is animatable)
        BlockType t = getBlockAt(pos.x,pos.y,pos.z);
        if(t != EMPTY)
        {
            switch(t)
            {
            case DIRT:
                all.push_back(glm::vec4(121.f, 85.f, 58.f, 255.f) / 255.f);
                all.push_back((glm::vec4((2 + offsetUVs[i].x)/16.f,
                                         (15 + offsetUVs[i].y)/16.f,
                                         4, 0.f)));
                break;
            case GRASS:
                all.push_back(glm::vec4(95.f, 159.f, 53.f, 255.f) / 255.f);

                if(faceNum == 5){
                    //push back grass top if its top face
                all.push_back((glm::vec4((8 + offsetUVs[i].x)/16.f,
                                         (13 + offsetUVs[i].y)/16.f,
                                         3, 0.f)));
                } else if (faceNum == 4){
                    all.push_back((glm::vec4((2 + offsetUVs[i].x)/16.f,
                                             (15 + offsetUVs[i].y)/16.f,
                                             3, 0.f)));
                } else {
                    all.push_back((glm::vec4((3 + offsetUVs[i].x)/16.f,
                                             (15 + offsetUVs[i].y)/16.f,
                                             3, 0.f)));
                }
                break;
            case HILLGRASS:
                all.push_back(glm::vec4(95.f, 159.f, 53.f, 255.f) / 255.f);
                all.push_back((glm::vec4((8 + offsetUVs[i].x)/16.f,
                                         (13 + offsetUVs[i].y)/16.f,
                                         3, 0.f)));
                break;
            case STONE:
                all.push_back(glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
                all.push_back((glm::vec4((1 + offsetUVs[i].x)/16.f,
                                         (15 + offsetUVs[i].y)/16.f,
                                         5, 0.f)));
                break;
            case ICE:
                all.push_back(glm::vec4(0.f, 0.f, 0.f, 0.5f));
                all.push_back((glm::vec4((3 + offsetUVs[i].x)/16.f,
                                         (11 + offsetUVs[i].y)/16.f,
                                         5, 0.f)));
                break;
            case SNOW:
                all.push_back(glm::vec4(0.f, 0.f, 0.f, 1.f));
                all.push_back((glm::vec4((2 + offsetUVs[i].x)/16.f,
                                         (11 + offsetUVs[i].y)/16.f,
                                         0.3, 0.f)));
                break;
            case BEDROCK:
                all.push_back(glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
                all.push_back((glm::vec4((2 + offsetUVs[i].x)/16.f,
                                         (14 + offsetUVs[i].y)/16.f,
                                         5, 0.f)));
                break;
            case LAVA:
                all.push_back(glm::vec4(1, 0, 0, 0.5));
                all.push_back((glm::vec4((14 + offsetUVs[i].x)/16.f,
                                         (1 + offsetUVs[i].y)/16.f,
                                         2, 1.f)));
                break;
            case WATER:
                all.push_back(glm::vec4(.2, .5, 1, 1.f));
                all.push_back((glm::vec4((14 + offsetUVs[i].x)/16,
                                         (3 + offsetUVs[i].y)/16,
                                         1, 1.f)));
                break;
            case SAND:
                all.push_back(glm::vec4(0.f, 0.f, 0.f, 1.f));
                all.push_back((glm::vec4((2 + offsetUVs[i].x)/16,
                                         (14 + offsetUVs[i].y)/16,
                                         1, 0.f)));
                break;
            case DARKSTONE:
                all.push_back(glm::vec4(0.f, 0.f, 0.f, 1.f));
                all.push_back((glm::vec4((5 + offsetUVs[i].x)/16,
                                         (13 + offsetUVs[i].y)/16,
                                         1, 0.f)));
                break;
            case VOLCANO:
                all.push_back(glm::vec4(0.f, 0.f, 0.f, 1.f));
                all.push_back((glm::vec4((1 + offsetUVs[i].x)/16,
                                         (8 + offsetUVs[i].y)/16,
                                         1, 0.f)));
                break;
            case BARK:
                all.push_back(glm::vec4(0.f, 0.f, 0.f, 1.f));
                all.push_back((glm::vec4((6 + offsetUVs[i].x)/16,
                                         (10 + offsetUVs[i].y)/16,
                                         1, 0.f)));
                break;
            case LEAF:
                all.push_back(glm::vec4(0.f, 0.f, 0.f, 1.f));
                all.push_back((glm::vec4((4 + offsetUVs[i].x)/16,
                                         (12 + offsetUVs[i].y)/16,
                                         1, 0.f)));
                break;
            case DARKLEAF:
                all.push_back(glm::vec4(0.f, 0.f, 0.f, 1.f));
                all.push_back((glm::vec4((5 + offsetUVs[i].x)/16,
                                         (12 + offsetUVs[i].y)/16,
                                         1, 0.f)));
                break;
            default:
                // Other types are as of yet not define
                all.push_back(glm::vec4(0));
                all.push_back(glm::vec4(0));
                break;
            }
        }
    }
}

void Chunk::drawOutFace(glm::vec4 pos, std::vector<GLuint>& idx, std::vector<glm::vec4>& all, int faceNum, int& si, bool opaque){
    Chunk* adjC = nullptr;
    glm::vec4 adjPos;
    if(faceNum == 0){
        adjC = front;
        adjPos = glm::vec4(pos.x, pos.y, 15, 1);
    } else if(faceNum == 1){
        adjC = right;
        adjPos = glm::vec4(0, pos.y, pos.z, 1);
    } else if(faceNum == 2){
        adjC = back;
        adjPos = glm::vec4(pos.x, pos.y, 0, 1);
    } else if(faceNum == 3){
        adjC = left;
        adjPos = glm::vec4(15, pos.y, pos.z, 1);
    }

    BlockType adj = (adjC == nullptr) ? EMPTY : adjC->getBlockAt(adjPos.x, adjPos.y, adjPos.z);
    if((opaque && !isOpaque(adj)) || (!opaque && (isOpaque(adj) || adj == EMPTY))){
        //draw the current face
        drawFace(pos, idx, all, faceNum, si);
    }
}

void Chunk::update() {
    destroy();
    compute();
    create();
}
