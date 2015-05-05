#include "newdistribution.h"


newdistribution::newdistribution(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setWindowTitle("File distribution");

    connect(aviableAttributes,SIGNAL(cellClicked(int,int)),this,SLOT(AttributeClicked(int,int)));

    aviableAttributes->setHorizontalHeaderLabels(QStringList("Possible properties"));
    aviableAttributes->horizontalHeader()->setStretchLastSection(true);

    QPalette p(this->palette());
    p.setColor(QPalette::Window, QColor::fromRgb(223,226,219));
    p.setColor(QPalette::Text, QColor::fromRgb(25,25,25));
    p.setColor(QPalette::Button, QColor::fromRgb(25,25,25));
    p.setColor(QPalette::ButtonText, QColor::fromRgb(255,240,86));
    p.setColor(QPalette::WindowText, QColor::fromRgb(25,25,25));
    this->setPalette(p);
}


void newdistribution::on_clearButton_clicked()
{
    distribution->clear();
    LoadProperties(fileFormat);
}

void newdistribution::SetResultString(QString *result){
    resultStr=result;
}

void newdistribution::LoadProperties(QString format){
    fileFormat = format;
    QStringList properites = controller->GetPropertiesForDistribution(format);
    int row;
    QTableWidgetItem * qi;

    for (QStringList::iterator it = properites.begin(); it != properites.end(); ++it) {
           row = aviableAttributes->rowCount();
           aviableAttributes->insertRow(row);
           QString current = *it;
           qi = new QTableWidgetItem;
           qi->setText(current);
           qi->setTextAlignment(Qt::AlignCenter);
           aviableAttributes->setItem(row,0,qi);
    }
}

void newdistribution::Initialize(){
    controller=Control::GetInstance();
}

void newdistribution::on_buttonBox_accepted()
{
    if(distribution->text() != ""){
        *resultStr = distribution->text();
    }

    this->close();
}

// Cancel
void newdistribution::on_buttonBox_rejected()
{
    this->close();
}


// Make some changes on table because of new order of rules or delete rule
void newdistribution::AttributeClicked(int row, int col){
    QTableWidgetItem *qi =  aviableAttributes->item(row,col);
    QString property = qi->text();
    if(distribution->text() != "")
        property = distribution->text()+"/"+property;
    else
        property= property;
    aviableAttributes->removeRow(row);
    distribution->setText(property);
}
