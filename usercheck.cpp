#include "usercheck.h"
#include "ui_usercheck.h"

usercheck::usercheck(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::usercheck)
{
    ui->setupUi(this);
}

usercheck::~usercheck()
{
    delete ui;
}
