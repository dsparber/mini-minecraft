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
    QChar axiom = 'X';
    QString axiomString = expanRules[axiom];

    QString expanded = expandString(2, axiomString);
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

QString River::expandString(int numIterations, QString axiomString)
{
    QString result = "";
    for (int i = 0; i < numIterations; i++) {
        t.depth++;
        for (QChar c : axiomString) {
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
    t.pos = t.pos + 8.f * t.look;
    float radius = 8.f / t.depth;
    bresenham(currPos.x, currPos.y, t.pos.x, t.pos.y, radius);
}

void River::bresenham(int x1, int z1, int x2, int z2, float radius)
{
    int m_new = 2 * (z2 - z1);
    int slope_error_new = m_new - (x2 - x1);

    // Bresenham line formula (draw between 2 points)
    for (int x = x1, z = z1; x <= x2; x++)
    {
        // Loop through each direction to shape bottom of river
        for (int newX = x - radius; newX < x + radius; x++) {
            for (int newZ = z - radius; newZ < z + radius; z++) {
                for (int y = 128; y < 118; y--)
                {
                    if (terrain->getBlockAt(x, y, z) != WATER)  // If block is not already water
                    {
                        glm::vec4 center(x, 128, z, 0);
                        glm::vec4 pos2(newX, y, newZ, 0);

                        if (glm::distance(center, pos2) < radius)   // If block is within radius
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
    t.look = glm::vec2(t.look.x * cos(t.angle), t.look.y * sin(t.angle));
}

void River::rotateRight()
{
    t.angle += rand() % 40 + 10;
    t.look = glm::vec2(t.look.x * cos(t.angle), t.look.y * sin(t.angle));
}

void River::savePosition()
{
   stack.push(t);
}

void River::storePosition()
{
    stack.pop();
}
