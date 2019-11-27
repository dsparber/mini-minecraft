#ifndef TURTLE_H
#define TURTLE_H
#include <la.h>

class Turtle
{
public:
    glm::vec2 pos;
    glm::vec2 look;

    Turtle();
    Turtle(glm::vec2 position, glm::vec2 orientation);
    Turtle(const Turtle &t2); // copy constructor
    ~Turtle();
};

#endif // TURTLE_H
