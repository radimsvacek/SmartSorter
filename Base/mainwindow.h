/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMovie>
#include <QFileDialog>
#include <QDialog>
#include <QMessageBox>
#include <QCloseEvent>

#include "loadrule.h"
#include "code/control.h"
#include "newrule.h"
#include "code/control.h"

#define DELETE_BUTTON 6
#define UP_BUTTON 4
#define DOWN_BUTTON 5

#define CAN_START 0
#define NO_SOURCE_FOLDER 1
#define NO_TRASH_FOLDER 2
#define NO_WORK 3

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *userInterface;
    bool trashSelected;
    bool folderSelected;
    Control *controller;
    int rules;
    QMovie *loading;

    void closeEvent(QCloseEvent *);
    bool eventFilter(QObject *obj, QEvent *event);
    void RePrintRules();
    void FreezeGui();
    void UnFreezeGui();

    void WorkDone(bool);
    int WorkCanStart(bool,bool);
    void PrintInfo();
    void InitializeVariables();
    void InitializeRulesView();
    void SetColorScheme();

    bool freeze;
private slots:

    void RulesClicked(int,int);

    void on_select_button_clicked();
    void on_start_button_clicked();
    void on_new_rule_button_clicked();
    void on_load_rule_button_clicked();
    void on_analyze_clicked();
    void on_undo_button_clicked();
    void on_clear_button_clicked();
    void on_empty_trash_clicked();
    void on_select_trash_clicked();
};

#endif // MAINWINDOW_H
