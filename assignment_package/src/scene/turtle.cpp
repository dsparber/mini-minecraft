#include "turtle.h"

Turtle::Turtle(glm::vec2 position, glm::vec2 orientation)
    : pos(position), look(orientation), depth(0)
{}

Turtle::~Turtle() {}
