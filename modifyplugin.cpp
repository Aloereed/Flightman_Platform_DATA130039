#include "modifyplugin.h"
#include "ui_modifyplugin.h"

modifyplugin::modifyplugin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::modifyplugin)
{
    ui->setupUi(this);
}
modifyplugin::modifyplugin(QStringList indexlist,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::modifyplugin)
{
    ui->setupUi(this);
    QStringList list =indexlist;
    ui->comboBox_2->addItems(list);
    ui->plainTextEdit->setMaximumSize(QSize(16777215,ui->comboBox_2->height()));

}
modifyplugin::~modifyplugin()
{
    delete ui;
}
int modifyplugin::index(){
    return ui->comboBox_2->currentIndex();
}


QString modifyplugin::text(){
    return ui->plainTextEdit->toPlainText();
}
