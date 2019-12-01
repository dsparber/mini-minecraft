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
    float radius;
    int currRiver;

    River(Terrain* terr);
    ~River();

    /// Create each type of river
    void createRiver1(int x, int z);
    void createRiver2(int x, int z);

    /// Expand the given string using expansion rules
    QString expandString(int numIterations, QString axiomString);

    /// Call the drawing rules for each character in expanded string
    void drawString(QString s);

    /// Carve out the terrain around rivers
    void carveTerrain(glm::vec2 pos, float step, float radius);

    /// Drawing rules for river generation
    void moveAndDrawLine();
    // Rotation functions for River 1
    void rotateLeft1();
    void rotateRight1();
    // Rotation functions for River 2
    void rotateLeft2();
    void rotateRight2();
    void savePosition();
    void storePosition();
    // When X char is read, do nothing, keep current Turtle
    void doNothing();
};

#endif // RIVER_H
