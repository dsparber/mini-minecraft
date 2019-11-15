#ifndef PLAYER_H
#define PLAYER_H

#include "la.h"
#include "camera.h"
#include <QKeyEvent>
#include <QMouseEvent>

class Player
{
private:
    /// Position of the player in 3D space
    glm::vec3 position;

    /// Current velocity of the player
    glm::vec3 velocity;

    /// Orientation of player, first component: around y, second around z
    glm::vec2 orientation;

    /// Requested move for player in 3D space
    glm::vec3 requestedDirection;

    /// Requested rotation for player in 3D space
    glm::vec2 requestedRotation;

    /// Pointer to the camera
    Camera* camera;

    /// Track pressed buttons
    bool forwardPressed;
    bool backwardPressed;
    bool leftPressed;
    bool rightPressed;
    bool upPressed;
    bool downPressed;
    bool jumpPressed;
    bool flyPressed;
    bool removeBlockPressed;
    bool addBlockPressed;

    /// track the change in the cursor's X and Y coordinate
    glm::vec2 mouseDelta;

public:
    void handleKeyEvent(QKeyEvent*);
    void handleMouseEvent(QMouseEvent*);

    /// Setter for the camera pointer
    void setCamera(Camera*);

    /// Reads present controller state.
    /// Based on the controller state, update the
    /// relevant attributes of the entity.
    void controllerUpdate();

    /// Processes the physics variables of an entity (position, velocity,
    /// acceleration, angular velocity, etc.) and updates all
    /// dependent variables according to the change in time
    void physicsUpdate(float);

    /// Modiefies camera attributes based on on position / orientation
    void cameraUpdate();

    Player();
};

#endif // PLAYER_H
