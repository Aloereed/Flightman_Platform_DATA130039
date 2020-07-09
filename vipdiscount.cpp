#include "vipdiscount.h"
#include "ui_vipdiscount.h"
#include "QSqlQuery"
#include <QMessageBox>
VIPDiscount::VIPDiscount(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VIPDiscount)
{
    ui->setupUi(this);
    QSqlQuery query;
    query.exec("select * from membership_discount");
    if(query.next()){
        ui->doubleSpinBox->setValue(query.value(1).toDouble());
        if(query.next()){
            ui->doubleSpinBox_2->setValue(query.value(1).toDouble());
        }
    }


}

VIPDiscount::~VIPDiscount()
{
    delete ui;
}

void VIPDiscount::on_buttonBox_accepted()
{
    QSqlQuery query;
    bool ok=true;
    ok&=query.exec("update membership_discount set `discount`="+QString::number(ui->doubleSpinBox->value())+" where `class`=0");
    ok&=query.exec("update membership_discount set `discount`="+QString::number(ui->doubleSpinBox_2->value())+" where `class`=1");
    if(ok)
        QMessageBox::information(NULL,tr("Success!"),tr("Success to change discount information!"));
    else
        QMessageBox::information(NULL,tr("Failure!"),tr("Failure to change discount information!"));
    this->close();
}
