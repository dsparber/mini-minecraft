#ifndef TEXTURE_H
#define TEXTURE_H


#include <openglcontext.h>
#include <la.h>
#include <memory>
#include <iostream>

class Texture
{
public:
    Texture(OpenGLContext* context);
    ~Texture();

    void create(const char *texturePath);
    void load(int texSlot);
    void bind(int texSlot);
    GLuint m_textureHandle;
private:
    OpenGLContext* context;

    std::shared_ptr<QImage> m_textureImage;
};


#endif // TEXTURE_H
