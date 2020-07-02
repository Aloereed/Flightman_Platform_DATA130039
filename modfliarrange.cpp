#include "modfliarrange.h"
#include "ui_modfliarrange.h"

modfliarrange::modfliarrange(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::modfliarrange)
{
    ui->setupUi(this);
}

modfliarrange::~modfliarrange()
{
    delete ui;
}
