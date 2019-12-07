#ifndef TURTLE3D_H
#define TURTLE3D_H
#include <la.h>

class Turtle3D
{
public:
    glm::vec3 pos;
    glm::vec3 look;

    Turtle3D();
    Turtle3D(glm::vec3 position, glm::vec3 orientation);
    Turtle3D(const Turtle3D &t2); // copy constructor
    ~Turtle3D();
};

#endif // TURTLE3D_H
