#include "queryplugin.h"
#include "ui_queryplugin.h"

queryplugin::queryplugin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::queryplugin)
{
    ui->setupUi(this);
    ui->plainTextEdit->setMaximumSize(QSize(16777215,ui->comboBox->height()));
}

queryplugin::~queryplugin()
{
    delete ui;
}
