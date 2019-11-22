#ifndef RIVER_H
#define RIVER_H

#include <iostream>
#include "turtle.h"
#include <QHash>
#include <QStack>
#include <QChar>
#include <QString>

class River
{
    typedef void (*Rule)(void);

public:
    QStack<Turtle> stack;
    QHash<QChar, QString> grammar;
    QHash<QChar, Rule> draw;

    River();
    ~River();

    void createRiver1(int x, int z);
    void createRiver2(int x, int z);
};

#endif // RIVER_H
