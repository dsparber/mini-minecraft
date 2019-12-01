#include "river.h"

River::River(Terrain* terr)
    : terrain(terr), stack(), expanRules(), drawRules(), t(), radius(4.f), currRiver(1)
{
    /// Fill maps with expansion and drawing rules
    expanRules.insert('X', "F[+X]F[-X]F[+X][-X]");
    expanRules.insert('F', "FFF");
    expanRules.insert('Y', "G[<Y][>YG[>Y][<Y][>Y]]");
    expanRules.insert('G', "GG");
    expanRules.insert('+', "+");
    expanRules.insert('-', "-");
    expanRules.insert('<', "<");
    expanRules.insert('>', ">");
    expanRules.insert('[', "[");
    expanRules.insert(']', "]");

    drawRules.insert('F', &River::moveAndDrawLine);
    drawRules.insert('G', &River::moveAndDrawLine);
    drawRules.insert('X', &River::doNothing);
    drawRules.insert('Y', &River::doNothing);
    drawRules.insert('+', &River::rotateLeft1);
    drawRules.insert('-', &River::rotateRight1);
    drawRules.insert('<', &River::rotateLeft2);
    drawRules.insert('>', &River::rotateRight2);
    drawRules.insert('[', &River::savePosition);
    drawRules.insert(']', &River::storePosition);
}


River::~River() {}

void River::createRiver1(int x, int z)
{
    /// Set initial values
    t.pos = glm::vec2(x, z);
    t.look = glm::vec2(1.f, 0.f);
    QString axiom = "X";

    QString expanded = expandString(2, axiom);
    drawString(expanded);
}

void River::createRiver2(int x, int z)
{
    /// Set initial values
    t.pos = glm::vec2(x, z);
    t.look = glm::vec2(-1.f, 0.f);
    radius = 4.f;
    currRiver = 2;
    QString axiom = "Y";

    QString expanded = expandString(3, axiom);
    drawString(expanded);
}

QString River::expandString(int numIterations, QString axiom)
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

void River::drawString(QString s)
{
    for (QChar c : s) {
        Rule r = drawRules.value(c);
        (this->*r)();
    }
}

void River::carveTerrain(glm::vec2 pos, float step, float radius)
{
    /// Carve right
    glm::vec3 currPos(pos.x + radius, 129, pos.y);
    for (int i = 0; i < 30; i++) {
        for (int j = currPos.y; j < 256; j++) {
            terrain->setBlockAt(glm::floor(currPos.x), j, currPos.z, EMPTY);
        }
        currPos.x = currPos.x + step;
        currPos.y = currPos.y + step;
    }

    /// Carve left
    currPos = glm::vec3(pos.x - radius, 129, pos.y);
    for (int i = 0; i < 30; i++) {
        for (int j = currPos.y; j < 256; j++) {
            terrain->setBlockAt(glm::floor(currPos.x), j, currPos.z, EMPTY);
        }
        currPos.x = currPos.x - step;
        currPos.y = currPos.y + step;
    }

    /// Carve up
    currPos = glm::vec3(pos.x, 129, pos.y + radius);
    for (int i = 0; i < 30; i++) {
        for (int j = currPos.y; j < 256; j++) {
            terrain->setBlockAt(currPos.x, j, glm::floor(currPos.z), EMPTY);
        }
        currPos.z = currPos.z + step;
        currPos.y = currPos.y + step;
    }

    /// Carve down
    currPos = glm::vec3(pos.x, 129, pos.y - radius);
    for (int i = 0; i < 30; i++) {
        for (int j = currPos.y; j < 256; j++) {
            terrain->setBlockAt(currPos.x, j, glm::floor(currPos.z), EMPTY);
        }
        currPos.z = currPos.z - step;
        currPos.y = currPos.y + step;
    }
}

void River::moveAndDrawLine()
{
    glm::vec2 currPos = t.pos;
    float step = 1.f;

    for (int i = 0; i < 12; i++)
    {
        currPos = currPos + step * t.look;

        int x = glm::floor(currPos.x);
        int z = glm::floor(currPos.y);

        /// Loop through each direction to shape bottom of river
        for (int i = x - radius; i < x + radius; i++) {
            for (int j = z - radius; j < z + radius; j++) {
                for (int y = 128; y > 118; y--)
                {
                    glm::vec4 center(x, 128, z, 0);
                    glm::vec4 pos2(i, y, j, 0);

                    /// If block is within radius, set to water
                    if (glm::distance(center, pos2) < radius)
                    {
                        terrain->setBlockAt(i, y, j, WATER);
                        for (int k = 129; k < 256; k++)
                        {
                            terrain->setBlockAt(i, k, j, EMPTY);
                        }
                    }
                }
            }
        }

        carveTerrain(glm::vec2(x, z), 0.75, radius);
    }

    t.pos = currPos;
}

void River::rotateLeft1()
{
    /// Include random numbers for path orientation
    float angle = rand() % 45 + 30;
    float angleRad = angle * M_PI / 180.f;

    /// Include random numbers for branch generation
    if ((float) rand() / (RAND_MAX) > 0.4) {
        t.look = glm::mat2(cos(angleRad), -sin(angleRad), sin(angleRad), cos(angleRad)) * t.look;
    }
}

void River::rotateRight1()
{
    /// Include random numbers for path orientation
    float angle = rand() % 45 + 30;
    float angleRad = angle * M_PI / 180.f;

    /// Include random numbers for branch generation
    if ((float) rand() / (RAND_MAX) > 0.4) {
        t.look = glm::mat2(cos(angleRad), sin(angleRad), -sin(angleRad), cos(angleRad)) * t.look;
    }
}

void River::rotateLeft2()
{
    /// Include random numbers for path orientation
    float angle = rand() % 70 + 45;
    float angleRad = angle * M_PI / 180.f;

    /// Include random numbers for branch generation
    if ((float) rand() / (RAND_MAX) > 0.2) {
        t.look = glm::mat2(cos(angleRad), -sin(angleRad), sin(angleRad), cos(angleRad)) * t.look;
    }
}

void River::rotateRight2()
{
    /// Include random numbers for path orientation
    float angle = rand() % 70 + 45;
    float angleRad = angle * M_PI / 180.f;

    /// Include random numbers for branch generation
    if ((float) rand() / (RAND_MAX) > 0.2) {
        t.look = glm::mat2(cos(angleRad), sin(angleRad), -sin(angleRad), cos(angleRad)) * t.look;
    }
}

void River::savePosition()
{
    /// Decrease river width for branches
    if (currRiver == 1) {
        radius = radius - 1.f;
    } else {
        radius = radius - 0.5;
    }

    stack.push(Turtle(t));
}

void River::storePosition()
{
    /// Increase river width back to original width
    if (currRiver == 1) {
        radius = radius + 1.f;
    } else {
        radius = radius + 0.5;
    }

    t = stack.pop();
}

void River::doNothing() {}
