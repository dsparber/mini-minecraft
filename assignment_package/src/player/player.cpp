#include "player.h"
#include "iostream"

Player::Player() :
    camera(nullptr),
    position(28, 130, 28),
    velocity(0, 0, 0),
    orientation(0, 0),
    requestedDirection(0, 0, 0),
    requestedRotation(0, 0),
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
    addBlockPressed(false)
{ }

void Player::setCamera(Camera* c) {
    camera = c;
}


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
    requestedDirection = glm::mat3(glm::rotate(glm::mat4(1.f), orientation[0], glm::vec3(0, 1, 0))) * requestedDirection;
    if (glm::length(requestedDirection) > 0) {
        requestedDirection = glm::normalize(requestedDirection);
    }

    // Compute requested rotation of controller input
    requestedRotation = mouseDelta * glm::radians(1.5f);
}

void Player::physicsUpdate(float dt) {

    bool flying = true || flyPressed;

    float s = 1; // Unit time, referred to as s (second)
    float m = 1; // Unit distance, referred to as m (meter)
    float s2 = s * s; // s^2
    float mps = m / s; // Velocity unit m/s
    float kph = 3.6f * mps; // Velocity unit km/h

    float g = 9.81 * m / s2; // gravitation
    float maxV = 5 * kph; // max. velocity
    float omega = .5 * glm::pi<float>() / s; // default angular velocity

    // Velocity
    float vy = velocity.y;
    velocity = requestedDirection * maxV;
    if (!flying) {
        velocity.y = vy - g * dt; // Apply gravitation
    }

    // Position
    position += velocity * dt;

    // Orientation
    orientation += requestedRotation * omega * dt;
    float pi_2 = glm::pi<float>() * .5f;
    orientation[1] = glm::clamp(orientation[1], -pi_2 + 0.1f, pi_2 - 0.1f);

    // Update camera
    cameraUpdate();
}

void::Player::cameraUpdate() {
    if (camera == nullptr) {
        return;
    }

    camera->eye = position;
    camera->theta = orientation[0];
    camera->phi = orientation[1];
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
