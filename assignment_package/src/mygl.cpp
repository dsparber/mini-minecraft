#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <QDateTime>


MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
      mp_geomCube(mkU<Cube>(this)),
      mp_progLambert(mkU<ShaderProgram>(this)),
      mp_progFlat(mkU<ShaderProgram>(this)),
      mp_camera(mkU<Camera>()),
      mp_player(mkU<Player>()),
      mp_terrain(mkU<Terrain>(this)),
      lastUpdate(0)
{
    // Connect the timer to a function so that when the timer ticks the function is executed
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    // Tell the timer to redraw 60 times per second
    timer.start(16);
    setFocusPolicy(Qt::ClickFocus);

    mp_player->setCamera(mp_camera.get());
    mp_player->setTerrain(mp_terrain.get());

    setMouseTracking(true); // MyGL will track the mouse's movements even if a mouse button is not pressed
    setCursor(Qt::BlankCursor); // Make the cursor invisible
}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    mp_geomCube->destroy();
}


void MyGL::MoveMouseToCenter()
{
    QCursor::setPos(this->mapToGlobal(QPoint(width() / 2, height() / 2)));
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.37f, 0.74f, 1.0f, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);


    // Create and set up the diffuse shader
    mp_progLambert->create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    mp_progFlat->create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    // Set a color with which to draw geometry since you won't have one
    // defined until you implement the Node classes.
    // This makes your geometry render green.
    mp_progLambert->setGeometryColor(glm::vec4(0,1,0,1));

    //Create the instance of Cube
    //mp_geomCube->create();

    // Initializes the terrain
    mp_terrain->initialize();

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    //    vao.bind();
    glBindVertexArray(vao);

    printGLErrorLog();
}

void MyGL::resizeGL(int w, int h)
{
    mp_camera->width = w;
    mp_camera->height = h;
    glm::mat4 viewproj = mp_camera->getViewProj();

    MoveMouseToCenter();
    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    mp_progLambert->setViewProjMatrix(viewproj);
    mp_progFlat->setViewProjMatrix(viewproj);
    printGLErrorLog();
}


// MyGL's constructor links timerUpdate() to a timer that fires 60 times per second.
// We're treating MyGL as our game engine class, so we're going to use timerUpdate
void MyGL::timerUpdate()
{
    // First call to timer update
    if (lastUpdate == 0) {
        lastUpdate = QDateTime::currentMSecsSinceEpoch();
        return;
    }

    // Compute dt
    long dt = QDateTime::currentMSecsSinceEpoch() - lastUpdate;
    lastUpdate += dt;

    // Update player
    mp_player->controllerUpdate();
    mp_player->physicsUpdate(dt / 1000.f);

    // Call update on mygl
    update();
}

// This function is called whenever update() is called.
// MyGL's constructor links update() to a timer that fires 60 times per second,
// so paintGL() called at a rate of 60 frames per second.
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mp_progFlat->setViewProjMatrix(mp_camera->getViewProj());
    mp_progLambert->setViewProjMatrix(mp_camera->getViewProj());

    GLDrawScene();

}

void MyGL::GLDrawScene()
{
    for(Chunk* c : mp_terrain->getChunksToDraw()){
        mp_progLambert->setModelMatrix(glm::translate(glm::mat4(), glm::vec3(c->pos)));
        mp_progLambert->draw(*c);
    }
}

void MyGL::createTexture(const char *texturePath)
{
    printGLErrorLog();

    QImage img(texturePath);
    img.convertToFormat(QImage::Format_ARGB32);
    img = img.mirrored();
//    m_textureImage = std::make_shared<QImage>(img);
//    context->glGenTextures(1, &m_textureHandle);
    //this->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, img.bits());

    printGLErrorLog();
}

void MyGL::setTextureSample(){
// Set the render settings for the texture we've just created.
// Essentially zero filtering on the "texture" so it appears exactly as rendered
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
// Clamp the colors at the edge of our texture
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void MyGL::keyPressEvent(QKeyEvent *e)
{
    mp_player->handleKeyEvent(e);
}

void MyGL::keyReleaseEvent(QKeyEvent *e)
{
    mp_player->handleKeyEvent(e);
}

void MyGL::mouseMoveEvent(QMouseEvent *e) {
    mp_player->handleMouseEvent(e);
    MoveMouseToCenter();
}

void MyGL::mousePressEvent(QMouseEvent *e) {
    mp_player->handleMouseEvent(e);

    if (e->buttons() == Qt::RightButton) {
        mp_terrain->addBlock(mp_camera->eye, mp_camera->look);
    } else if (e->buttons() == Qt::LeftButton) {
        mp_terrain->removeBlock(mp_camera->eye, mp_camera->look);
    }
}

