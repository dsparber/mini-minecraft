#include "player.h"
#include "iostream"

Player::Player() :
    camera(nullptr),
    position(28, 140, 28.f),
    velocity(0, 0, 0),
    phi(0),
    theta(0),
    requestedDirection(0, 0, 0),
    requestedTheta(0),
    requestedPhi(0),
    mouseDelta(0, 0),
    forwardPressed(false),
    backwardPressed(false),
    leftPressed(false),
    rightPressed(false),
    upPressed(false),
    downPressed(false),
    jumpPressed(false),
    flyPressed(false),
    removeBlockPressed(false),
    addBlockPressed(false),
    boundingBox()
{ }

void Player::controllerUpdate() {
    // Compute the direction of the controller input
    requestedDirection = glm::vec3(0, 0, 0);
    if (forwardPressed) { requestedDirection.z += 1; }
    if (backwardPressed) { requestedDirection.z -= 1; }
    if (leftPressed) { requestedDirection.x += 1; }
    if (rightPressed) { requestedDirection.x -= 1; }
    if (jumpPressed) { requestedDirection.y += 1; }
    if (flyPressed) {
        if (upPressed) { requestedDirection.y += 1; }
        if (downPressed) { requestedDirection.y -= 1; }
    }
    requestedDirection = glm::mat3(glm::rotate(glm::mat4(1.f), theta, glm::vec3(0, 1, 0))) * requestedDirection;
    if (glm::length(requestedDirection) > 0) {
        requestedDirection = glm::normalize(requestedDirection);
    }

    // Compute requested rotation of controller input
    requestedTheta = mouseDelta.x * glm::radians(1.5f);
    requestedPhi = mouseDelta.y * glm::radians(1.5f);
}

void Player::physicsUpdate(float dt) {

    bool flightMode = flyPressed;
    bool grounded = isGrounded();

    float pi_2 = glm::pi<float>() * .5f;

    float s = 1; // Unit time, referred to as s (second)
    float m = 1; // Unit distance, referred to as m (meter)
    float s2 = s * s; // s^2
    float mps = m / s; // Velocity unit m/s
    float kph = mps / 3.6f; // Velocity unit km/h

    float g = 9.81 * m / s2; // gravitation
    float maxV = 12 * kph; // max. velocity
    float omega = pi_2 / s; // default angular velocity

    float jumpHeigth = 1 * m; // Jump 1 m into the air
    float jumpTakeofVelocity = glm::sqrt(2 * g * jumpHeigth) * mps; // v = sqrt(mgh) [m/s]

    // Velocity 
    float vy = velocity.y;
    // Move with max. velocity in requested direction
    velocity = requestedDirection * maxV;
    if (!flightMode) {
        // If grounded -> able to jump, else: falling
        velocity.y = grounded & requestedDirection.y > 0 ? jumpTakeofVelocity : vy;
        // Apply gravitation
        velocity.y -= g * dt;
        if (grounded) {
            // If player on ground, no downwards velocity
            velocity.y = glm::max(0.f, velocity.y);
        }
    }

    // Calculate travel distance and collision distance
    glm::vec3 ds = velocity * dt; // delta s, distance that would be covered by moving v * dt
    float travelDistance = glm::length(ds);
    float collisionDistance = getCollisionDistance(ds);

    // Move at most by collision distance
    if (collisionDistance < travelDistance) {
        ds = ds / travelDistance * collisionDistance;
    }

    // Position update
    position += ds;

    // Orientation update
    theta += requestedTheta * omega * dt;
    phi = glm::clamp(phi + requestedPhi * omega * dt, -pi_2 + .1f, pi_2 - .1f);

    // Update camera
    cameraUpdate();
}

float Player::getCollisionDistance(glm::vec3& ds) {

    float tMax = glm::length(ds);

    // TODO

    return tMax;
}


bool Player::isGrounded() {

    /*
    // Blocks are only at integer positions
    int y = glm::round(position.y);

    // If position.y not an integer -> trivially not grounded
    if (glm::abs(position.y - y) > eps) {
        return false;
    }
    */

    // Check if any of the corners of the bounding box is on a block
    for (glm::vec3 offset : getBoundingBox()) {
        glm::vec3 pos = position + offset;
        int x = (int) pos.x;
        int z = (int) pos.z;
        int y = (int) pos.y;

        // If currently on non empty block
        if (terrain->getBlockOrEmpty(x, y - 1, z) != EMPTY) {
            return true;
        }
    }

    return false;
}


std::vector<glm::vec3> Player::getBoundingBox() {

    if (boundingBox.size() == 8) {
        return boundingBox;
    }

    std::vector<glm::vec3> box;

    // The position of the player is the position of the eyes.
    // The eyes are at the center of the upper cube

    // Bottom
    box.push_back(glm::vec3(.5f,  -1.5f,  .5f));
    box.push_back(glm::vec3(.5f,  -1.5f, -.5f));
    box.push_back(glm::vec3(-.5f, -1.5f,  .5f));
    box.push_back(glm::vec3(-.5f, -1.5f, -.5f));

    // Top
    box.push_back(glm::vec3(.5f,  .5f,  .5f));
    box.push_back(glm::vec3(.5f,  .5f, -.5f));
    box.push_back(glm::vec3(-.5f, .5f,  .5f));
    box.push_back(glm::vec3(-.5f, .5f, -.5f));

    boundingBox = box;
    return boundingBox;
}

void Player::cameraUpdate() {
    if (camera == nullptr) {
        return;
    }

    camera->eye = position;
    camera->theta = theta;
    camera->phi = phi;
    camera->RecomputeAttributes();
}

void Player::handleKeyEvent(QKeyEvent* e) {
    bool pressed = e->type() == QEvent::KeyPress;

    if (e->key() == Qt::Key_W) {
        forwardPressed = pressed;
    }
    if (e->key() == Qt::Key_A) {
        leftPressed = pressed;
    }
    if (e->key() == Qt::Key_S) {
        backwardPressed = pressed;
    }
    if (e->key() == Qt::Key_D) {
        rightPressed = pressed;
    }
    if (e->key() == Qt::Key_E) {
        upPressed = pressed;
    }
    if (e->key() == Qt::Key_Q) {
        downPressed = pressed;
    }
    if (e->key() == Qt::Key_F) {
        flyPressed = pressed;
    }
    if (e->key() == Qt::Key_Space) {
        jumpPressed = pressed;
    }
}

void Player::handleMouseEvent(QMouseEvent* e) {
    // Compute distance to center of mygl
    float x0 = camera->width / 2.f;
    float y0 = camera->height / 2.f;
    mouseDelta = glm::vec2(e->x() - x0, e->y() - y0);
}

void Player::setCamera(Camera* c) {
    camera = c;
}

void Player::setTerrain(Terrain* t) {
    terrain = t;
}
