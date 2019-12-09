#include "turtle3d.h"

Turtle3D::Turtle3D()
    : pos(glm::vec3(0,0,0)), look(glm::vec3(0,0,0))
{}

Turtle3D::Turtle3D(glm::vec3 position, glm::vec3 orientation)
    : pos(position), look(orientation)
{}

Turtle3D::Turtle3D(const Turtle3D &t2)
    : pos(t2.pos), look(t2.look)
{}

Turtle3D::~Turtle3D() {}
