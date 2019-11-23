#ifndef RIVER_H
#define RIVER_H

#include <iostream>
#include "turtle.h"
#include <QHash>
#include <QStack>
#include <QChar>
#include <QString>
#include <map>

class River
{
    typedef void (*Rule)(void);

public:
    QStack<Turtle> stack;
    //QHash<QChar, QString> expanRules;
    std::map<QChar, QString> expanRules;
    //QHash<QChar, Rule> drawRules;
    std::map<QChar, Rule> drawRules;
    Turtle t;

    River();
    ~River();

    void createRiver1(int x, int z);
    void createRiver2(int x, int z);
    QString expandString(int numIterations, QString s);
    void drawString(QString s);
};

#endif // RIVER_H
