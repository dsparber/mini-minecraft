#ifndef TURTLE_H
#define TURTLE_H
#include <la.h>


class Turtle
{
public:
    glm::vec2 pos;
    glm::vec2 look;
    int depth;

    Turtle(glm::vec2 position, glm::vec2 orientation);
    ~Turtle();
};

#endif // TURTLE_H
