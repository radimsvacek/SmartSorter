/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#include "operation.h"

// Only getters and setters

Operation::Operation(){}

Operation::~Operation(){}


void Operation::SetOperation(int op){
    action =op;
}

void Operation::SetDistribution(QString distr){
    distribution=distr;
}

void Operation::SetDestination(QString dest){
    destinatin=dest;
}

void Operation::SetPriority(int p){
    priority=p;
}

int Operation::GetPriority(){
    return priority;
}

int Operation::GetOperation(){
    return action;
}

QString Operation::GetDestination(){
    return destinatin;
}

QString Operation::GetDistribution(){
    return distribution;
}
