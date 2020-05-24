#include "addflight_passingairport.h"
#include "ui_addflight_passingairport.h"

addflight_passingairport::addflight_passingairport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addflight_passingairport)
{
    ui->setupUi(this);
}

addflight_passingairport::~addflight_passingairport()
{
    delete ui;
}

void addflight_passingairport::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->button(QDialogButtonBox::Ok) == button)
    {
        my_pass tran;

        tran.airport_id = ui->ID->text();
        tran.arrival_time = ui->radioButton->isChecked()?"":ui->timeEdit->time().toString();
        tran.departure_time = ui->radioButton_3->isChecked()?"":ui->timeEdit_2->time().toString();

        emit sendPass(tran);
        this->close();

    }
    else if(ui->buttonBox->button(QDialogButtonBox::Cancel) == button)
        this->close();
}

void addflight_passingairport::on_radioButton_clicked()
{
    ui->timeEdit->setEnabled(false);
    ui->timeEdit_2->setEnabled(true);
}

void addflight_passingairport::on_radioButton_3_clicked()
{
    ui->timeEdit_2->setEnabled(false);
    ui->timeEdit->setEnabled(true);
}

void addflight_passingairport::on_radioButton_2_clicked()
{
    ui->timeEdit_2->setEnabled(true);
    ui->timeEdit->setEnabled(true);
}
