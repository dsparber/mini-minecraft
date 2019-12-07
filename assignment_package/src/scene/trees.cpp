#include "trees.h"

Trees::Trees()
    : stack(), expanRules(), drawRules(), t(), radius(2.f)
{
    /// Fill maps with expansion and drawing rules
    expanRules.insert('X', "F[-X][+X]");
    expanRules.insert('F', "F");
    expanRules.insert('Y', "G-[[Y]+Y]+G[+GY]-Y");
    expanRules.insert('G', "GG");
    expanRules.insert('+', "+");
    expanRules.insert('-', "-");
    expanRules.insert('[', "[");
    expanRules.insert(']', "]");

    drawRules.insert('F', &Trees::moveAndDrawLine);
    drawRules.insert('G', &Trees::moveAndDrawLine);
    drawRules.insert('X', &Trees::doNothing);
    drawRules.insert('Y', &Trees::doNothing);
    drawRules.insert('+', &Trees::rotateLeft);
    drawRules.insert('-', &Trees::rotateRight);
    drawRules.insert('[', &Trees::savePosition);
    drawRules.insert(']', &Trees::storePosition);
}


Trees::~Trees() {}

void Trees::createEntTrees(int x, int z)
{
    /// Set initial values
    t.pos = glm::vec2(x, z);
    t.look = glm::vec2(0.f, 1.f);
    QString axiom = "X";

    QString expanded = expandString(8, axiom);
    drawString(expanded);
}

void Trees::createShireTrees(int x, int z)
{
    /// Set initial values
    t.pos = glm::vec2(x, z);
    t.look = glm::vec2(0.f, 1.f);
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
    glm::vec2 currPos = t.pos;
    float step = 1.f;

    for (int i = 0; i < 6; i++)
    {
        currPos = currPos + step * t.look;

        int x = glm::floor(currPos.x);
        int z = glm::floor(currPos.y);

        /// Loop through x & z to shape tree
        for (int i = x - radius; i < x + radius; i++) {
            for (int j = z - radius; j < z + radius; j++) {
                for (int y = 128; y < 128 + 6; y++)
                {
                    glm::vec4 center(x, 128, z, 0);
                    glm::vec4 pos2(i, y, j, 0);

                    /// If block is within radius, set to bark
                    if (glm::distance(center, pos2) < radius)
                    {
                        //terrain->setBlockAt(i, y, j, BARK);
                    }
                }
            }
        }
    }

    t.pos = currPos;
}

void Trees::rotateLeft()
{
    /// Include random numbers for path orientation
    float angle = rand() % 45 + 30;
    float angleRad = angle * M_PI / 180.f;

    t.look = glm::mat2(cos(angleRad), -sin(angleRad), sin(angleRad), cos(angleRad)) * t.look;
}

void Trees::rotateRight()
{
    /// Include random numbers for path orientation
    float angle = rand() % 45 + 30;
    float angleRad = angle * M_PI / 180.f;

    t.look = glm::mat2(cos(angleRad), sin(angleRad), -sin(angleRad), cos(angleRad)) * t.look;
}

void Trees::savePosition()
{
    /// Decrease tree width for branches
    radius = radius - 0.5;
    stack.push(Turtle(t));
}

void Trees::storePosition()
{
    /// Increase tree width back to original width
    radius = radius + 0.5;
    t = stack.pop();
}

void Trees::doNothing() {}
