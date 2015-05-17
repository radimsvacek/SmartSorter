/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#ifndef TREEBRANCH_H
#define TREEBRANCH_H

class TreeBranch;

#include "node.h"
#include "attribute.h"
#include <QDate>
#include <QString>


/****************************/
// TreeBranch

/*! \brief Branch of node
 *
 * Class represents one branch of node in decision tree
 */
class TreeBranch
{
public:
    TreeBranch();

    void SetOperand(int);
    void SetChild(Node *);

    void SetStringValue(QString);
    void SetDateValue(QDate);
    void SetIntValue(int);
    void SetWalkThrough();

    QString GetStringValue();
    QDate GetDateValue();
    int GetNumericValue();
    int GetType();

    int GetOperand();
    Node * GetChild();
    bool IsWalkThrough();
    QString GetChildPropertyToCompare();

private:

    int operand;
    int type;
    QString stringValue;
    QDate dateValue;
    int numericValue;
    Node *childNode;
    bool walkThroughNode;

};

#endif // TREEBRANCH_H
