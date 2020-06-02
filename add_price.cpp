#include "add_price.h"
#include "ui_add_price.h"

add_price::add_price(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_price)
{
    ui->setupUi(this);
}

add_price::~add_price()
{
    delete ui;
}
