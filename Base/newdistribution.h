/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#ifndef NEWDISTRIBUTION_H
#define NEWDISTRIBUTION_H

#include <QDialog>

#include "ui_newdistribution.h"
#include "code/control.h"

/*! \brief New distribution window
 *
 * Methods that provides data for window where distribution can be selected
 */
class NewDistributionWindow : public QDialog, public Ui::newdistribution
{
    Q_OBJECT
    
public:
    NewDistributionWindow(QWidget *parent = 0);
    void SetResultString(QString *);
    void LoadProperties(QString);
    void Initialize();

private:
    Control *controller;
    QString*  resultStr;
    QString fileFormat;

private slots:

    void AttributeClicked(int,int);
    void on_clearButton_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif // NEWDISTRIBUTION_H
