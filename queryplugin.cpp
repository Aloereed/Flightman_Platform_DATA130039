#include "queryplugin.h"
#include "ui_queryplugin.h"

queryplugin::queryplugin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::queryplugin)
{
    ui->setupUi(this);

}

queryplugin::queryplugin(QStringList indexlist,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::queryplugin)
{
    ui->setupUi(this);
    QStringList list =indexlist;
    ui->comboBox_2->addItems(list);
}

queryplugin::~queryplugin()
{
    delete ui;
}

QString queryplugin::sign_1(){
    return ui->comboBox->currentText();
}

int queryplugin::index(){
    return ui->comboBox_2->currentIndex();
}

QString queryplugin::sign_2(){
    return ui->comboBox_3->currentText();
}

QString queryplugin::text(){
    return ui->plainTextEdit->toPlainText();
}
