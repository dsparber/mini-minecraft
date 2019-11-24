#include "turtle.h"

Turtle::Turtle()
    : pos(glm::vec2(0,0)), look(glm::vec2(0,0)), angle(0.f), depth(0)
{}

Turtle::Turtle(glm::vec2 position, glm::vec2 orientation)
    : pos(position), look(orientation), angle(0.f), depth(0)
{}

Turtle::Turtle(const Turtle &t2)
    : pos(t2.pos), look(t2.look), angle(t2.angle), depth(t2.depth)
{}

Turtle::~Turtle() {}
