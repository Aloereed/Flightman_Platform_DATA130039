#include "sqlquery.h"
#include "ui_sqlquery.h"

SQLQuery::SQLQuery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SQLQuery)
{
    ui->setupUi(this);
}

SQLQuery::~SQLQuery()
{
    delete ui;
}
