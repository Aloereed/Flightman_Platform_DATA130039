#include "queryplugin.h"
#include "ui_queryplugin.h"

queryplugin::queryplugin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::queryplugin)
{
    ui->setupUi(this);
}

queryplugin::~queryplugin()
{
    delete ui;
}
