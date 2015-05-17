/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"



/// Constructor of main window
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), userInterface(new Ui::MainWindow)
{
    userInterface->setupUi(this);

    InitializeVariables();
    InitializeRulesView();
    SetColorScheme();
    PrintInfo();

}

/// Destructor of main window, clean all used memory
MainWindow::~MainWindow()
{
    delete userInterface;
    delete controller;
    delete loading;
}

/// Print title, app status and loaded plugins
void MainWindow::PrintInfo(){
    this->setWindowTitle("Smart sorter");
    QStringList plugins = controller->GetLoadedPlugins();
    if(!plugins.empty()){
        QString load = "These plugins were loaded: ";
        foreach(QString format, plugins){
            load += " "+format+" ";
        }
        userInterface->logs->insertHtml(load+"<br>");
    }
    userInterface->statusBar->showMessage("Ready");

    if(!controller->IsDuplicityScriptExists()){
        userInterface->logs->insertHtml("Script for delete duplicity files was not found<br>");
        userInterface->delete_duplicate->hide();
    }
}

/// Initialize all variables to start values
void MainWindow::InitializeVariables(){
    folderSelected=false;
    trashSelected=false;
    controller = Control::GetInstance();
    userInterface->source->installEventFilter(this);
    userInterface->trash->installEventFilter(this);
    loading=NULL;
    userInterface->undo_button->hide();
    freeze=false;
}

/// Initialize rules view -> load headers and resize, connect to click event
void MainWindow::InitializeRulesView(){
    QStringList lab;
    lab << "  Name  " << "  Format  " << " Action " << "   Destination  " << QString::fromUtf8("\u25B2") << QString::fromUtf8("\u25bc") << "X ";
    userInterface->rules_view->setHorizontalHeaderLabels(lab);
    userInterface->rules_view->resizeColumnsToContents();
    userInterface->rules_view->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    connect(userInterface->rules_view,SIGNAL(cellClicked(int,int)),this,SLOT(RulesClicked(int,int)));
}

/// Set color scheme of application, will be used for every window and msg box
void MainWindow::SetColorScheme(){
    QPalette p(this->palette());/*
    p.setColor(QPalette::Window, QColor::fromRgb(46,38,51));
    p.setColor(QPalette::Text, QColor::fromRgb(153,23,60));
    p.setColor(QPalette::Button, QColor::fromRgb(85,81,82));
    p.setColor(QPalette::ButtonText, QColor::fromRgb(220,233,190));
    p.setColor(QPalette::WindowText, QColor::fromRgb(160,196,90));*/


    p.setColor(QPalette::Window, QColor::fromRgb(223,226,219));
    p.setColor(QPalette::Text, QColor::fromRgb(25,25,25));
    p.setColor(QPalette::Button, QColor::fromRgb(25,25,25));
    p.setColor(QPalette::Disabled, QPalette::Button, QColor::fromRgb(204,204,204));
    p.setColor(QPalette::ButtonText, QColor::fromRgb(255,240,86));
    p.setColor(QPalette::WindowText, QColor::fromRgb(25,25,25));

    this->setPalette(p);
}

/// Button click for browse and select source folder
void MainWindow::on_select_button_clicked()
{
    if(freeze)
        return;

    QString dir = QFileDialog::getExistingDirectory(this, tr("Select source folder"),
                                                    "",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(dir != ""){
        userInterface->source->setText(dir);
        folderSelected=true;
    }
}

/// Check if work can be started
int MainWindow::WorkCanStart(bool RulesCreated,bool DeleteDuplicity){
    // Source folder was not selected
    if(!folderSelected)
        return NO_SOURCE_FOLDER;

    // Trash folder must be choosen
    if(DeleteDuplicity || controller->IsRulesContainsDelete())
        if(!trashSelected)
            return NO_TRASH_FOLDER;

    // No work was selected
    if(!RulesCreated && !DeleteDuplicity)
        return NO_WORK;

    controller->SetSourcePath(userInterface->source->text(),userInterface->trash->text());
    FreezeGui();
    return 0;
}

void MainWindow::WorkDone(bool DeleteRules){
    if(DeleteRules){
        while(userInterface->rules_view->rowCount() >0)
            userInterface->rules_view->removeRow(0);
    }
    UnFreezeGui();
    userInterface->undo_button->show();
}

/// Function handles click on start button
void MainWindow::on_start_button_clicked()
{
    int retValue = WorkCanStart(controller->RulesCount()>0, userInterface->delete_duplicate->isChecked());

    // Work can be started
    if(retValue == CAN_START){
        // Organize files or delete diplicate files, or both of those two activities
        if(controller->RulesCount()>0 && userInterface->delete_duplicate->isChecked()){
            controller->DeleteDuplicateFiles(userInterface->logs);
            controller->Work(userInterface->logs);
            WorkDone(true);
        }
        else if(userInterface->delete_duplicate->isChecked()){
            controller->DeleteDuplicateFiles(userInterface->logs);
            WorkDone(false);
        }
        else if(controller->RulesCount()>0){
            controller->Work(userInterface->logs);
            WorkDone(true);
        }

    }
    // Otherwise print warning message
    else{
        QMessageBox Msgbox;
        Msgbox.setPalette(this->palette());
        switch(retValue){
            case NO_WORK:
                Msgbox.setText("Nothing to be done");
                break;
            case NO_TRASH_FOLDER:
                Msgbox.setText("Trash folder was not selected");
                break;
            case NO_SOURCE_FOLDER:
                Msgbox.setText("Source folder was not selected");
                break;
        }
        Msgbox.setWindowTitle("Warning");
        Msgbox.exec();
    }
}

/// New rule button click, Its necessary to save and display new rule
void MainWindow::on_new_rule_button_clicked()
{
    NewRuleWindow *dialog;
    dialog = new NewRuleWindow(this);
    dialog->Initialize();
    dialog->exec();

    if(userInterface->rules_view->rowCount() !=controller->RulesCount()){
        RePrintRules();
    }
}


/// User may load rule from saved xml file through this button
void MainWindow::on_load_rule_button_clicked()
{
    LoadRuleWindow *loadrule_dialog;
    loadrule_dialog = new LoadRuleWindow(this);
    loadrule_dialog->Initialize();
    loadrule_dialog->exec();



    if(userInterface->rules_view->rowCount() !=controller->RulesCount()){
        RePrintRules();
    }
}



/// Analyze selected source folder
void MainWindow::on_analyze_clicked()
{
    if(folderSelected){
        FreezeGui();
        QString result = controller->DoTheAnalysis(userInterface->source->text());
        UnFreezeGui();

        QMessageBox Msgbox;
        Msgbox.setPalette(this->palette());
        Msgbox.setText(result);
        Msgbox.setWindowTitle("Analysis result");
        Msgbox.exec();
    }
    else{
        QMessageBox Msgbox;
        Msgbox.setPalette(this->palette());
        Msgbox.setText("Please select source folder to organize");
        Msgbox.setWindowTitle("Warning");
        Msgbox.exec();
    }
}

/// Exit program
void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox;
    msgBox.setPalette(this->palette());
    msgBox.setWindowTitle("Exit");
    msgBox.setText("Are you sure?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes){
        QApplication::quit();
    }
    else
        event->ignore();
}

/// Catch click on table with rules
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress){
        if(obj->objectName() == "source")
            on_select_button_clicked();
        else if(obj->objectName() == "trash")
            on_select_trash_clicked();

    }
    return false;
}

/// Make some changes on table because of new order of rules or delete rule
void MainWindow::RulesClicked(int row, int col){
    if(col == DELETE_BUTTON)
    {
        controller->DeleteRule(row);
        RePrintRules();
    }
    else if(col == UP_BUTTON)
    {
        controller->MoveUpRule(row);
        RePrintRules();
    }
    else if(col==DOWN_BUTTON)
    {
        controller->MoveDownRule(row);
        RePrintRules();
    }

}

/// Print rules in main window in new order
void MainWindow::RePrintRules(){
    userInterface->rules_view->clear();
    while (userInterface->rules_view->rowCount() > 0)
    {
        userInterface->rules_view->removeRow(0);
    }

    QStringList lab;
    lab << "  Name  " << "  Format  " << " Action " << "   Destination  " << QString::fromUtf8("\u25B2") << QString::fromUtf8("\u25bc") << "X ";
    userInterface->rules_view->setHorizontalHeaderLabels(lab);
    userInterface->rules_view->resizeColumnsToContents();

    if(controller->RulesCount()>0)
    {
        for(int position = 0;position<controller->RulesCount();position++){
            int row =userInterface->rules_view->rowCount();
            int col = 0;
            userInterface->rules_view->insertRow(row);
            Rule *rule = controller->ReturnRule(position);

            QTableWidgetItem *qi = new QTableWidgetItem;
            qi->setText(rule->GetName());
            userInterface->rules_view->setItem(row,col++,qi);
            qi = new QTableWidgetItem;
            qi->setText(rule->GetFileFormat());
            userInterface->rules_view->setItem(row,col++,qi);
            qi = new QTableWidgetItem;
            switch(rule->GetOperation()){
                case MOVE:
                    qi->setText("Move");
                    break;
                case COPY:
                    qi->setText("Copy");
                    break;
                case REMOVE:
                    qi->setText("Del");
                    break;
            }

            userInterface->rules_view->setItem(row,col++,qi);
            qi = new QTableWidgetItem;
            qi->setText(rule->GetDestination());
            userInterface->rules_view->setItem(row,col++,qi);
            qi = new QTableWidgetItem;
            qi->setText(QString::fromUtf8("\u25B2"));
            userInterface->rules_view->setItem(row,col++,qi);
            qi = new QTableWidgetItem;
            qi->setText(QString::fromUtf8("\u25BC"));
            userInterface->rules_view->setItem(row,col++,qi);
            qi = new QTableWidgetItem;
            qi->setText("X");
            userInterface->rules_view->setItem(row,col++,qi);
        }
    }
}

/// Undo current operation
void MainWindow::on_undo_button_clicked(){
    FreezeGui();
    controller->UndoOperations();
    UnFreezeGui();
    userInterface->undo_button->hide();
}

/// Clear all created rules
void MainWindow::on_clear_button_clicked(){
    controller->DeleteAllRules();
    RePrintRules();
}

/// Enabled false -> to every buttons etc while doing job
void MainWindow::FreezeGui(){
    freeze=true;
    userInterface->trash->setEnabled(false);
    userInterface->source->setEnabled(false);
    userInterface->empty_trash->setEnabled(false);
    userInterface->select_trash->setEnabled(false);
    userInterface->clear_button->setEnabled(false);
    userInterface->delete_duplicate->setEnabled(false);
    userInterface->analyze->setEnabled(false);
    userInterface->load_rule_button->setEnabled(false);
    userInterface->new_rule_button->setEnabled(false);
    userInterface->rules_view->setEnabled(false);
    userInterface->select_button->setEnabled(false);
    userInterface->source->setEnabled(false);
    userInterface->start_button->setEnabled(false);
    userInterface->undo_button->setEnabled(false);
    userInterface->statusBar->showMessage("Working ...");
    userInterface->loading_label->show();
    if(loading == NULL){
        loading = new QMovie(":images/loader.gif");
        if(loading->isValid())
            userInterface->loading_label->setMovie(loading);
    }
    loading->start();

}

/// Job is done, set everything enabled true
void MainWindow::UnFreezeGui(){
    freeze=false;
    userInterface->trash->setEnabled(true);
    userInterface->source->setEnabled(true);
    userInterface->empty_trash->setEnabled(true);
    userInterface->select_trash->setEnabled(true);
    userInterface->clear_button->setEnabled(true);
    userInterface->delete_duplicate->setEnabled(true);
    userInterface->analyze->setEnabled(true);
    userInterface->load_rule_button->setEnabled(true);
    userInterface->new_rule_button->setEnabled(true);
    userInterface->rules_view->setEnabled(true);
    userInterface->select_button->setEnabled(true);
    userInterface->source->setEnabled(true);
    userInterface->start_button->setEnabled(true);
    userInterface->undo_button->setEnabled(true);
    loading->stop();
    userInterface->loading_label->hide();
    userInterface->statusBar->showMessage("Ready");
}

/// Clicked on empty button -> delete all files in trash
void MainWindow::on_empty_trash_clicked()
{
    if(userInterface->trash->text() == ""){
        QMessageBox Msgbox;
        Msgbox.setPalette(this->palette());
        Msgbox.setText("Please choose trash folder");
        Msgbox.setWindowTitle("Warning");
        Msgbox.exec();
        return;
    }

    QDir folder(userInterface->trash->text());
    folder.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    // TODO get file num
    QMessageBox msgBox;
    msgBox.setPalette(this->palette());
    msgBox.setWindowTitle("Empty trash");
    msgBox.setText("Are you sure? "+ QString::number(folder.count()) +" files will be permanently deleted");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    if(msgBox.exec() == QMessageBox::Yes){
        QStringList files = folder.entryList();

        controller->EmptyTrash(userInterface->logs,files, userInterface->trash->text());
    }
}

/// Folder select dialog for trash folder
void MainWindow::on_select_trash_clicked()
{
    if(freeze)
        return;
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select source folder"),
                                                    "",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(dir != ""){
        userInterface->trash->setText(dir+"/");
        trashSelected=true;
    }
}
