/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#include "attribute.h"

Attribute::Attribute(){}


Attribute::Attribute(QString propertyName, std::vector<int> propertyOperations,int propertyType)
{
    type=propertyType;
    name = propertyName;
    operators = propertyOperations;
}

Attribute::~Attribute()
{
    operators.clear();
}

// Name of attribute
QString Attribute::GetName(){
    return name;
}

// Possible operations with that attribute
std::vector<int> Attribute::GetPropertyOperations(){
    return operators;
}

// Type of attribute (date/num/str)
int Attribute::GetPropertyType(){
    return type;
}
