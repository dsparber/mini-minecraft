#include "drawable.h"
#include <la.h>

Drawable::Drawable(OpenGLContext* context)
    : bufIdx(), bufOp(), bufNonOp(),
      idxBound(false), opBound(false), nonOpBound(false), context(context)
{}

Drawable::~Drawable()
{}


void Drawable::destroy()
{
    context->glDeleteBuffers(1, &bufIdx);
    context->glDeleteBuffers(1, &bufOp);
    context->glDeleteBuffers(1, &bufNonOp);
}

GLenum Drawable::drawMode()
{
    // Since we want every three indices in bufIdx to be
    // read to draw our Drawable, we tell that the draw mode
    // of this Drawable is GL_TRIANGLES

    // If we wanted to draw a wireframe, we would return GL_LINES

    return GL_TRIANGLES;
}

int Drawable::elemCount()
{
    return count;
}

void Drawable::generateIdx()
{
    idxBound = true;
    // Create a VBO on our GPU and store its handle in bufIdx
    context->glGenBuffers(1, &bufIdx);
}


void Drawable::generateOp()
{
    opBound = true;
    // Create a VBO on our GPU and store its handle in bufAll
    context->glGenBuffers(1, &bufOp);
}

void Drawable::generateNonOp()
{
    nonOpBound = true;
    // Create a VBO on our GPU and store its handle in bufAll
    context->glGenBuffers(1, &bufNonOp);
}

void Drawable::generateTexture()
{
    context->printGLErrorLog();
    // Create a VBO on our GPU and store its handle in bufAll
    context->glGenTextures(1, &textureHandle);
    context->glActiveTexture(GL_TEXTURE0);
    context->glBindTexture(GL_TEXTURE_2D, textureHandle);
    unsigned char* image;// = loadImage();
//  context->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
//                          image->width(), image->height(),
//                          0, GL_BGRA, GL_UNSIGNED_BYTE, image);
    context->printGLErrorLog();
}

bool Drawable::bindIdx()
{
    if(idxBound) {
        context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    }
    return idxBound;
}

bool Drawable::bindOp()
{
    if(opBound) {
        context->glBindBuffer(GL_ARRAY_BUFFER, bufOp);
    }
    return opBound;
}

bool Drawable::bindNonOp()
{
    if(nonOpBound) {
        context->glBindBuffer(GL_ARRAY_BUFFER, bufNonOp);
    }
    return nonOpBound;
}
