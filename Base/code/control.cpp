/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#include "control.h"
#include "QTime"

bool Control::instanceFlag = false;

Control* Control::single = NULL;

Control* Control::GetInstance()
{
    if(!instanceFlag && single == NULL)
    {
        single = new Control();
        instanceFlag = true;
        return single;
    }
    else
    {
        return single;
    }
}


/// Constructor
Control::Control(){
	// New instance of file lookup engine
    lookUp = new LookUp();
    // Create instance of basic_classif and childs (PDF, TXT, ...)
    // then pointer will be send to dectree
	// Instance of decision tree
    classify = new BasicClassification();
    decisionTree = new DecTree(classify);
    analyze = new Analysis();

}

/// Delete instances of lookup and decision tree
Control::~Control(){
    delete lookUp;
    delete decisionTree;
    delete analyze;

    for(auto it = rules.begin(); it != rules.end(); ++it) {
        delete (*it);
    }
    rules.clear();

    instanceFlag = false;// SINGLETON
}

/// Run the analysis of source folder
QString Control::DoTheAnalysis(QString folder){

    lookUp->SetPath(folder);
    analyze->NullIt();
    File *fileToClassify;
    int counter = 0;
    // Do the analysis of folder
    while((fileToClassify = lookUp->GetNextFile()) != NULL){
        if(counter++ > REFRESH_GUI_ANALYSIS){
            qApp->processEvents();
            counter=0;
        }
        analyze->AddFile(fileToClassify->GetFileInfo());
        delete fileToClassify;
    }
    return analyze->PrintResults();

}

void Control::SetSourcePath(QString path, QString trash){
    sourcePath=path;
    trashPath = trash;
}

/*! \brief Main work of app
 *
 * Preparation to work -> build tree then work and final remove tree
 * in cycle sending files to decision tree, whichc make magic with it
 */
void Control::Work(QTextBrowser *logsOut){
    // Build decision tree based on user's wish
    decisionTree->BuildTree(rules);
    decisionTree->Initialize(trashPath,logsOut);
	// Start look up in selected folder
    lookUp->SetPath(sourcePath);

	// Pointer to instance of file object
    File *fileToClassify;

    QTime start;
    start.start();
    int counter = 0;

	// Infinity loop to look-up all files 
    while((fileToClassify = lookUp->GetNextFile()) != NULL){
        if(counter++ > REFRESH_GUI){
            qApp->processEvents();
            counter=0;
        }
        decisionTree->DoTheMagic(fileToClassify);
	}	

    // Time needed to work
    //qDebug() << start.elapsed();

    // Remove tree
    decisionTree->RemoveTree();

    for(auto it = rules.begin(); it != rules.end(); ++it) {
        delete (*it);
    }
    rules.clear();

}

/// Call perl script that call FDUPE, which find all duplicate files than delete all of duplicates except one
void Control::DeleteDuplicateFiles(QTextBrowser *logsOut){
    decisionTree->Initialize(trashPath,logsOut);
    decisionTree->DeleteDuplicityFiles(sourcePath);
}

/// Delete all files in trash folder
void Control::EmptyTrash(QTextBrowser *logsOut, QStringList files,QString path){
    decisionTree->Initialize(trashPath,logsOut);
    decisionTree->EmptyTrash(files,path);
}

/// Undo operations that was made
void Control::UndoOperations(){
    decisionTree->UndoOperations();
}

/***/// Methods for GUI
QStringList Control::GetLoadedPlugins(){
    return classify->GetLoadedPlugins();
}

QStringList Control::GetProperties(QString arg){
    return classify->GetProperties(arg);
}

QStringList Control::GetPropertiesForDistribution(QString format){
    return classify->GetPropertiesForDistribution(format);
}

std::vector<int> Control::GetPropertyOperations(QString property,QString fileFormat){
    return classify->GetPropertyOperations(property,fileFormat);
}
int Control::GetPropertyType(QString arg, QString fileFormat){
    return classify->GetPropertyType(arg, fileFormat);
}
QStringList Control::GetFileFormats(){
    return classify->GetFileFormats();
}

void Control::AddRule(Rule *r){
    rules.push_back(r);
}
int Control::RulesCount(){
    return rules.size();
}

Rule * Control::ReturnRule(int position){
    return rules.at(position);
}
void Control::DeleteRule(int position){
    Rule *rule = ReturnRule(position);
    rules.erase(rules.begin()+position);
    delete rule;
}

void Control::MoveUpRule(int position){
    if(position!=0)
        iter_swap(rules.at(position),rules.at(position-1));
}

void Control::MoveDownRule(unsigned position){
    position+=1;
    if(position<rules.size())
        iter_swap(rules.at(position),rules.at(position+1));
}

bool Control::IsRulesContainsDelete(){
    for(std::vector<Rule *>::iterator it = rules.begin(); it != rules.end(); ++it){
        if((*it)->GetOperation() == REMOVE)
            return true;
    }
    return false;
}

void Control::DeleteAllRules(){
    rules.clear();
}

bool Control::IsDuplicityScriptExists(){
    return decisionTree->IsDuplicityScriptExists();
}



