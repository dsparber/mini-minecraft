#ifndef TREES_H
#define TREES_H

#include <iostream>
#include "turtle3D.h"
#include <QHash>
#include <QStack>
#include <QChar>
#include <QString>
#include <map>
#include "chunk.h"

class Trees
{
    typedef void (Trees::*Rule)(void);

public:
    Chunk* currChunk;
    QStack<Turtle3D> stack;
    QHash<QChar, QString> expanRules;
    QHash<QChar, Rule> drawRules;
    Turtle3D t;
    float radius;

    Trees(Chunk* chunk);
    ~Trees();

    /// Create each type of tree
    void createEntTree(int x, int y, int z);
    void createShireTree(int x, int y, int z);

    /// Expand the given string using expansion rules
    QString expandString(int numIterations, QString axiomString);

    /// Call the drawing rules for each character in expanded string
    void drawString(QString s);

    /// Drawing rules for river generation
    void moveAndDrawLine();
    void rotateX();
    void rotateY();
    void rotateZ();
    void savePosition();
    void storePosition();
    // When X char is read, do nothing, keep current Turtle
    void doNothing();
};

#endif // TREES_H
