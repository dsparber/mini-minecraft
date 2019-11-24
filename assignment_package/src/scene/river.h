#ifndef RIVER_H
#define RIVER_H

#include <iostream>
#include "turtle.h"
#include <QHash>
#include <QStack>
#include <QChar>
#include <QString>
#include <map>
#include "terrain.h"

class River
{
    typedef void (River::*Rule)(void);

public:
    Terrain* terrain;
    QStack<Turtle> stack;
    QHash<QChar, QString> expanRules;
    QHash<QChar, Rule> drawRules;
    Turtle t;

    River(Terrain* terr);
    ~River();

    void createRiver1(int x, int z);
    void createRiver2(int x, int z);
    QString expandString(int numIterations, QString s);
    void drawString(QString s);
    void carveTerrain();
    void bresenham(int x1, int z1, int x2, int z2);

    /// Drawing rules for river generation
    void moveAndDrawLine();
    void rotateLeft();
    void rotateRight();
    void savePosition();
    void storePosition();
};

#endif // RIVER_H
