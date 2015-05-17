/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#ifndef RULE_H
#define RULE_H

struct Condition;
class Rule;

#include "QString"
#include "QDate"
#include "attribute.h"

#include <QDebug>
#include <QDir>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QtXml/QDomDocument>

#define RULE_LOADED 0
#define FILE_ERROR 1
#define XML_ERROR 2

/*! \brief Actions enumeration
 *
 * Possible actions with file
 */
enum actions{
    MOVE=1,
    COPY,
    REMOVE
};


/****************************/
// Rule
/*! \brief Rule class
 *
 * Rule that is created by user is represented by this class
 */
class Rule
{
public:
    Rule();
    ~Rule();
    // Setting attributes
    Condition * SetCond(int,QString,int);

    void SetValue(QDate, Condition* );
    void SetValue(QString,Condition*);
    void SetValue(int,Condition*);
    void SetValue(bool,Condition*);

    void SetOperation(int);
    void SetFileFormat(QString n,QString ff);
    void SetDestination(QString,QString);

    // Save & Load etc
    int SaveRule();
    int LoadRule(QString);

    // Getting values
    QString GetFileFormat();
    QString GetName();
    int GetOperation();
    QString GetDestination();
    QString GetDistribution();
    int GetConditionsCount();    
    Condition * GetNextCondition();
    Condition * GetSpecificCondition(QString);

private:    
    bool IsRuleValid();

    QString fileFormat;
    std::vector<Condition *> conds;
    int operation;
    QString destination;
    QString distribution;
    QString name;
};

/*! \brief Condition structure
 *
 * Structure where is saved all needed for one condition
 */
struct Condition{
    int operand;
    QString property;
    int type;

    QDate date;
    int num;
    QString val;
    bool boolean;
};

#endif // RULE_H
