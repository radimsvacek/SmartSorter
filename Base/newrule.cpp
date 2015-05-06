/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#include "newrule.h"


// Cnstructor .. only some stuff
NewRule::NewRule(QWidget * parent) : QDialog(parent)
{
    setupUi(this);
    setWindowTitle("New rule");
    folderSelected=false;
    // Tady mam list vsech obecnych vlastnosti

    destination->installEventFilter(this);

    connect(cond_table,SIGNAL(cellClicked(int,int)),this,SLOT(CondsClicked(int,int)));

    QPalette p(this->palette());
    p.setColor(QPalette::Window, QColor::fromRgb(223,226,219));
    p.setColor(QPalette::Text, QColor::fromRgb(25,25,25));
    p.setColor(QPalette::Button, QColor::fromRgb(25,25,25));
    p.setColor(QPalette::ButtonText, QColor::fromRgb(255,240,86));
    p.setColor(QPalette::WindowText, QColor::fromRgb(25,25,25));
    this->setPalette(p);
}

// Load pointer to control object
// Also preparation of dialog
void NewRule::Initialize(){

    inserted_value->show();
    inserted_date->hide();
    inserted_num->hide();
    inserted_bool->hide();

    controller=Control::GetInstance();

    QStringList p;
    p = controller->GetFileFormats();
    fileformat_select->addItem("");
    fileformat_select->addItems(p);

    operation_with_file->addItem("");
    operation_with_file->addItem("Move");
    operation_with_file->addItem("Copy");
    operation_with_file->addItem("Delete");

    cond_table->setColumnWidth(0,65);
    cond_table->setColumnWidth(1,60);
    cond_table->setColumnWidth(2,68);
    cond_table->setColumnWidth(3,10);
}

// Select property to compare
void NewRule::on_property_select_currentIndexChanged(const QString &arg1)
{
    // Clear operation select box & value box
    operation_select->clear();
    inserted_value->clear();
    inserted_date->clear();
    inserted_num->clear();


    // Load possible operations
    if(arg1==""){
        operation_select->clear();
    }
    else{
        inserted_value->hide();
        inserted_date->hide();
        inserted_num->hide();
        inserted_bool->hide();
        std::vector<int> ops = controller->GetPropertyOperations(arg1,fileformat_select->currentText());
        int type = controller->GetPropertyType(arg1,fileformat_select->currentText());
        // According to selected property type is shown right type of input
        switch(type){
            case DATE_TYPE:
                inserted_date->show();
                    break;
            case NUM_TYPE:
                inserted_num->show();
                break;
            case TEXT_TYPE:
                inserted_value->show();
                break;
            case TRUE_TYPE:
                inserted_bool->show();
                break;
            default:
                break;
        }
        operation_select->addItem("");

        // Load operands to selected property
        for(std::vector<int>::iterator it = ops.begin() ; it != ops.end() ; it++){
            switch(*it){
                case LESS:
                    operation_select->addItem("<");
                    break;
                case LESS_EQUAL:
                    operation_select->addItem("<=");
                    break;
                case EQUAL:
                    operation_select->addItem("=");
                    break;
                case MORE:
                    operation_select->addItem(">");
                    break;
                case MORE_EQUAL:
                    operation_select->addItem(">=");
                    break;
                case CONTAINS:
                    operation_select->addItem("Contains");
                    break;
                case NOT_CONTAINS:
                    operation_select->addItem("Not Contains");
                    break;
                case IS_TRUE:
                    operation_select->addItem("is");
                    break;
                default:
                    break;
            }
        }
    }
}


void NewRule::on_fileformat_select_currentIndexChanged(const QString &arg1){    
    // Input for value of condition
    inserted_value->show();
    inserted_date->hide();
    inserted_num->hide();
    inserted_bool->hide();

    // Clear operation select box & value box
    operation_select->clear();
    inserted_value->clear();
    inserted_date->clear();
    inserted_num->clear();
    property_select->clear();
    cond_table->clear();

    // Also clear distribution,destination and action with file
    destination->clear();
    dest_folder_distr->clear();
    operation_with_file->setCurrentIndex(0);

    // Choose properities to specific file format
    property_select->addItems(controller->GetProperties(arg1));
}

// delete created condition
void NewRule::CondsClicked(int row,int cell){
    if(cell==3)
        cond_table->removeRow(row);
}

// Select destination folder
void NewRule::on_destination_browse_clicked(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select source folder"),
                                                    "",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(dir != ""){
        destination->setText(dir+"/");
        folderSelected=true;
    }
}

// Save this rule to xml file with saved rules
void NewRule::on_saveButton_clicked()
{
    if(Confirm()){
        QString dir = QFileDialog::getExistingDirectory(this, tr("Select source to save rule"),
                                                        "",
                                                        QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
        if(dir == ""){
            QMessageBox Msgbox;
            Msgbox.setPalette(this->palette());
            Msgbox.setText("Please select destination, where rule will be saved.");
            Msgbox.setWindowTitle("Warning");
            Msgbox.exec();
            return;
        }


        Rule *CreatedRule = CreateRule();
        int retValue = CreatedRule->SaveRule();
        if(retValue != RULE_LOADED){
            QMessageBox Msgbox;
            Msgbox.setPalette(this->palette());
            if(retValue==FILE_ERROR)
                Msgbox.setText("This rule already exists.");
            else{
                Msgbox.setText("Some error occurred. Please try it again with new name.");
            }

            Msgbox.setWindowTitle("Warning");
            Msgbox.exec();
        }

        delete CreatedRule;
        QMessageBox Msgbox;
        Msgbox.setPalette(this->palette());
        Msgbox.setText("Rule was successfully saved.");
        Msgbox.setWindowTitle("Info");
        Msgbox.exec();
    }
}

// Add new rule -> create it in mainwindow
void NewRule::on_addButton_clicked()
{
    if(Confirm()){
        Rule *CreatedRule = CreateRule();
        controller->AddRule(CreatedRule);
        this->close();
    }
}

// Create rule and return reference to it
Rule* NewRule::CreateRule(){
    int type;
    int operand=0;
    QString op;

    Rule *CreatedRule = new Rule();

    // Add created conditions
    for(int row = 0;row<cond_table->rowCount();row++){

        QTableWidgetItem *ui = cond_table->item(row,0);
        QString prop = ui->text();
        type = controller->GetPropertyType(prop,fileformat_select->currentText());

        ui = cond_table->item(row,1);
        op=ui->text();
        if(op == "<")
            operand=LESS;
        else if(op == "<")
            operand=LESS;
        else if(op == "<=")
            operand=LESS_EQUAL;
        else if(op == "=")
            operand=EQUAL;
        else if(op == ">")
            operand=MORE;
        else if(op == ">=")
            operand=MORE_EQUAL;
        else if(op == "Contains")
            operand=CONTAINS;
        else if(op == "Not Contains")
            operand=NOT_CONTAINS;
        else if(op == "is")
            operand=IS_TRUE;


        auto c = CreatedRule->SetCond(operand,prop,type);

        ui = cond_table->item(row,2);
        switch(type){
            case DATE_TYPE:
                CreatedRule->SetValue(QDate::fromString(ui->text(),"dd.MM.yyyy"),c);
                break;
            case NUM_TYPE:
                CreatedRule->SetValue(ui->text().toInt(),c);
                break;
            case TEXT_TYPE:
                CreatedRule->SetValue(ui->text(),c);
                break;
            case TRUE_TYPE:
                bool value;
                if(ui->text() == "TRUE")
                    value=true;
                else
                    value = false;
                CreatedRule->SetValue(value,c);
                        break;
            default:
                break;
        }

    }

    CreatedRule->SetFileFormat(rule_name->text(),fileformat_select->currentText());
    CreatedRule->SetOperation( operation_with_file->currentIndex());
    if(operation_with_file->currentText()!="Delete"){
        CreatedRule->SetDestination(destination->text(),dest_folder_distr->text());
    }
    return CreatedRule;
}

// Cancel adding new rule
void NewRule::on_cancelButton_clicked(){
    this->close();
}

// Add new condition
void NewRule::on_add_one_clicked()
{
    // Check if is everything nessecary is filled
    if(operation_select->currentIndex()!=0
            && property_select->currentIndex()!=0
            && fileformat_select->currentIndex()!=0)
    {

        int type = controller->GetPropertyType(property_select->currentText(),fileformat_select->currentText());
        QString out;
        QDate date_of;
        switch(type){
            case DATE_TYPE:
                date_of = inserted_date->date();
                out = date_of.toString("dd.MM.yyyy");
                break;
            case NUM_TYPE:
                out = QString::number(inserted_num->value());
                break;
            case TEXT_TYPE:
                out = inserted_value->text();
                break;
            case TRUE_TYPE:
                out = inserted_bool->currentText();
                break;
            default:
                break;
        }

        // Insert new values into table
        if(!out.isEmpty()){
            int row = cond_table->rowCount();
            QTableWidgetItem *qi = new QTableWidgetItem;
            cond_table->insertRow(row);

            qi->setText(property_select->currentText());
            cond_table->setItem(row,0,qi);

            qi = new QTableWidgetItem;
            qi->setText(operation_select->currentText());
            cond_table->setItem(row,1,qi);

            qi = new QTableWidgetItem;
            qi->setText(out);
            cond_table->setItem(row,2,qi);

            qi = new QTableWidgetItem;
            qi->setText("X");
            cond_table->setItem(row,3,qi);

            // Input for value of condition
            inserted_value->show();
            inserted_date->hide();
            inserted_num->hide();
            inserted_bool->hide();

            // Clear operation select box & value box
            operation_select->clear();
            inserted_value->clear();
            inserted_date->clear();
            inserted_num->clear();
            property_select->setCurrentIndex(0);
        }
    }
}

// Click on line edit with path to destination folder
bool NewRule::eventFilter(QObject *, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress){
        on_destination_browse_clicked();
    }
    return false;
}

// Check if everything necessary is filled
bool NewRule::Confirm(){
    bool retValue = true;


    if(rule_name->text() == ""){
        retValue = false;
    }
    if(destination->text() == "" && operation_with_file->currentText() != "Delete"){
        retValue = false;
    }
    if(fileformat_select->currentIndex()==0){
        retValue = false;
    }
    if(operation_with_file->currentIndex()==0){
        retValue = false;
    }

    if(!retValue){
        QMessageBox Msgbox;
        Msgbox.setPalette(this->palette());
        Msgbox.setText("Please fill all required fields");
        Msgbox.setWindowTitle("Warning");
        Msgbox.exec();
    }

    return retValue;
}


// Enable edit destination, if delete operation is selected
void NewRule::on_operation_with_file_currentIndexChanged(const QString &arg1)
{
    if(arg1=="Delete"){
        tabWidget->setTabEnabled(2,false);
    }
    else{
        tabWidget->setTabEnabled(2,true);
    }
}

// Create distribution path for files based on their properities
void NewRule::on_distribution_button_clicked()
{
    QString res = QString();
    QString *result = &res;
    newdistribution *distr_dialog;

    distr_dialog = new newdistribution(this);
    distr_dialog->SetResultString(result);
    distr_dialog->Initialize();
    distr_dialog->LoadProperties(fileformat_select->currentText());
    distr_dialog->exec();

    dest_folder_distr->setText(*result);
}
