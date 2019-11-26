#include "river.h"

River::River(Terrain* terr)
    : terrain(terr), stack(), expanRules(), drawRules(), t()
{
    expanRules.insert('X', "F[-X]F[+X]F[-X]");
    expanRules.insert('F', "FF");
    expanRules.insert('Y', "G[+Y][-YG[-Y][+Y][-Y]]");
    expanRules.insert('G', "GG");
    expanRules.insert('+', "+");
    expanRules.insert('-', "-");
    expanRules.insert('[', "[");
    expanRules.insert(']', "]");

    drawRules.insert('F', &River::moveAndDrawLine);
    drawRules.insert('G', &River::moveAndDrawLine);
    drawRules.insert('X', &River::doNothing);
    drawRules.insert('Y', &River::doNothing);
    drawRules.insert('+', &River::rotateLeft);
    drawRules.insert('-', &River::rotateRight);
    drawRules.insert('[', &River::savePosition);
    drawRules.insert(']', &River::storePosition);
}


River::~River() {}

void River::createRiver1(int x, int z)
{
    t.pos = glm::vec2(x, z); // (4, 50)
    t.look = glm::vec2(1.f, 0.f);
    t.depth = 0;
    QString axiom = "X";

    QString expanded = expandString(2, axiom);
    drawString(expanded);
}

void River::createRiver2(int x, int z)
{
    t.pos = glm::vec2(x, z); // (56, 62)
    t.look = glm::vec2(0.f, -1.f);
    t.depth = 0;
    QString axiom = "Y";

    QString expanded = expandString(3, axiom);
    drawString(expanded);
}

QString River::expandString(int numIterations, QString axiom)
{
    QString expandedString = axiom;

    for (int i = 0; i < numIterations; i++) {
        t.depth++;

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

void River::carveTerrain(glm::vec2 pos, glm::vec2 lk, float radius)
{
    // Carve right
    glm::vec3 currPos(glm::floor(pos.x) + radius, 129, pos.y);
    for (int i = 0; i < 10; i++) {
        for (int j = currPos.y; j < 256; j++) {
            terrain->getBlockAt(currPos.x, j, currPos.y);
            terrain->setBlockAt(currPos.x, j, currPos.y, EMPTY);
        }
        currPos.x = currPos.x + 1;
        currPos.y = currPos.y + 1;
    }


    // Carve left
    currPos = glm::vec3(pos.x - radius, 129, pos.y);
    for (int i = 0; i < 10; i++) {
        for (int j = currPos.y; j < 256; j++) {
            terrain->setBlockAt(glm::floor(currPos.x), j, currPos.y, EMPTY);
        }
        currPos.x = currPos.x - 1;
        currPos.y = currPos.y + 1;
    }

    //glm::vec2 perpend(lk.y, -(lk.x));   // Two diff perpends for each side?
    //float y = pow(perpend.x, 3) / 3.0;
    //float y = 129;

    //bool hit = true;
//    glm::vec3 look(perpend.x, y, perpend.y);
//    glm::vec3 currPos(pos.x + radius, 128, pos.y + radius);
//    float step = 0.75;

//    currPos = currPos + step * look;
//    for (int i = glm::floor(currPos.x); i < 256; i++) {
//        terrain->setBlockAt(glm::floor(currPos.x), i, glm::floor(currPos.z), EMPTY);
//    }
//    y++;

//    while(hit)
//    {
//        currPos = currPos + step * look;

//        if(terrain->getBlockAt(glm::floor(currPos.x), glm::floor(currPos.y), glm::floor(currPos.z)) == EMPTY)
//        {
//            hit = false;
//        } else {
//            for (int i = glm::floor(currPos.x); i < 256; i++) {
//                terrain->setBlockAt(glm::floor(currPos.x), i, glm::floor(currPos.z), EMPTY);
//            }
//        }
//    }


//    currPos = glm::vec3(pos.x - radius, 128, pos.y - radius);
//    while(hit)
//    {
//        currPos = currPos + step * look;

//        if(terrain->getBlockAt(glm::floor(currPos.x), glm::floor(currPos.y), glm::floor(currPos.z)) == EMPTY)
//        {
//            hit = false;
//        } else {
//            for (int i = glm::floor(currPos.x); i < 256; i++) {
//                terrain->setBlockAt(glm::floor(currPos.x), i, glm::floor(currPos.z), EMPTY);
//            }
//        }
//    }
}

void River::moveAndDrawLine()
{
    glm::vec2 currPos = t.pos;
    float radius = 5.f;
    float step = 1.f;

    for (int i = 0; i < 15; i++)
    {
        currPos = currPos + step * t.look;

        int x = glm::floor(currPos.x);
        int z = glm::floor(currPos.y);

        // Loop through each direction to shape bottom of river
        for (int i = x - radius; i < x + radius; i++) {
            for (int j = z - radius; j < z + radius; j++) {
                for (int y = 128; y > 118; y--)
                {
                    glm::vec4 center(x, 128, z, 0);
                    glm::vec4 pos2(i, y, j, 0);

                    if (glm::distance(center, pos2) < radius)   // If block is within radius
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

        carveTerrain(glm::vec2(x, z), t.look, radius);
    }

    t.pos = currPos;
}

void River::rotateLeft()
{
    float angle = rand() % 60 + 20;
    float angleRad = angle * M_PI / 180.f;
    t.look = glm::mat2(cos(angleRad), -sin(angleRad), sin(angleRad), cos(angleRad)) * t.look;
}

void River::rotateRight()
{
    float angle = rand() % 60 + 20;
    float angleRad = angle * M_PI / 180.f;
    t.look = glm::mat2(cos(angleRad), sin(angleRad), -sin(angleRad), cos(angleRad)) * t.look;
}

void River::savePosition()
{
    stack.push(Turtle(t));
}

void River::storePosition()
{
    t = stack.pop();
}

void River::doNothing() {}
