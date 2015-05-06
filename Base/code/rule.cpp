/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#include "rule.h"
#include "control.h"

Rule::Rule()
{
}

Rule::~Rule()
{
    for(auto it = conds.begin(); it != conds.end();it++){
        delete (*it);
    }
    conds.clear();
}

int Rule::SaveRule(){

    QFile file(name+".xml");
    if(file.exists())
        return FILE_ERROR;

    if (!file.open(QIODevice::ReadWrite)) {
        return XML_ERROR;
    }
    QTextStream stream(&file);
    stream << "<?xml version=\"1.0\"?>" << endl;

    QString XML;
    XML += "<rule>";
    XML += "<name>" + name + "</name>\n";
    XML += "<FileFormat>" + fileFormat + "</FileFormat>\n";
    XML += "<operation>" + QString::number(operation) + "</operation>\n";
    XML += "<destination>" + destination + "</destination>\n";
    XML += "<distribution>" + distribution + "</distribution>\n";

    XML += "<conditions>\n";
    for(int i = 0; i < GetConditionsCount(); i++){
        XML += "<condition>\n";
        XML += "<property>" + conds[i]->property + "</property>\n";
        XML += "<operand>" + QString::number(conds[i]->operand) + "</operand>\n";
        XML += "<type>" + QString::number(conds[i]->type) + "</type>\n";
        XML += "<value>";
        switch(conds[i]->type){
            case DATE_TYPE:
                XML += conds[i]->date.toString("dd.MM.yyyy");
                break;
            case NUM_TYPE:
                XML += QString::number(conds[i]->num);
                break;
            case TEXT_TYPE:
                XML += conds[i]->val;
                break;
            case TRUE_TYPE:
                XML += QString::number(conds[i]->boolean);
                break;
            default:
                break;
        }
        XML += "</value>\n";
        XML += "</condition>\n";
    }
    XML += "</conditions>\n";
    XML += "</rule>";
    stream << XML;
    return RULE_LOADED;

}

// Load rule from XML file
int Rule::LoadRule(QString nameRule){
    QDomDocument doc(nameRule);
    QFile file(nameRule);
    if (!file.open(QIODevice::ReadOnly))
        return FILE_ERROR;
    if (!doc.setContent(&file)) {
        file.close();
        return XML_ERROR;
    }
    file.close();

    QDomElement docElem = doc.documentElement();
    QDomNode conditions;
    QDomNode oneCondition;
    QDomNode n = docElem.firstChild();
    QString helping;
    int typeBefore = -1;

    // Start parsing XML document
    while(!n.isNull()) {
        // First element
        QDomElement e = n.toElement();
        if(!e.isNull()) {

            // Load properties
            if (e.tagName() == "name")
                name = e.text();

            if (e.tagName() == "FileFormat")
                fileFormat = e.text();

            if (e.tagName() == "operation")
                operation = e.text().toInt();

            if (e.tagName() == "destination")
                destination = e.text();

            if (e.tagName() == "distribution")
                distribution = e.text();

            if (e.tagName() == "name")
                name = e.text();

            // Conditions
            if (e.tagName() == "conditions"){
                conditions = e.firstChild();
                // Goes through list of conditions
                while(!conditions.isNull()){
                    e = conditions.toElement();

                    if(!e.isNull()) {
                        // One condition specification
                        oneCondition = conditions.firstChild();
                        Condition* cond = new Condition();
                        while(!oneCondition.isNull()) {
                            e = oneCondition.toElement();
                            if(!e.isNull()) {
                                if (e.tagName() == "property")
                                    cond->property = e.text();
                                if (e.tagName() == "operand")
                                    cond->operand = e.text().toInt();
                                if (e.tagName() == "type"){
                                    typeBefore = e.text().toInt();
                                    cond->type = e.text().toInt();
                                }
                                if (e.tagName() == "value"){
                                    if(typeBefore!=-1){
                                        switch(cond->type){
                                            case DATE_TYPE:
                                                cond->date = QDate::fromString(e.text(),"dd.MM.yyyy");
                                                break;
                                            case NUM_TYPE:
                                                cond->num = e.text().toInt();
                                                break;
                                            case TEXT_TYPE:
                                                cond->val = e.text();
                                                break;
                                            case TRUE_TYPE:
                                                cond->boolean = e.text().toInt();
                                                break;
                                            default:
                                                break;
                                        }
                                    }
                                    else{
                                        helping=e.text();
                                        typeBefore=-2;
                                    }

                                }
                            }
                            oneCondition = oneCondition.nextSibling();
                        }
                        if(typeBefore==-2){
                            switch(cond->type){
                                case DATE_TYPE:
                                    cond->date = QDate::fromString(helping,"dd.MM.yyyy");
                                    break;
                                case NUM_TYPE:
                                    cond->num = helping.toInt();
                                    break;
                                case TEXT_TYPE:
                                    cond->val = helping;
                                    break;
                                case TRUE_TYPE:
                                    cond->boolean = helping.toInt();
                                    break;
                                default:
                                    break;
                            }
                        }
                        conds.push_back(cond);
                    }
                    conditions =conditions.nextSibling();
                }
            }
        }
        n = n.nextSibling();
    }

    if(!IsRuleValid())
        return XML_ERROR;

    return RULE_LOADED;
}

// Check if created rule is valid
bool Rule::IsRuleValid(){
    // initialize
    Control *controller = Control::GetInstance();
    QStringList check;
    QStringList help;

    // FileFormat check
    check = controller->GetFileFormats();
    if(!check.contains(fileFormat))
        return false;

    // Operation check
    if(!(operation==MOVE || operation==REMOVE || operation == COPY))
        return false;


    // Destination check
    if(operation != REMOVE){
        if(!QDir(destination).exists())
            return false;
    }

    // Distribution check
    if(!(distribution.isEmpty() || distribution.isNull())){
        check = distribution.split("/");
        help = controller->GetPropertiesForDistribution(fileFormat);
        foreach(QString str,check){
            if(str=="")
                return false;
            if(!help.contains(str))
                return false;
        }
    }
    // Condition check
    std::vector<int> operands;
    check = controller->GetProperties(fileFormat);


    for(auto conditionsIterator = conds.begin(); conditionsIterator != conds.end();conditionsIterator++){

        // Check for property
        if(!check.contains((*conditionsIterator)->property))
            return false;

        // Check for operand
        operands = controller->GetPropertyOperations((*conditionsIterator)->property,fileFormat);
        if(std::find(operands.begin(),operands.end(),(*conditionsIterator)->operand) == operands.end())
            return false;

        // Check for type
        if(!((*conditionsIterator)->type ==DATE_TYPE || (*conditionsIterator)->type==NUM_TYPE || (*conditionsIterator)->type == TEXT_TYPE || (*conditionsIterator)->type == TRUE_TYPE))
            return false;

        // Check for value
        switch((*conditionsIterator)->type){
            case DATE_TYPE:
                if(!((*conditionsIterator)->date.isValid() && !(*conditionsIterator)->date.isNull()))
                    return false;
                break;
            case TEXT_TYPE:
                if((*conditionsIterator)->val.isNull() || (*conditionsIterator)->val.isEmpty())
                    return false;
                break;
            default:
                break;
        }

    }

    return true;
}

// Create new condition
Condition * Rule::SetCond(int op,QString prop,int tp){
    Condition *condition = new Condition();
    condition->operand=op;
    condition->property=prop;
    condition->type=tp;
    conds.push_back(condition);
    return condition;
}

void Rule::SetValue(QDate date,Condition *condition){
    condition->date=date;

}

void Rule::SetValue(QString string,Condition *condition){
    condition->val=string;
}

void Rule::SetValue(bool val,Condition *condition){
    condition->boolean=val;
}

void Rule::SetValue(int number,Condition *condition){
    condition->num=number;
}

void Rule::SetOperation(int oper){
    operation=oper;
}

void Rule::SetDestination(QString de,QString di){
    destination=de;
    distribution=di;
}
void Rule::SetFileFormat(QString n,QString ff){
    fileFormat=ff;
    name=n;
}

QString Rule::GetFileFormat(){
    return fileFormat;
}

QString Rule::GetName(){
    return name;
}

int Rule::GetOperation(){
    return operation;
}

QString Rule::GetDestination(){
    return destination;
}

QString Rule::GetDistribution(){
    return distribution;
}

// Return next condition in rule, also remove it from vector
Condition * Rule::GetNextCondition(){
    if(conds.size() > 0){
        Condition *c = conds.back();
        conds.pop_back();
        return c;
    }
    else{
        return NULL;
    }
}

int Rule::GetConditionsCount(){
    return conds.size();
}

// Finding condition in vector with specific attribute to compare
Condition * Rule::GetSpecificCondition(QString attribute){
    Condition *conditionToReturn = NULL;

    if(attribute.isEmpty())
        return NULL;

    for(auto conditionsIterator = conds.begin(); conditionsIterator != conds.end(); ++conditionsIterator){
        if((*conditionsIterator)->property == attribute){
            conditionToReturn = (*conditionsIterator);
            conds.erase(conditionsIterator);
            break;
        }
    }

    return conditionToReturn;
}
