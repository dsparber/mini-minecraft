#ifndef TREES_H
#define TREES_H

#include <iostream>
#include "turtle.h"
#include <QHash>
#include <QStack>
#include <QChar>
#include <QString>
#include <map>

class Trees
{
    typedef void (Trees::*Rule)(void);

public:
    QStack<Turtle> stack;
    QHash<QChar, QString> expanRules;
    QHash<QChar, Rule> drawRules;
    Turtle t;
    float radius;

    Trees();
    ~Trees();

    /// Create each type of tree
    void createEntTrees(int x, int z);
    void createShireTrees(int x, int z);

    /// Expand the given string using expansion rules
    QString expandString(int numIterations, QString axiomString);

    /// Call the drawing rules for each character in expanded string
    void drawString(QString s);

    /// Drawing rules for river generation
    void moveAndDrawLine();
    void rotateLeft();
    void rotateRight();
    void savePosition();
    void storePosition();
    // When X char is read, do nothing, keep current Turtle
    void doNothing();
};

#endif // TREES_H
