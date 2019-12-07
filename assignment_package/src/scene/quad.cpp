#include "quad.h"

Quad::Quad(OpenGLContext *context) : Drawable(context)
{}

void Quad::create()
{
    float z = 0.99999f;
    GLuint idx[6]{0, 1, 2, 0, 2, 3};
    glm::vec4 vert_pos[4] {glm::vec4(-1.f, -1.f, z, 1.f),
                           glm::vec4(1.f, -1.f, z, 1.f),
                           glm::vec4(1.f, 1.f,z, 1.f),
                           glm::vec4(-1.f, 1.f, z, 1.f)};

    glm::vec2 vert_UV[4] {glm::vec2(0.f, 0.f),
                          glm::vec2(1.f, 0.f),
                          glm::vec2(1.f, 1.f),
                          glm::vec2(0.f, 1.f)};

    std::vector<glm::vec4> all;

    for (int i = 0; i < 4; i++) {
        all.push_back(vert_pos[i]);
        all.push_back(glm::vec4(0)); // Normal
        all.push_back(glm::vec4(1, 0, 0, 1)); // Color
        all.push_back(glm::vec4(vert_UV[i], 0, 0));
    }

    nCount = 6;

    // Create a VBO on our GPU and store its handle in bufIdx
    generateNIdx();
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufNIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // CYL_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), idx, GL_STATIC_DRAW);


    generateNonOp();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufNonOp);
    context->glBufferData(GL_ARRAY_BUFFER, all.size() * sizeof(glm::vec4), all.data(), GL_STATIC_DRAW);
}
