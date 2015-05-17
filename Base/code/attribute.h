/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <QString>
#include <vector>

#define NO_OPERATOR 0

/*! \brief Possible operators
 *
 * Less, equal etc
 */
enum Operators{
    LESS=1,
    LESS_EQUAL,
    MORE,
    MORE_EQUAL,
    EQUAL,
    CONTAINS,
    NOT_CONTAINS,
    IS_TRUE
};

/*! \brief Type of value
 *
 * Value inserted by user
 */
enum Atr_types{
    DATE_TYPE = 1,
    TEXT_TYPE,
    NUM_TYPE,
    TRUE_TYPE
};


/****************************/
// Attribute
/*! \brief Stands for one file property
 *
 * Attribute represent possible property that can be used for classification
 */
class Attribute
{
public:
    Attribute(QString, std::vector<int>,int);
    Attribute();

    ~Attribute();
    void SetName(QString);
    QString GetName();
    std::vector<int> GetPropertyOperations();
    int GetPropertyType();

private:
    QString name;
    int type;
    std::vector<int> operators;
};

#endif // ATTRIBUTE_H
