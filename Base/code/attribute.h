#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <QString>
#include <vector>

#define NO_OPERATOR 0

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

enum Atr_types{
    DATE_TYPE = 1,
    TEXT_TYPE,
    NUM_TYPE,
    TRUE_TYPE
};


/****************************/
// Attribute
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
