/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#ifndef NEWRULE_H
#define NEWRULE_H

#include <ui_dialog.h>
#include <QDialog>
#include <QDebug>
#include <QStringList>
#include <QFileDialog>
#include <QMessageBox>

#include "code/control.h"
#include "code/rule.h"
#include "newdistribution.h"

/*! \brief Window to create new rule
 *
 * Methods that provides data for window where new rule can be created
 */
class NewRuleWindow : public QDialog, public Ui::Dialog
{
    Q_OBJECT
public:
    NewRuleWindow(QWidget *parent = 0);
    void Initialize();
    bool Confirm();

private slots:

    void CondsClicked(int,int);
    void on_property_select_currentIndexChanged(const QString &arg1);
    void on_fileformat_select_currentIndexChanged(const QString &arg1);
    void on_destination_browse_clicked();
    void on_saveButton_clicked();
    void on_addButton_clicked();
    void on_cancelButton_clicked();
    void on_add_one_clicked();
    void on_operation_with_file_currentIndexChanged(const QString &arg1);
    void on_distribution_button_clicked();

private:
    Control *controller;
    bool folderSelected;
    Rule* CreateRule();
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // NEWRULE_H
