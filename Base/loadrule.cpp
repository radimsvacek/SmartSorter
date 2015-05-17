/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#include "loadrule.h"
#include "ui_loadrule.h"

LoadRuleWindow::LoadRuleWindow(QWidget * parent) : QDialog(parent)
{
    setupUi(this);
    setWindowTitle("Load rule");
    QPalette p(this->palette());
    p.setColor(QPalette::Window, QColor::fromRgb(223,226,219));
    p.setColor(QPalette::Text, QColor::fromRgb(25,25,25));
    p.setColor(QPalette::Button, QColor::fromRgb(25,25,25));
    p.setColor(QPalette::ButtonText, QColor::fromRgb(255,240,86));
    p.setColor(QPalette::WindowText, QColor::fromRgb(25,25,25));
    this->setPalette(p);

    selectedFile->installEventFilter(this);

}

void LoadRuleWindow::Initialize(){
    C=Control::GetInstance();
    fileSelected=false;
}

void LoadRuleWindow::on_buttonBox_accepted()
{
    if(fileSelected){
        Rule * rule = new Rule();
        int retValue = rule->LoadRule(contentOfXML);
        if(retValue==RULE_LOADED){
            C->AddRule(rule);
            this->close();
        }
        else{
            delete rule;
            QMessageBox Msgbox;
            Msgbox.setPalette(this->palette());
            if(retValue==FILE_ERROR)
                Msgbox.setText("File cannot be opened");
            else if(retValue == XML_ERROR)
                Msgbox.setText("XML file does not match proper XML file");

            Msgbox.setWindowTitle("Warning");
            Msgbox.exec();
        }


    }
}

void LoadRuleWindow::on_buttonBox_rejected()
{
    this->close();
}


void LoadRuleWindow::on_pushButton_clicked()
{
    contentOfXML = QFileDialog::getOpenFileName(this, tr("Select XML file with rule"),"", tr("XML files (*.xml)"));
    if(contentOfXML != ""){
        selectedFile->setText(contentOfXML);
        fileSelected=true;
    }
}

/// Click on line edit with path to destination folder
bool LoadRuleWindow::eventFilter(QObject *, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress){
        on_pushButton_clicked();
    }
    return false;
}
