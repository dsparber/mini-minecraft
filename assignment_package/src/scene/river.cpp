#include "river.h"

River::River()
    : stack(), expanRules(), drawRules(), t(glm::vec2(0,0), glm::vec2(0,0))
{
    expanRules.insert({'X', "F[-X]F[+X[-X][+X]]"});
    expanRules.insert({'F', "FF"});
    expanRules.insert({'Y', "G[+Y][-YG[-Y][+Y][-Y]]"});
    expanRules.insert({'G', "GG"});

//    drawRules.insert('F', &River::moveAndDrawLine);
//    drawRules.insert({'F', &River::moveAndDrawLine});
//    drawRules.insert({'F', (River::moveAndDrawLine*) (void)});
//    drawRules.insert('G', moveAndDrawLine());
//    drawRules.insert('+', rotateLeft());
//    drawRules.insert('-', rotateRight());
//    drawRules.insert('[', savePosition());
//    drawRules.insert(']', storePosition());
}

River::~River() {}

void River::createRiver1(int x, int z)
{
    t.pos = glm::vec2(x, z);
    t.look = glm::vec2(x, z);
    t.depth = 0;
    QString axiom = "X";

    QString expanded = expandString(2, axiom);
    drawString(expanded);
    carveTerrain();
}

void River::createRiver2(int x, int z)
{
    t.pos = glm::vec2(x, z);
    t.look = glm::vec2(x, z);
    t.depth = 0;
    QString axiom = "Y";

    QString expanded = expandString(3, axiom);
    drawString(expanded);
    carveTerrain();
}

QString River::expandString(int numIterations, QString axiom)
{
    QString result = "";
    for (int i = 0; i < numIterations; i++) {
        t.depth++;
        for (QChar c : axiom) {
            result += expanRules[c];
        }
    }
    return result;
}

void River::drawString(QString s)
{
    for (QChar c : s) {
        drawRules[c];
    }
}

void River::carveTerrain()
{
    // carve out surrounding terrain so that it slopes into river
}

void River::moveAndDrawLine()
{
    glm::vec2 currPos = t.pos;
    float radius = 5.f / t.depth;

    for (int x = currPos.x - radius; x < currPos.x + radius; x++) {
        for (int z = currPos.y - radius; z < currPos.y + radius; z++) {
            for (int y = 128; y < 128 - radius; y--) {
                if (getBlockAt(x, y, z) != WATER) {
                    glm::vec4 center(currPos.x, 128, currPos.y, 0);
                    glm::vec4 pos2(x, y, z, 0);
                    if (glm::distance(center, pos2) < radius) {
                        setBlockAt(x, y, z, WATER);
                    }
                }
            }
        }
    }

    // move currPos forward 4 or 5 blocks, start again from this position
    // repeat 2 times to draw a line, line length = 15 blocks?
}

void River::rotateLeft()
{
    t.angle -= rand() % 40 + 10;
}

void River::rotateRight()
{
    t.angle += rand() % 40 + 10;
}

void River::savePosition()
{
    //stack.push(t);
}

void River::storePosition()
{
    //stack.pop();
}
