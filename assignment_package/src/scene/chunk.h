#pragma once
#include <QList>
#include <la.h>
#include <drawable.h>
#include "blocktype.h"
#include <array>

// C++ 11 allows us to define the size of an enum. This lets us use only one byte
// of memory to store our different block types. By default, the size of a C++ enum
// is that of an int (so, usually four bytes). This *does* limit us to only 256 different
// block types, but in the scope of this project we'll never get anywhere near that many.
class Chunk: public Drawable
{
public:
    Chunk(OpenGLContext* context);
    Chunk(OpenGLContext* context, glm::vec4 pos);
    Chunk(OpenGLContext* context, const Chunk &Chunk);
    OpenGLContext* mp_context;

    //16 * 256 * 16
    std::array<BlockType, 65536> m_blocks;


    //functions NOT used, just for debugging
    BlockType getBlockAt(int x, int y, int z) const;
    BlockType& getBlockAt(int x, int y, int z);
    void setBlockAt(int x, int y, int z, BlockType t);
    void tempTest();


    glm::vec4 pos;
    Chunk* left;
    Chunk* right;
    Chunk* front;
    Chunk* back;


    void create();
    void drawFace(glm::vec4 pos, std::vector<GLuint>& idx, std::vector<glm::vec4>& all, int faceNum);
    void drawOutFace(glm::vec4 pos, std::vector<GLuint>& idx, std::vector<glm::vec4>& all, int faceNum);



};
