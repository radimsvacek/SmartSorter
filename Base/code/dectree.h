/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#ifndef dectree_h
#define dectree_h

#include "file.h"
#include "node.h"
#include "basic_classif.h"
#include "rule.h"
#include "command.h"
#include "commandmanager.h"

#include <QTextBrowser>
#include <QDir>
#include <QProcess>
#include <QMessageBox>
#include <QDebug>
#include <iostream>

#define REFRESH_GUI 5

/****************************/
// DecTree
class DecTree{
	public:
        DecTree(BasicClassification *);
		~DecTree();

        void DoTheMagic(File *);
        void BuildTree(std::vector<Rule *>);
		void RemoveTree();
        void Initialize(QString, QTextBrowser *);
        void UndoOperations();
        void DeleteDuplicityFiles(QString);
        bool IsDuplicityScriptExists();
        void EmptyTrash(QStringList,QString);

	private:
        int timeForScript = 10000;
        Node * CreateBranchAndNode(Node * ,Condition *);
        std::vector<Node *> FindSubTrees(QString);
        Operation * FindBestResult(File *, Node *);
        void DoTheActionWithFile(File *,Operation *);

        Node *root;
        BasicClassification *classify;
        CommandManager *commandManager;
        QTextBrowser *logsOut;
        QString trashPath;
        QString duplicityScriptPath = "/usr/local/bin/tool/fdupe.pl";
        QString duplicityScriptPath2 = "tool/fdupe.pl";
};

#endif
