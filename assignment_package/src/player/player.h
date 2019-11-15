#ifndef PLAYER_H
#define PLAYER_H

#include "la.h"
#include "camera.h"
#include "scene/terrain.h"
#include <vector>
#include <QKeyEvent>
#include <QMouseEvent>

class Player
{
private:
    /// Precision for floating point comparison
    float eps = 0.01;

    /// Position of the player in 3D space
    glm::vec3 position;

    /// Current velocity of the player
    glm::vec3 velocity;

    /// Orientation of player up axis
    float theta;

    /// Orientation of player around x axis
    float phi;

    /// Requested move for player in 3D space
    glm::vec3 requestedDirection;

    /// Requested rotation for player around up axis
    float requestedTheta;

    /// Requested rotation for player around x axis
    float requestedPhi;

    /// Pointer to the camera
    Camera* camera;

    /// Pointer to terrain
    Terrain* terrain;

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

    /// Modiefies camera attributes based on on position / orientation
    void cameraUpdate();

    /// Returns if player is currently touching the ground
    bool isGrounded();

    /// Computes the distance to the next collision along travel vector
    float getCollisionDistance(glm::vec3&);

    /// Retunrs the edges of the players bounding box relative to the position
    std::vector<glm::vec3> getBoundingBox();

    /// Used for caching of bounding box
    std::vector<glm::vec3> boundingBox;

public:
    void handleKeyEvent(QKeyEvent*);
    void handleMouseEvent(QMouseEvent*);

    /// Setter for the camera pointer
    void setCamera(Camera*);

    /// Setter for the terrain pointer
    void setTerrain(Terrain*);

    /// Reads present controller state.
    /// Based on the controller state, update the
    /// relevant attributes of the entity.
    void controllerUpdate();

    /// Processes the physics variables of an entity (position, velocity,
    /// acceleration, angular velocity, etc.) and updates all
    /// dependent variables according to the change in time
    void physicsUpdate(float);

    Player();
};

#endif // PLAYER_H
