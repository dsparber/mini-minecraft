#include "river.h"

River::River(Terrain* terr)
    : terrain(terr), stack(), expanRules(), drawRules(), t(glm::vec2(0,0), glm::vec2(0,0))
{
    expanRules.insert('X', "F[-X]F[+X[-X][+X]]");
    expanRules.insert('F', "FF");
    expanRules.insert('Y', "G[+Y][-YG[-Y][+Y][-Y]]");
    expanRules.insert('G', "GG");

    drawRules.insert('F', &River::moveAndDrawLine);
    drawRules.insert('G', &River::moveAndDrawLine);
    drawRules.insert('+', &River::rotateLeft);
    drawRules.insert('-', &River::rotateRight);
    drawRules.insert('[', &River::savePosition);
    drawRules.insert(']', &River::storePosition);
}

River::~River() {}

void River::createRiver1(int x, int z)
{
    t.pos = glm::vec2(x, z); // (2, 50)
    t.look = glm::vec2(1.f, 0.f);
    t.depth = 0;
    QString axiom = "X";

    QString expanded = expandString(2, axiom);
    drawString(expanded);
    carveTerrain();
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
        carveTerrain();
    }
}

void River::carveTerrain()
{
//    // carve out surrounding terrain so that it slopes into river
//    glm::vec2 direct = t.look * t.angle;

//    // How to get perpendicular vector on each side of river?
//    glm::vec2 perpend = ;
//    int y = 128;

//    while (getBlockAt(glm::floor(currPos.x), y, glm::floor(currPos.z)) != EMPTY) {
//        setBlockAt(glm::floor(currPos.x), y, glm::floor(currPos.z), EMPTY);
//        y++;
//    }

//    currPos = currPos + 0.75 * look;
}

void River::moveAndDrawLine()
{
    glm::vec2 currPos = t.pos;
    t.pos = t.pos + 6.f * t.look;
    bresenham(currPos.x, currPos.y, t.pos.x, t.pos.y);
}

void River::bresenham(int x1, int z1, int x2, int z2)
{
    int m_new = 2 * (z2 - z1);
    int slope_error_new = m_new - (x2 - x1);

    for (int x = x1, z = z1; x <= x2; x++)
    {
        for (int newX = x - 8.f; newX < x + 8.f; x++) {
            for (int newZ = z - 8.f; newZ < z + 8.f; z++) {
                for (int y = 128; y < 118; y--)
                {
                    QColor color = QColor(0, 1, 0);

                    if (terrain->getBlockAt(x, y, z) != WATER)
                    {
                        glm::vec4 center(x, 128, z, 0);
                        glm::vec4 pos2(newX, y, newZ, 0);
                        if (glm::distance(center, pos2) < 8.f)
                        {
                            terrain->setBlockAt(newX, y, newZ, WATER);
                        }
                    }
                }
            }
        }

        slope_error_new += m_new;
        if (slope_error_new >= 0)
        {
            z++;
            slope_error_new  -= 2 * (x2 - x1);
        }
    }
}

void River::rotateLeft()
{
    t.angle -= rand() % 40 + 10;
    t.look = t.look * t.angle;
}

void River::rotateRight()
{
    t.angle += rand() % 40 + 10;
    t.look = t.look * t.angle;
}

void River::savePosition()
{
   stack.push(t);
}

void River::storePosition()
{
    stack.pop();
}
