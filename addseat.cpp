#include "addseat.h"
#include "ui_addseat.h"

addseat::addseat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addseat)
{
    ui->setupUi(this);
}

addseat::addseat(QString flight_id,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addseat)
{
    ui->setupUi(this);
    QSqlQuery query;
    query.exec(tr("SELECT * FROM seat WHERE flight_id = '%1'").arg(flight_id));
    query.next();
    if(query.value(1)==1)
        ui->radioButton->setChecked(1);
    else
        ui->radioButton_2->setChecked(1);
    ui->business->setText(query.value(2).toString());
    ui->economy->setText(query.value(3).toString());

}

addseat::~addseat()
{
    delete ui;
}

void addseat::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->button(QDialogButtonBox::Ok) == button)
    {
        my_seat tran;
        if(ui->radioButton->isChecked())
            tran.type = 1;
        else
            tran.type = 0;
        tran.business = ui->business->text().toInt();
        tran.economy = ui->economy->text().toInt();

        emit sendseat(tran);
        this->close();

    }
    else if(ui->buttonBox->button(QDialogButtonBox::Cancel) == button)
        this->close();
}
