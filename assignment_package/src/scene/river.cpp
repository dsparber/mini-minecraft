#include "river.h"

River::River()
    : stack(), grammar(), draw()
{}

River::~River() {}

void River::createRiver1(int x, int z)
{
    int n = 2;
    float angle = 25.f;

    grammar.insert(QChar('X'), QString("F[-X]F[+X[-X][+X]]"));
    grammar.insert(QChar('F'), QString("FF"));
    Turtle t();
}

void River::createRiver2(int x, int z)
{
    int n = 3;
    float angle = 25.f;

    grammar.insert(QChar('X'), QString("F[+X][-XF[-X][+X][-X]]"));
    grammar.insert(QChar('F'), QString("FF"));
    Turtle t();
}

void River::moveAndDrawLine()
{

}

void River::rotate()
{

}

void River::savePosition()
{

}

void River::storePosition()
{

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

//QStack<Turtle> stack;
//QHash<QChar, QString> grammar;
//QHash<QChar, Rule> draw;

// F = draw a line, moving forward
// - = rotate(30)
// + = rotate(-30)
// [ = save position
// ] = store position
