/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#include "node.h"

Node::Node(){
    operationInNode = NULL;
    stuffToCompare="";
}

Node::~Node(){

}

void Node::SetParent(Node *P){
	parentNode=P;
}

// Delete all tree recursively
void Node::DeleteTree(){
    for(auto branchesIterator = branches.begin(); branchesIterator != branches.end();){
        (*branchesIterator)->GetChild()->DeleteTree();
        delete (*branchesIterator);
        branchesIterator = branches.erase(branchesIterator);
    }
    delete operationInNode;
    delete this;
}


// Printing tree -> for DEBUG
void Node::PrintTree(){
    qDebug() << stuffToCompare << "!! Kids from this !!";
    if(operationInNode != NULL)
        qDebug() << "TADY uz je operace - " << operationInNode->GetPriority();

    int type;
    for(auto branchesIterator = branches.begin(); branchesIterator != branches.end();branchesIterator++){

        if((*branchesIterator)->IsWalkThrough()){
            qDebug() << "just walking through";
        }
        else{
            qDebug() << "Operand " << (*branchesIterator)->GetOperand();
            type=(*branchesIterator)->GetType();
                    switch(type){
                        case DATE_TYPE:
                            qDebug() << "Hodnota (date) " << ((*branchesIterator)->GetDateValue());
                            break;
                        case NUM_TYPE:
                            qDebug() << "Hodnota int " << (*branchesIterator)->GetNumericValue();
                            break;
                        case TEXT_TYPE:
                            qDebug() << "Hodnota string " << (*branchesIterator)->GetStringValue();
                            break;
                        default:
                            break;
                    }
        }
        (*branchesIterator)->GetChild()->PrintTree();
    }
    qDebug() << stuffToCompare << "!! EXIT Kids from this !!";
}



void Node::SetOperation(int act,QString dest,QString distr){
    operationInNode = new Operation();
    operationInNode->SetOperation(act);
    operationInNode->SetDistribution(distr);
    operationInNode->SetDestination(dest);
}

Operation * Node::GetOperation(){
    return operationInNode;
}

void Node::SetAttributeToCompare(QString attribute){
    stuffToCompare=attribute;
}

// Found file format root node from basic root node
Node * Node::GetFileFormatNode(QString Format){
    for(auto branchesIterator = branches.begin(); branchesIterator != branches.end(); ++branchesIterator){
        if((*branchesIterator)->GetStringValue() == Format)
            return (*branchesIterator)->GetChild();
    }
    return NULL;
}


// Return vector of subtrees representing possible subtrees of current file format
std::vector<Node *> Node::GetSubTreesByFile(QString format, QString generalFormat){
    std::vector<Node *> subTrees;

    for(auto branchesIterator = branches.begin(); branchesIterator != branches.end(); ++branchesIterator){
        if(QString::compare((*branchesIterator)->GetStringValue(),format,Qt::CaseInsensitive) == 0){
            subTrees.push_back((*branchesIterator)->GetChild());
            continue;
        }

        if(QString::compare((*branchesIterator)->GetStringValue(),generalFormat,Qt::CaseInsensitive) == 0){
            subTrees.push_back((*branchesIterator)->GetChild());
            continue;
        }

        if((*branchesIterator)->GetStringValue() == "All files"){
            subTrees.push_back((*branchesIterator)->GetChild());
            continue;
        }
    }

    return subTrees;
}

// Creating new branch -> value to compare is string
void Node::SetNewBranch(Node *child,QString value,int oper){
    TreeBranch *newBranch = new TreeBranch();
    newBranch->SetChild(child);
    newBranch->SetStringValue(value);
    newBranch->SetOperand(oper);
    branches.push_back(newBranch);
}

// Creating new branch -> value to compare is date
void Node::SetNewBranch(Node *child,QDate value,int oper){
    TreeBranch *newBranch = new TreeBranch();
    newBranch->SetChild(child);
    newBranch->SetDateValue(value);
    newBranch->SetOperand(oper);
    branches.push_back(newBranch);
}

// Creating new branch -> value to compare is number
void Node::SetNewBranch(Node *child,int value,int oper){
    TreeBranch *newBranch = new TreeBranch();
    newBranch->SetChild(child);
    newBranch->SetIntValue(value);
    newBranch->SetOperand(oper);
    branches.push_back(newBranch);
}

// Creating new branch -> it will be walk through branch
void Node::SetNewBranch(Node *child){
    TreeBranch *newBranch = new TreeBranch();
    newBranch->SetChild(child);
    newBranch->SetWalkThrough();
    branches.push_back(newBranch);
}

int Node::GetBranchesCount(){
    return branches.size();
}

void Node::SetPriority(int p){
    operationInNode->SetPriority(p);
}

bool Node::IsLeaf(){
    if(operationInNode == NULL)
        return false;
    else
        return true;

}

QString Node::GetComparingAttribute(){
    return stuffToCompare;
}

// Method for find branch with exactly same condition
Node * Node::GetBranchNodeWithSameCond(Condition *condition){
    Node *nodeToReturn = NULL;

    // Loop for check all existing branches from current node
    for(auto branchesIterator = branches.begin(); branchesIterator != branches.end(); ++branchesIterator){
        // It must not be walk through node
        if(!(*branchesIterator)->IsWalkThrough()){
            // Check if is it realy same
            if( ((*branchesIterator)->GetType() == condition->type) && ((*branchesIterator)->GetOperand() == condition->operand) ) {
                switch(condition->type){
                    case DATE_TYPE:
                        if(condition->date == (*branchesIterator)->GetDateValue()){
                            nodeToReturn=(*branchesIterator)->GetChild();
                        }
                        break;
                    case NUM_TYPE:
                        if(condition->num == (*branchesIterator)->GetNumericValue()){
                            nodeToReturn=(*branchesIterator)->GetChild();
                        }
                        break;
                    case TEXT_TYPE:
                        if(condition->val == (*branchesIterator)->GetStringValue()){
                            nodeToReturn=(*branchesIterator)->GetChild();
                        }
                        break;
                    default:
                        break;
                }
                // If branch was found, jump out of loop
                if(nodeToReturn != NULL)
                    break;
            }
        }
    }

    return nodeToReturn;
}

// Method to find walk through node with same property to compare
Node * Node::GetWalkThroughBranchNode(QString attr){
    for(auto branchesIterator = branches.begin(); branchesIterator != branches.end(); ++branchesIterator){
        if((*branchesIterator)->IsWalkThrough()){
            if( (*branchesIterator)->GetChildPropertyToCompare() == attr){
                return (*branchesIterator)->GetChild();
            }
        }
    }
    return NULL;
}

// Getting branch at specific postiion
TreeBranch * Node::GetBranchAtPosition(int i){
    return branches.at(i);
}
