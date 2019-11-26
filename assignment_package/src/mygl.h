#ifndef MYGL_H
#define MYGL_H

#include <openglcontext.h>
#include <utils.h>
#include <shaderprograms/shaderprogram.h>
#include <shaderprograms/postprocessshader.h>
#include <scene/cube.h>
#include <scene/quad.h>
#include "camera.h"
#include "player/player.h"
#include <scene/terrain.h>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <smartpointerhelp.h>
#include "texture.h"
#include <scene/river.h>


class MyGL : public OpenGLContext
{
    Q_OBJECT
private:

    /// The screen-space quadrangle used to draw
    /// the scene with the post-process shaders.
    Quad mp_geomQuad;

    /// A shader program that uses lambertian reflection
    uPtr<ShaderProgram> mp_progLambert;

    /// A shader program that uses "flat" reflection (no shadowing at all)
    uPtr<ShaderProgram> mp_progFlat;

    /// PostProcessShader for water
    uPtr<PostProcessShader> mp_waterShader;

    /// PostProcessShader for lava
    uPtr<PostProcessShader> mp_lavaShader;

    /// No modification PostProcessShader
    uPtr<PostProcessShader> mp_plainShader;

    /// A handle for our vertex array object. This will store the VBOs created in our geometry classes.
    /// Don't worry too much about this. Just know it is necessary in order to render geometry.
    GLuint vao;

    /// Handles to the frame buffers to perform render passes.
    /// The 0th frame buffer is always written to by the render pass.
    GLuint mp_frameBuffer;

    /// A collection of handles to the depth buffers used by our frame buffers.
    /// m_frameBuffers[i] writes to m_depthRenderBuffers[i].
    GLuint mp_depthRenderBuffer;

    // A collection of handles to the textures used by the frame buffers.
    // m_frameBuffers[i] writes to m_renderedTextures[i].
    GLuint mp_renderedTexture;

    uPtr<Player> mp_player;
    uPtr<Camera> mp_camera;
    uPtr<Terrain> mp_terrain;

    /// Save when the timer update was last called
    long lastUpdate;

    /// Timer linked to timerUpdate(). Fires approx. 60 times per second
    QTimer timer;

    // Time for post process shader
    int mp_time;

    /// Forces the mouse position to the screen's center. You should call this
    /// from within a mouse move event after reading the mouse movement so that
    /// your mouse stays within the screen bounds and is always read.
    void MoveMouseToCenter();

    /// Sets up the arrays of frame buffers used to store render passes.
    /// Invoked once in initializeGL().
    void createRenderBuffers();

    /// Sets up the different shaders used to ender the scene.
    /// Invoked once in initializeGL().
    void createShaders();

    /// A helper function that iterates through each of the render passes
    /// required by the currently bound post-process shader and invokes them.
    void performPostprocessRenderPass();

    void GLDrawScene();

    void createTextures();

    void setTextureSample();

    std::shared_ptr<Texture> m_texture;
    Texture* mp_currentTex;


public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

protected:
    void keyReleaseEvent(QKeyEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent*);

private slots:
    /// Slot that gets called ~60 times per second
    void timerUpdate();
};


#endif // MYGL_H
