/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#include "dectree.h"

DecTree::DecTree(BasicClassification * BC){
    classify = BC;
    commandManager = new CommandManager();
}

DecTree::~DecTree(){
    delete classify;
    delete commandManager;
}

void DecTree::Initialize(QString path, QTextBrowser * logs){
    trashPath = path;
    logsOut=logs;
}

/// File goes through decision tree, execute proper operation with file
void DecTree::DoTheMagic(File *file){
    // Some variables that is needed in method
    QString fileFormat = file->GetExtension();
    Operation *operation = NULL;
    Operation *help = NULL;
    std::vector<Node *> subTrees;

    // Find all subtrees where can be founded best result
    subTrees = FindSubTrees(fileFormat);

    // goes through all subtrees and find best result
    for(std::vector<Node *>::iterator subTree = subTrees.begin(); subTree != subTrees.end(); ++subTree){
        help = FindBestResult(file,(*subTree));

        if(operation==NULL)
            operation=help;
        else if (help == NULL)
            ;
        else{
            if(help->GetPriority() < operation->GetPriority())
                operation = help;
        }

    }

    // If exists action for that file, do it!
    if(operation != NULL){
        DoTheActionWithFile(file,operation);
    }

    delete file;
}

/// Goes through all branches of root and find these which is able to specific file format
std::vector<Node *> DecTree::FindSubTrees(QString format){
    QString generalFormat = classify->GetGeneralFileFormat(format);
    return root->GetSubTreesByFile(format,generalFormat);
}

/// Method for finding best result in tree
Operation * DecTree::FindBestResult(File *file, Node *subRoot){

    // Some variables needed
    Operation *bestResult = NULL;
    Operation *helpResult = NULL;
    TreeBranch *branch = NULL;

    // If subroot which is get there as attribute is leaf, save its operation
    if(subRoot->IsLeaf()){
        bestResult = subRoot->GetOperation();
    }

    // Recursively goes through tree and find best result
    for(int i = 0;i<subRoot->GetBranchesCount();i++){
        // goes through every branch in this node
        branch = subRoot->GetBranchAtPosition(i);
        // If file fullfiled condition in this branch, continue to the child
        if(classify->IsFileFullfilCondition(branch,file,subRoot->GetComparingAttribute())){

            helpResult = FindBestResult(file,branch->GetChild());
            // Compare results of finding
            if(helpResult != NULL){
                if(bestResult==NULL)
                    bestResult=helpResult;
                else if(helpResult->GetPriority() < bestResult->GetPriority()){
                    bestResult=helpResult;
                }
            }
        }
    }

    return bestResult;
}

/// Do action with file based on operation founded in decision tree
void DecTree::DoTheActionWithFile(File *file,Operation *op){

    // Get exact distribution folder
    QString distribution = op->GetDistribution();
    distribution = classify->GetDistributionPath(distribution,file);

    // Create this folder if not exists
    if(op->GetOperation()!=REMOVE){
        QDir dir(op->GetDestination()+distribution);
        if(!dir.exists())
            dir.mkpath(".");
    }

    // Create command based on specific operation and execute it
    Command *command = NULL;
    switch(op->GetOperation()){
        case MOVE:
            command = new MoveCommand;
            command->Initialize(file->GetName(),file->GetPath(),op->GetDestination()+distribution,logsOut);
            break;
        case COPY:
            command = new CopyCommand;
            command->Initialize(file->GetName(),file->GetPath(),op->GetDestination()+distribution,logsOut);
            break;
        case REMOVE:
            command = new DeleteCommand;
            command->Initialize(file->GetName(),file->GetPath(),trashPath,logsOut);
            break;
    }
    commandManager->ExecuteCommand(command);

    return;
}

/// Undo all operations that was made
void DecTree::UndoOperations(){
    commandManager->UndoCommands();
    commandManager->ClearCommands();
}

/// Method for building tree from rules vector
void DecTree::BuildTree(std::vector<Rule *> rules){
    // Clear command in case of previous work
    commandManager->ClearCommands();
	// Create root of decision tree
    root = new Node();
    root->SetParent(NULL);
    root->SetAttributeToCompare("Format");
    Condition *condition=NULL;
    QString fileFormat;
    Node *nextNode;
    Node *nextNextNode;
    int rulePriority = 0;

    // Build rest of tree from rules
    /////////////////////////////////
    // Start loop for each condition
    // In cycle through all created rules
    for(auto ruleIterator = rules.begin(); ruleIterator != rules.end(); ++ruleIterator){

        // Counter of rules -> lower number means higher priority
        rulePriority++;
        // First branch is for file format
        // Get first child node or create new node for that file format
        fileFormat = (*ruleIterator)->GetFileFormat();
        nextNode = root->GetFileFormatNode(fileFormat);
        if(nextNode==NULL){
            nextNode = new Node();
            nextNode->SetParent(root);
            root->SetNewBranch(nextNode,fileFormat,NO_OPERATOR);
        }

        // In case there is no other conditions
        if((*ruleIterator)->GetConditionsCount() == 0){
            if(!nextNode->IsLeaf()){
                nextNode->SetOperation((*ruleIterator)->GetOperation(),(*ruleIterator)->GetDestination(),(*ruleIterator)->GetDistribution());
                nextNode->SetPriority(rulePriority);
            }
        }
        // There is no other branches, so it will be created automaticaly one node after another
        else if(nextNode->GetBranchesCount() == 0){
            // creating nodes in loop
            while((condition = (*ruleIterator)->GetNextCondition()) != NULL){
                nextNode->SetAttributeToCompare(condition->property);
                // Create new node and branch from parent node to that new child node
                nextNextNode = CreateBranchAndNode(nextNode,condition);
                nextNode=nextNextNode;
            }
            // Create leaf node with opearation for that rule
            if(!nextNode->IsLeaf()){
                nextNode->SetOperation((*ruleIterator)->GetOperation(),(*ruleIterator)->GetDestination(),(*ruleIterator)->GetDistribution());
                nextNode->SetPriority(rulePriority);
            }
        }

        //////////////////////////////////////////////////////////////
        // There is another branches from that file format part of tree
        // and also is there another conditions to add into tree
        // Is necessary to find similar conditions to expand tree
        else{
            // Expand existing branches until there is branches to expand and conditions to add
            while( (nextNode->GetBranchesCount() != 0) && ((*ruleIterator)->GetConditionsCount() != 0) ){
                // Finding out if exist condition to compare same attribute as current node
                QString attribute = nextNode->GetComparingAttribute();
                condition = (*ruleIterator)->GetSpecificCondition(attribute);

                // There exists condition with same attribute to compare
                if(condition != NULL){
                    // Check if exists branch with exactly same value and operand -> then continue to this node
                    nextNextNode = nextNode->GetBranchNodeWithSameCond(condition);

                    // Else create new branch and new node
                    if(nextNextNode==NULL){
                        // Create new node and branch from parent node to that new child node
                        nextNextNode = CreateBranchAndNode(nextNode,condition);
                    }
                    nextNode=nextNextNode;
                }

                // There is no condition with same attribute to compare, so it must be created a non comparing branch
                else{
                    // At first load some condition
                    condition = (*ruleIterator)->GetNextCondition();
                    // Check if exists walk through node with same property to compare
                    nextNextNode = nextNode->GetWalkThroughBranchNode(condition->property);

                    // If exists, move to this node and create new branch
                    if(nextNextNode != NULL){
                        // Move to child node
                        nextNode=nextNextNode;
                        // Check if exists branch with exactly same value and operand -> then continue to this node
                        nextNextNode = nextNode->GetBranchNodeWithSameCond(condition);
                        // Else create new branch and new node
                        if(nextNextNode==NULL){
                             // Create new node and branch from parent node to that new child node
                            nextNextNode = CreateBranchAndNode(nextNode,condition);
                        }
                        nextNode=nextNextNode;
                    }

                    // If does not exists it's necessary to create walk through node
                    else{
                        // Creating walk through node and move to this node
                        nextNextNode = new Node();
                        nextNextNode->SetParent(nextNode);
                        nextNextNode->SetAttributeToCompare(condition->property);
                        nextNode->SetNewBranch(nextNextNode);
                        nextNode=nextNextNode;

                        // Then create branch and new node from current condition
                        nextNextNode = CreateBranchAndNode(nextNode,condition);
                        // Move to child node
                        nextNode=nextNextNode;
                    }
                }
           }

           // After find node that have not got another branches then add rest of conditions rule by rule
           // also node does not have choosen a property which will be comparing
           while((condition = (*ruleIterator)->GetNextCondition()) != NULL){
               nextNode->SetAttributeToCompare(condition->property);
               nextNextNode = CreateBranchAndNode(nextNode,condition);
               nextNode=nextNextNode;
           }

           // Create leaf node with opearation for that rule
           if(!nextNode->IsLeaf()){
               nextNode->SetOperation((*ruleIterator)->GetOperation(),(*ruleIterator)->GetDestination(),(*ruleIterator)->GetDistribution());
               nextNode->SetPriority(rulePriority);
           }
        }
    }
}


/// Create and return child node, create new branch from parent node to newly created child node
Node * DecTree::CreateBranchAndNode(Node * Parent,Condition *condition){
    Node * nextNextNode = new Node();
    nextNextNode->SetParent(Parent);
    switch(condition->type){
        case DATE_TYPE:
            Parent->SetNewBranch(nextNextNode,condition->date,condition->operand);
            break;
        case NUM_TYPE:
            Parent->SetNewBranch(nextNextNode,condition->num,condition->operand);
            break;
        case TEXT_TYPE:
            Parent->SetNewBranch(nextNextNode,condition->val,condition->operand);
            break;
        case TRUE_TYPE:
            Parent->SetNewBranch(nextNextNode,condition->boolean,condition->operand);
            break;
        default:
            break;
    }
    return nextNextNode;
}

/// Recursively delete rest of tree
void DecTree::RemoveTree(){
    // Root->PrintTree();
    root->DeleteTree();
}

/// Check if script for duplicity really exists
bool DecTree::IsDuplicityScriptExists(){
    return (QFile(duplicityScriptPath).exists() || QFile(duplicityScriptPath2).exists());
}

/// Delete all duplicity files exclude one
void DecTree::DeleteDuplicityFiles(QString path){
    QString scriptPath;

    if(QFile(duplicityScriptPath).exists())
        scriptPath = duplicityScriptPath;
    else if(QFile(duplicityScriptPath2).exists())
        scriptPath = duplicityScriptPath2;
    else return;


    // Clear command in case of previous work
    commandManager->ClearCommands();
    Command *command = NULL;

    QStringList params;
    params << scriptPath << path;
    QProcess deleteScript;
    deleteScript.start("perl",params);

    if(!deleteScript.waitForStarted(timeForScript)){
        logsOut->insertHtml("Error durring running duplicity script <br>");
        return;
    }

    deleteScript.waitForFinished(-1);
    QString output(deleteScript.readAllStandardOutput());

    QStringList files = output.split("\n");
    int counter = 0;

    foreach(QString line,files){
        if(line.isEmpty())
            continue;

        qDebug() << line;

        if(counter++ > REFRESH_GUI){
            qApp->processEvents();
            counter=0;
        }

        QFile file(line);
        QFileInfo fileInfo(file.fileName());
        command = new DeleteCommand;
        command->Initialize(fileInfo.fileName(),line,trashPath,logsOut);
        commandManager->ExecuteCommand(command);
    }

}


/// Delete all files in trash
void DecTree::EmptyTrash(QStringList files, QString path){
    int counter = 0;
    foreach(QString str,files){
        if(counter++ > REFRESH_GUI){
            qApp->processEvents();
            counter=0;
        }
        logsOut->insertHtml("File " + path+str + " was permanently deleted<br>");
        QFile::remove(path+str);
    }
}

