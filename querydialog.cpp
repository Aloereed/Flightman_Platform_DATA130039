#include "querydialog.h"
#include "ui_querydialog.h"
#include "queryplugin.h"
QueryDialog::QueryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QueryDialog)
{
    ui->setupUi(this);
}

QueryDialog::~QueryDialog()
{
    delete ui;
}

void QueryDialog::on_pushButton_clicked()
{
    queryplugin* ptr = new queryplugin;
    ui->verticalLayout_2->addWidget(ptr);
}
