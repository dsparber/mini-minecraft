#include "river.h"

River::River(Terrain* terr)
    : terrain(terr), stack(), expanRules(), drawRules(), t()
{
    expanRules.insert('X', "F[-X]F[+X[-X]+X]");
    expanRules.insert('F', "FF");
    expanRules.insert('Y', "G[+Y][-YG[-Y][+Y][-Y]]");
    expanRules.insert('G', "GG");
    expanRules.insert('+', "+");
    expanRules.insert('-', "-");
    expanRules.insert('[', "[");
    expanRules.insert(']', "]");

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
    std::cout << "expanded: " << expanded.toStdString() << std::endl;
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

void River::carveTerrain()
{
    // carve out surrounding terrain so that it slopes into river
    glm::vec2 perpend = t.look * glm::rotate(90);
    float y = pow(perpend.x, 3) / 3.0;

    bool hit = true;
    glm::vec3 look(perpend.x, y, perpend.y);
    glm::vec3 currPos(t.pos.x, 128, t.pos.y);
    float step = 0.75;

    while(hit)
    {
        currPos = currPos + step * look;

        if(terrain->getBlockAt(glm::floor(currPos.x), glm::floor(currPos.y), glm::floor(currPos.z)) == EMPTY)
        {
            hit = false;
        } else {
            for (int i = glm::floor(currPos.y); i < ; i++) {
                terrain->setBlockAt(glm::floor(currPos.x), i, glm::floor(currPos.z), EMPTY);
            }
        }
    }
}

void River::moveAndDrawLine()
{
    std::cout << "testmoveAndDrawLine" << std::endl;
    glm::vec2 currPos = t.pos;
    t.pos = t.pos + 5.f * t.look;
    float radius = 5.f / t.depth;
    bresenham(currPos.x, currPos.y, t.pos.x, t.pos.y, radius);
}

void River::bresenham(int x1, int z1, int x2, int z2, float radius)
{
    int m_new = 2 * (z2 - z1);
    int slope_error_new = m_new - (x2 - x1);
    std::cout << "test bresenham" << std::endl;

    // Bresenham line formula (draw between 2 points)
    for (int x = x1, z = z1; x <= x2; x++)
    {
        // Loop through each direction to shape bottom of river
        for (int newX = x - radius; newX < x + radius; x++) {
            for (int newZ = z - radius; newZ < z + radius; z++) {
                for (int y = 150; y > 160; y--)
                {
                    if (terrain->getBlockAt(x, y, z) != WATER)  // If block is not already water
                    {
                        std::cout << "test getBlock if state" << std::endl;
                        glm::vec4 center(x, 128, z, 0);
                        glm::vec4 pos2(newX, y, newZ, 0);

                        if (glm::distance(center, pos2) < radius)   // If block is within radius
                        {
                            std::cout << "test setBlock if state" << std::endl;
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
    std::cout << "test rotateLeft" << std::endl;
    float angle = rand() % 40 + 10;
    t.look = glm::vec2(t.look.x * cos(angle), t.look.y * sin(angle));
}

void River::rotateRight()
{
    std::cout << "test rotateRight" << std::endl;
    float angle = -(rand() % 40 + 10);
    t.look = glm::vec2(t.look.x * cos(angle * M_PI / 180.f), t.look.y * sin(angle * M_PI / 180.f));
}

void River::savePosition()
{
   std::cout << "test savePosition" << std::endl;
   stack.push(t);
}

void River::storePosition()
{
    std::cout << "test storePosition" << std::endl;
    stack.pop();
}
