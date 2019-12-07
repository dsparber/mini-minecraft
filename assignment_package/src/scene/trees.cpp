#include "trees.h"

Trees::Trees(Chunk* chunk)
    : currChunk(chunk), stack(), expanRules(), drawRules(), t(), radius(2.f)
{
    /// Fill maps with expansion and drawing rules
    expanRules.insert('X', "F[-X][+X]");
    expanRules.insert('F', "F");
    expanRules.insert('Y', "G-[[Y]+Y]+G[+GY]-Y");
    expanRules.insert('G', "GG");
    expanRules.insert('+', "+");
    expanRules.insert('-', "-");
    expanRules.insert('=', "=");
    expanRules.insert('[', "[");
    expanRules.insert(']', "]");

    drawRules.insert('F', &Trees::moveAndDrawLine);
    drawRules.insert('G', &Trees::moveAndDrawLine);
    drawRules.insert('X', &Trees::doNothing);
    drawRules.insert('Y', &Trees::doNothing);
    drawRules.insert('+', &Trees::rotateX);
    drawRules.insert('-', &Trees::rotateY);
    drawRules.insert('=', &Trees::rotateZ);
    drawRules.insert('[', &Trees::savePosition);
    drawRules.insert(']', &Trees::storePosition);
}


Trees::~Trees() {}

void Trees::createEntTree(int x, int y, int z)
{
    /// Set initial values
    t.pos = glm::vec3(x, y, z);
    t.look = glm::vec3(0.f, 1.f, 0.f);
    QString axiom = "X";

    QString expanded = expandString(8, axiom);
    drawString(expanded);
}

void Trees::createShireTree(int x, int y, int z)
{
    /// Set initial values
    t.pos = glm::vec3(x, y, z);
    t.look = glm::vec3(0.f, 1.f, 0.f);
    radius = 2.f;
    QString axiom = "Y";

    QString expanded = expandString(6, axiom);
    drawString(expanded);
}

QString Trees::expandString(int numIterations, QString axiom)
{
    QString expandedString = axiom;

    for (int i = 0; i < numIterations; i++) {
        QString copyExpStr = expandedString;
        expandedString = "";

        for (QChar c : copyExpStr) {
            expandedString += expanRules.value(c);
        }
    }

    return expandedString;
}

void Trees::drawString(QString s)
{
    for (QChar c : s) {
        Rule r = drawRules.value(c);
        (this->*r)();
    }
}

void Trees::moveAndDrawLine()
{
    glm::vec3 currPos = t.pos;
    float step = 1.f;

    for (int i = 0; i < 4; i++)
    {
        currPos = currPos + step * t.look;

        int x = glm::floor(currPos.x);
        int y = glm::floor(currPos.y);
        int z = glm::floor(currPos.z);

        currChunk->setBlockAt(x, y, z, BARK);
    }

    t.pos = currPos;
}

void Trees::rotateX()
{
    /// Include random numbers for path orientation
    float angle = rand() % 45 + 30;
    float angleRad = angle * M_PI / 180.f;

    t.look = glm::mat3(glm::vec3(1.f, 0.f, 0.f),
                       glm::vec3(0.f, cos(angleRad), sin(angleRad)),
                       glm::vec3(0.f, -sin(angleRad), cos(angleRad))) * t.look;
}

void Trees::rotateY()
{
    /// Include random numbers for path orientation
    float angle = rand() % 45 + 30;
    float angleRad = angle * M_PI / 180.f;

    t.look = glm::mat3(glm::vec3(cos(angleRad), 0.f, -sin(angleRad)),
                       glm::vec3(0.f, 1.f, 0.f),
                       glm::vec3(sin(angleRad), 0.f, cos(angleRad))) * t.look;
}

void Trees::rotateZ()
{
    /// Include random numbers for path orientation
    float angle = rand() % 45 + 30;
    float angleRad = angle * M_PI / 180.f;

    t.look = glm::mat3(glm::vec3(cos(angleRad), sin(angleRad), 0.f),
                       glm::vec3(-sin(angleRad), cos(angleRad), 0.f),
                       glm::vec3(0.f, 0.f, 1.f)) * t.look;
}

void Trees::savePosition()
{
    /// Decrease tree width for branches
    radius = radius - 0.5;
    stack.push(Turtle3D(t));
}

void Trees::storePosition()
{
    /// Increase tree width back to original width
    radius = radius + 0.5;
    t = stack.pop();
}

void Trees::doNothing() {}
