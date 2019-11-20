#include "cube.h"
#include <la.h>
#include <iostream>

static const int CUB_IDX_COUNT = 36;
static const int CUB_VERT_COUNT = 24;

glm::vec4 GetCubeNormal(const glm::vec4& P)
{
    int idx = 0;
    float val = -1;
    for(int i = 0; i < 3; i++){
        if(glm::abs(P[i]) > val){
            idx = i;
            val = glm::abs(P[i]);
        }
    }
    glm::vec4 N(0,0,0,0);
    N[idx] = glm::sign(P[idx]);
    return N;
}

//These are functions that are only defined in this cpp file. They're used for organizational purposes
//when filling the arrays used to hold the vertex and index data.
void createCubeVertexPositions(glm::vec4 (&cub_vert_pos)[CUB_VERT_COUNT])
{
    int idx = 0;
    //Front face
    //UR
    cub_vert_pos[idx++] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //LR
    cub_vert_pos[idx++] = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
    //LLs
    cub_vert_pos[idx++] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    //UL
    cub_vert_pos[idx++] = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);

    //Right face
    //UR
    cub_vert_pos[idx++] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
    //LR
    cub_vert_pos[idx++] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    //LL
    cub_vert_pos[idx++] = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
    //UL
    cub_vert_pos[idx++] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    //Left face
    //UR
    cub_vert_pos[idx++] = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
    //LR
    cub_vert_pos[idx++] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    //LL
    cub_vert_pos[idx++] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    //UL
    cub_vert_pos[idx++] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

    //Back face
    //UR
    cub_vert_pos[idx++] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    //LR
    cub_vert_pos[idx++] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    //LL
    cub_vert_pos[idx++] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    //UL
    cub_vert_pos[idx++] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);

    //Top face
    //UR
    cub_vert_pos[idx++] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
    //LR
    cub_vert_pos[idx++] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //LL
    cub_vert_pos[idx++] = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
    //UL
    cub_vert_pos[idx++] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

    //Bottom face
    //UR
    cub_vert_pos[idx++] = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
    //LR
    cub_vert_pos[idx++] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    //LL
    cub_vert_pos[idx++] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    //UL
    cub_vert_pos[idx++] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
}


void createCubeVertexNormals(glm::vec4 (&cub_vert_nor)[CUB_VERT_COUNT])
{
    int idx = 0;
    //Front
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec4(0,0,1,0);
    }
    //Right
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec4(1,0,0,0);
    }
    //Left
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec4(-1,0,0,0);
    }
    //Back
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec4(0,0,-1,0);
    }
    //Top
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec4(0,1,0,0);
    }
    //Bottom
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec4(0,-1,0,0);
    }
}

void createCubeIndices(GLuint (&cub_idx)[CUB_IDX_COUNT])
{
    int idx = 0;
    for(int i = 0; i < 6; i++){
        cub_idx[idx++] = i*4;
        cub_idx[idx++] = i*4+1;
        cub_idx[idx++] = i*4+2;
        cub_idx[idx++] = i*4;
        cub_idx[idx++] = i*4+2;
        cub_idx[idx++] = i*4+3;
    }
}

void Cube::create()
{
}
