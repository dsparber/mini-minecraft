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
    t.look = glm::vec2();
    QString axiom = "X";

    QString expanded = expandString(2, axiom);
    drawString(expanded);
}

void River::createRiver2(int x, int z)
{
    t.pos = glm::vec2(x, z);
    t.look = glm::vec2();
    QString axiom = "Y";

    QString expanded = expandString(3, axiom);
    drawString(expanded);
}

QString River::expandString(int numIterations, QString axiom)
{
    QString result = "";
    for (QChar c : axiom) {
        result += expanRules[c];
    }
    return result;
}

void River::drawString(QString s)
{
    for (QChar c : s) {
        drawRules[c];
    }
}

// River 1
// n = 2
// Angle: 25
// Constants: X
// Axioms: X
// Rule 1: X = F[-X]F[+X[-X][+X]]
// Rule 2: F = FF

// River 2
// n = 3
// Angle: 25
// Constants: X
// Axioms: X
// Rule 1: X = F[+X][-XF[-X][+X][-X]]
// Rule 2: F = FF
