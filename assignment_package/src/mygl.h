#ifndef MYGL_H
#define MYGL_H

#include <openglcontext.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/cube.h>
#include "camera.h"
#include "player/player.h"
#include <scene/terrain.h>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <smartpointerhelp.h>


class MyGL : public OpenGLContext
{
    Q_OBJECT
private:
    uPtr<Cube> mp_geomCube;// The instance of a unit cube we can use to render any cube. Should NOT be used in final version of your project.
    uPtr<ShaderProgram> mp_progLambert;// A shader program that uses lambertian reflection
    uPtr<ShaderProgram> mp_progFlat;// A shader program that uses "flat" reflection (no shadowing at all)

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.

    uPtr<Player> mp_player;
    uPtr<Camera> mp_camera;
    uPtr<Terrain> mp_terrain;
    //uPtr<Chunk> mp_chunk;

    long lastUpdate; // Save when the timer update was last called

    /// Timer linked to timerUpdate(). Fires approx. 60 times per second
    QTimer timer;

    void MoveMouseToCenter(); // Forces the mouse position to the screen's center. You should call this
                              // from within a mouse move event after reading the mouse movement so that
                              // your mouse stays within the screen bounds and is always read.


public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void GLDrawScene();
    void createTexture(const char *texturePath);
    void setTextureSample();


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
