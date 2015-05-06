/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#ifndef LOADRULE_H
#define LOADRULE_H

#include <QDialog>
#include <QFileDialog>

#include "ui_loadrule.h"
#include "code/control.h"


class loadrule : public QDialog, public Ui::loadrule
{
    Q_OBJECT
    
public:
    loadrule(QWidget *parent = 0);
    void Initialize();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_pushButton_clicked();

private:
    Control *C;
    bool fileSelected;
    QString contentOfXML;
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // LOADRULE_H
