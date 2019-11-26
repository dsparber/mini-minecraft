#ifndef RIVER_H
#define RIVER_H

#include <iostream>
#include "turtle.h"
#include <QHash>
#include <QStack>
#include <QChar>
#include <QString>
#include "terrain.h"
#include <map>

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
    QString expandString(int numIterations, QString axiomString);
    void drawString(QString s);
    void carveTerrain(glm::vec2 pos, glm::vec2 look, float radius);

    /// Drawing rules for river generation
    void moveAndDrawLine();
    void rotateLeft();
    void rotateRight();
    void savePosition();
    void storePosition();
    void doNothing();
};

#endif // RIVER_H
