/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#include "treebranch.h"

// Only getters and setters

TreeBranch::TreeBranch()
{
    walkThroughNode=false;
}


QString TreeBranch::GetStringValue(){
    return stringValue;
}


QDate TreeBranch::GetDateValue(){
    return dateValue;
}

int TreeBranch::GetNumericValue(){
    return numericValue;
}

int TreeBranch::GetType(){
    return type;
}

int TreeBranch::GetOperand(){
    return operand;
}

Node * TreeBranch::GetChild(){
    return childNode;
}

void TreeBranch::SetOperand(int op){
    operand=op;
}

void TreeBranch::SetStringValue(QString val){
    stringValue = val;
    type=TEXT_TYPE;
}

void TreeBranch::SetIntValue(int val){
    numericValue = val;
    type=NUM_TYPE;
}

void TreeBranch::SetDateValue(QDate val){
    dateValue = val;
    type=DATE_TYPE;
}

void TreeBranch::SetChild(Node *node){
    childNode=node;
}

void TreeBranch::SetWalkThrough(){
    walkThroughNode=true;

}

bool TreeBranch::IsWalkThrough(){
    return walkThroughNode;
}

QString TreeBranch::GetChildPropertyToCompare(){
    return childNode->GetComparingAttribute();
}
