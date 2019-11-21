#pragma once

#include <openglcontext.h>
#include <la.h>

//This defines a class which can be rendered by our shader program.
//Make any geometry a subclass of ShaderProgram::Drawable in order to render it with the ShaderProgram class.
class Drawable
{
protected:
    int count;     // The number of indices stored in bufIdx.
    int nCount;     // The number of indices stored in bufNIdx, for nonOpaque Blocks
    //int count
    GLuint bufIdx; // A Vertex Buffer Object that we will use to store triangle indices (GLuints)
    GLuint bufNIdx;
    GLuint bufOp; //opaque buffer
    GLuint bufNonOp; //

    GLuint textureHandle;


    bool opBound;
    bool nonOpBound;

    bool idxBound; // Set to TRUE by generateIdx(), returned by bindIdx().
    bool nIdxBound;
    bool posBound;
    bool norBound;
    bool colBound;

    OpenGLContext* context; // Since Qt's OpenGL support is done through classes like QOpenGLFunctions_3_2_Core,
                          // we need to pass our OpenGL context to the Drawable in order to call GL functions
                          // from within this class.


public:
    Drawable(OpenGLContext* context);
    virtual ~Drawable();

    virtual void create() = 0; // To be implemented by subclasses. Populates the VBOs of the Drawable.
    void destroy(); // Frees the VBOs of the Drawable.

    // Getter functions for various GL data
    virtual GLenum drawMode();
    int elemCount();
    int elemNCount();

    // Call these functions when you want to call glGenBuffers on the buffers stored in the Drawable
    // These will properly set the values of idxBound etc. which need to be checked in ShaderProgram::draw()
    void generateIdx();
    void generateNIdx();

    void generateOp();
    void generateNonOp();
    void generateTexture();

    bool bindIdx();
    bool bindNIdx();
    bool bindOp();
    bool bindNonOp();
    bool bindTexture();

};
