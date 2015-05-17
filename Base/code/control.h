/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#ifndef control_h
#define control_h

#include "basic_classif.h"
#include "lookup.h"
#include "dectree.h"
#include "file.h"
#include "ui_mainwindow.h"
#include "rule.h"

#include <vector>
#include <QString>
#include <QPalette>
#include <QProcess>
#include <QString>
#include <QMessageBox>
#include <QDebug>


#define REFRESH_GUI_ANALYSIS 1000

/****************************/
// Control
/*! \brief Main class of app
 *
 * Cornerstone of application, communicate with GUI and provides services of backend
 */
class Control{
	public:
        static Control* GetInstance();

        ~Control();
        QString DoTheAnalysis(QString);

        void SetSourcePath(QString,QString);
        void Work(QTextBrowser *);
        void DeleteDuplicateFiles(QTextBrowser *);
        void EmptyTrash(QTextBrowser *,QStringList,QString);

        // Methods for comunication with GUI
        QStringList GetLoadedPlugins();
        QStringList GetPropertiesForDistribution(QString);
        QStringList GetProperties(QString);
        QStringList GetFileFormats();
        std::vector<int> GetPropertyOperations(QString,QString);
        int GetPropertyType(QString, QString);
        void AddRule(Rule*);
        int RulesCount();
        Rule * ReturnRule(int);
        void DeleteRule(int);
        void MoveUpRule(int);
        void MoveDownRule(unsigned);
        void UndoOperations();
        void DeleteAllRules();
        bool IsRulesContainsDelete();
        bool IsDuplicityScriptExists();

	private:
        static bool instanceFlag;
        static Control *single;
        Control();

        QPalette palette;
        QString sourcePath;
        QString trashPath;

        Analysis *analyze;
        LookUp *lookUp;
        DecTree *decisionTree;
        BasicClassification *classify;
        Ui::MainWindow *GUI;
        std::vector<Rule *> rules;
};
#endif
