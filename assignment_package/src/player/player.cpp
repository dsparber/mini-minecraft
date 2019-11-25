#include "player.h"
#include "iostream"

Player::Player() :
    camera(nullptr),
    position(0, 160, 0),
    velocity(0, 0, 0),
    phi(0),
    theta(0),
    requestedDirection(0, 0, 0),
    requestedTheta(0),
    requestedPhi(0),
    mouseDelta(0, 0),
    lastMousePos(-1, -1),
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
    bool swimming = isSwimming();
    bool airborne = !grounded && !swimming;

    float pi_2 = glm::pi<float>() * .5f;

    float s = 1; // Unit time, referred to as s (second)
    float m = 1; // Unit distance, referred to as m (meter)
    float s2 = s * s; // s^2
    float mps = m / s; // Velocity unit m/s
    float kph = mps / 3.6f; // Velocity unit km/h

    float speedModifier = swimming ? .67 : 1; // Changes acceleration and velocity

    float g = 9.81 * speedModifier * m / s2; // gravitation
    float defaultAcceleration = 5 * speedModifier * m / s2; // Player acceleration, for moving
    float maxVelocity = 15 * speedModifier * kph; // max. velocity
    float flightVelocity = 30 * kph; // velocity for flying
    float omega = pi_2 / s; // default angular velocity


    float jumpHeigth = 2 * m; // Jump h meters into the air
    float jumpTakeofVelocity = glm::sqrt(2 * g * jumpHeigth) * mps; // v = sqrt(mgh) [m/s]

    // Orientation update
    theta += requestedTheta * omega * dt;
    phi = glm::clamp(phi + requestedPhi * omega * dt, -pi_2 + .1f, pi_2 - .1f);

    // Handle flight mode
    if (flightMode) {
        velocity = requestedDirection * flightVelocity;
        position += velocity * dt; // Position update
        terrain->playerMoved(position); // Tell terrain that player moved
        cameraUpdate(); // Update camera
        return;
    }

    // Acceleration vector
    glm::vec3 a;
    if (airborne) {
        // Gravity only
        a = glm::vec3(0, -g, 0);
    }
    else if (grounded || swimming) {
        // Stop
        if (glm::length(requestedDirection) == 0 && grounded) {
            a = -defaultAcceleration * velocity;
        }
        // Accelerate
        else {
            // Discard y if standing on ground
            glm::vec3 requested = glm::vec3(requestedDirection.x, 0, requestedDirection.z);
            a = requested * defaultAcceleration;
            if (swimming) {
                a.y = requestedDirection.y * (defaultAcceleration + g) - g;
            }
        }
        // Slow down if max velocity violated
        if (glm::length(velocity) > maxVelocity) {
            a -= velocity * defaultAcceleration;
        }
    }

    // Velocity 
    glm::vec3 dv = a * dt; // Delta velocity
    velocity += dv;

    if (grounded) {
        // Handle jump via velocity instead of acceleration for simplicity
        if (requestedDirection.y > 0) {
            velocity.y = jumpTakeofVelocity;
        }
        // If player on ground, no downwards velocity
        velocity.y = glm::max(0.f, velocity.y);
    }


    // Calculate travel distance and collision distance
    glm::vec3 ds = velocity * dt; // delta s, distance that would be covered by moving v * dt
    float travelDistance = glm::length(ds);
    float collisionDistance = getCollisionDistance(ds);

    // Move at most by collision distance
    if (collisionDistance < travelDistance) {
        // Adjust velocity and recalculate ds
        velocity = velocity / travelDistance * collisionDistance * .5f;
        ds = velocity * dt;
    }

    // Position update
    position += ds; 

    // Tell terrain that player moved
    if (glm::length(ds) > eps) {
        terrain->playerMoved(position);
    }

    // Update camera
    cameraUpdate();
}

float Player::getCollisionDistance(glm::vec3 ds) {

    // Setup
    float tMax = glm::length(ds);
    float tMin = tMax;

    // Max == 0 -> return
    if (tMax == 0) {
        return 0;
    }

    // Get signs
    glm::vec3 s = glm::sign(ds);

    // Unit direction
    glm::vec3 u = glm::normalize(ds);

    for (glm::vec3 offset : getBoundingBox()) {

        // Initialize starting point
        glm::vec3 p = position + offset;
        float t = 0;

        // Keep track of first iteration
        bool firstIteration = true;

        // While max. not reached
        while (t < tMax) {

            // Current cube
            glm::ivec3 cube = (glm::ivec3) glm::floor(p);

            // Get nearest intersection
            float intersectionDistance = 1000;
            glm::vec3 intersection;
            glm::ivec3 block;

            // For every coordinate i in [x,y,z]
            for (int i = 0; i < 3; i++) {

                // i component of vector is zero -> no i intersection
                if (u[i] == 0) {
                    continue;
                }

                float pi = cube[i] + s[i]; // position_[x/y/z] of intersection
                float di = glm::abs(p[i] - pi); // distance in i direction

                // First iteration started exactly on border
                if (firstIteration && di == 1) {
                    di = 0;
                }
                // Do not skip cells
                if (glm::abs(di) > 1) {
                    di -= 1;
                }
                // position of intersection
                glm::vec3 pIntersect = p + di / glm::abs(u[i]) * u;

                // Calculate distance
                float d = glm::length(p - pIntersect);

                // Update intersection if smallest distance
                if (d < intersectionDistance) {
                    intersectionDistance = d;
                    intersection = pIntersect;

                    // Block correctetion based on sign
                    glm::ivec3 blockCorrection = glm::ivec3(((i == 0 && s[i] == -1) ? -1 : 0),
                                                            ((i == 1 && s[i] == -1) ? -1 : 0),
                                                            ((i == 2 && s[i] == -1) ? -1 : 0));
                    // block position
                    block = (glm::ivec3) glm::floor(pIntersect) + blockCorrection;
                }
            }

            // Increment t
            t += intersectionDistance;

            // Update current point
            p = intersection;

            // If solid block -> break loop
            BlockType type = terrain->getBlockOrEmpty(block.x, block.y, block.z);
            if (isSolid(type)) {
                tMin = glm::min(tMin, t);
                break;
            }

            firstIteration = false;
        }
    }
    if (tMin < eps) {
        tMin = 0;
    }
    return tMin;
}


bool Player::isSwimming() {

    // Standing on ground
    if (isGrounded()) {
        return false;
    }

    // Player is standing on at least one non-empty block

    // Check if any of the corners of the bounding box is on a block
    for (glm::vec3 offset : getBoundingBoxBottom()) {
        glm::vec3 pos = position + offset;
        int x = (int) pos.x;
        int z = (int) pos.z;
        int y = glm::round(pos.y);

        // At least one corner in a liquid
        BlockType type = terrain->getBlockOrEmpty(x, y - 1, z);
        if (isLiquid(type)) {
            return true;
        }
    }

    return false;
}


bool Player::isGrounded() {

    // Check if any of the corners of the bounding box is on a block
    for (glm::vec3 offset : getBoundingBoxBottom()) {
        glm::vec3 pos = position + offset;
        int x = (int) pos.x;
        int z = (int) pos.z;
        int y = glm::round(pos.y);

        // If currently on solid block
        BlockType type = terrain->getBlockOrEmpty(x, y - 1, z);
        if (isSolid(type)) {
            return true;
        }
    }

    return false;
}

BlockType Player::getBlockAtEye() const {
    return terrain->getBlockOrEmpty(position);
}


std::vector<glm::vec3> Player::getBoundingBox() {

    if (boundingBox.size() == 12) {
        return boundingBox;
    }

    std::vector<glm::vec3> box;

    // The position of the player is the position of the eyes.
    // The eyes are at the center of the upper cube

    // Bottom
    box.push_back(glm::vec3(.5f,  -2.f,  .5f));
    box.push_back(glm::vec3(.5f,  -2.f, -.5f));
    box.push_back(glm::vec3(-.5f, -2.f,  .5f));
    box.push_back(glm::vec3(-.5f, -2.f, -.5f));

    // Middle
    box.push_back(glm::vec3(.5f,  -1.f,  .5f));
    box.push_back(glm::vec3(.5f,  -1.f, -.5f));
    box.push_back(glm::vec3(-.5f, -1.f,  .5f));
    box.push_back(glm::vec3(-.5f, -1.f, -.5f));

    // Top
    box.push_back(glm::vec3(.5f,  .0f,  .5f));
    box.push_back(glm::vec3(.5f,  .0f, -.5f));
    box.push_back(glm::vec3(-.5f, .0f,  .5f));
    box.push_back(glm::vec3(-.5f, .0f, -.5f));

    boundingBox = box;
    return boundingBox;
}

std::vector<glm::vec3> Player::getBoundingBoxBottom() {

    if (boundingBoxBottom.size() != 4) {
        std::vector<glm::vec3> box;
        box.push_back(glm::vec3(.5f,  -2.f,  .5f));
        box.push_back(glm::vec3(.5f,  -2.f, -.5f));
        box.push_back(glm::vec3(-.5f, -2.f,  .5f));
        box.push_back(glm::vec3(-.5f, -2.f, -.5f));
        boundingBoxBottom = box;
    }
    return boundingBoxBottom;
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
    glm::vec2 mousePos(e->x(), e->y());
    glm::vec2 center(x0, y0);
    // Last position not initialised
    if (lastMousePos.x == -1) {
        lastMousePos = mousePos;
    }

    if (mousePos != center) {
        mouseDelta = mousePos - lastMousePos;
    } else {
        mouseDelta = glm::vec2(0);
    }

    lastMousePos = mousePos;
}

void Player::setCamera(Camera* c) {
    camera = c;
}

void Player::setTerrain(Terrain* t) {
    terrain = t;
}
