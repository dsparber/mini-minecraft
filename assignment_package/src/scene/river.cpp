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

    std::cout << "create river1" << std::endl;
    QString expanded = expandString(2, axiom);
    drawString(expanded);
}

void River::createRiver2(int x, int z)
{
    t.pos = glm::vec2(x, z); // (56, 62)
    t.look = glm::vec2(0.f, -1.f);
    t.depth = 0;
    QString axiom = "Y";

    std::cout << "create river2" << std::endl;
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

    std::cout << "expanded string: " << expandedString.toStdString() << std::endl;
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
    glm::vec2 perpend(t.look.y, -(t.look.x));
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
            int myY = glm::floor(currPos.y);
            while (terrain->getBlockAt(glm::floor(currPos.x), myY, glm::floor(currPos.z)) != EMPTY) {
                terrain->setBlockAt(glm::floor(currPos.x), myY, glm::floor(currPos.z), EMPTY);
                myY++;
            }
        }
    }
}

void River::moveAndDrawLine()
{
    std::cout << "testmoveAndDrawLine" << std::endl;
    glm::vec2 currPos = t.pos;
    t.pos = t.pos + 5.f * t.look;
    float radius = 10.f / t.depth;
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

//        carveTerrain();
//        terrain->setBlockAt(x, 128, z, WATER);

//        for (int i = 129; i < 256; i++)
//        {
//            terrain->setBlockAt(x, i, z, EMPTY);
//        }

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
    float angle = rand() % 60 + 20;
    float angleRad = angle * M_PI / 180.f;
    t.look = t.look * glm::mat2(cos(angleRad), -sin(angleRad), sin(angleRad), cos(angleRad));
}

void River::rotateRight()
{
    std::cout << "test rotateRight" << std::endl;
    float angle = -(rand() % 60 + 20);
    float angleRad = angle * M_PI / 180.f;
    t.look = t.look * glm::mat2(cos(angleRad), sin(angleRad), -sin(angleRad), cos(angleRad));
}

void River::savePosition()
{
   std::cout << "test savePos" << std::endl;
   stack.push(t);
}

void River::storePosition()
{
    std::cout << "test storePos" << std::endl;
    stack.pop();
}

void River::doNothing() {}
