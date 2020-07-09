#include "modfliarrange.h"
#include "ui_modfliarrange.h"
extern QSqlDatabase db;
extern mainplatformwindow *w;

modfliarrange::modfliarrange(QString departure_date_input,QString flight_id_input,QString status_input,
                             QString discount_input,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::modfliarrange)
{
    ui->setupUi(this);
    QStringList list = departure_date_input.split("-");
    year = list[0];
    month = list[1];
    day = list[2];
    flight_id = flight_id_input;
    status = status_input;
    discount = discount_input;
    ui->year->setText(year);
    ui->month->setText(month);
    ui->day->setText(day);
    ui->flight_id->setText(flight_id);
    ui->flight_id->setReadOnly(true);
    if(status=="PLA")
        ui->plan->setChecked(true);
    else
        ui->cancel->setChecked(true);
    ui->discount->setText(discount);
}

modfliarrange::~modfliarrange()
{
    delete ui;
}

void modfliarrange::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->button(QDialogButtonBox::Ok) == button)
    {
        QString newyear = ui->year->text();
        QString newmonth = ui->month->text();
        QString newday = ui->day->text();

        if(ui->plan->isChecked()){
            status = QString("PLA");
        }
        else{
            status = QString("CAN");
        }
        discount = ui->discount->text();
        QString newdeparture_time = newyear+QString("-")+newmonth+QString("-")+newday;
        QString olddeparture_time = year+QString("-")+month+QString("-")+day;
        QDate newdeparture = QDate::fromString(newdeparture_time,"yyyy-M-d");
        QDate olddeparture = QDate::fromString(olddeparture_time,"yyyy-M-d");
        QDate limit_down = QDate::currentDate().addDays(3);
        QDate limit_up = olddeparture.addDays(7);



        QProgressDialog dialog(tr("Modifying"),tr("cancel"), 0, 30000, this);
        dialog.setWindowTitle(tr("process"));
        dialog.setWindowModality(Qt::WindowModal);
        dialog.show();
        for(int k = 0; k < 30000; k++)
        {
            dialog.setValue(k);
            QCoreApplication::processEvents();
            if(dialog.wasCanceled())
            {
                break;
            }
        }
        dialog.setValue(30000);

        QString sql1;
        QSqlQuery query;
        bool ok;




        if(limit_down.daysTo(newdeparture)<0||newdeparture.daysTo(limit_up)<0){
            QMessageBox::warning(this,tr("Failure"),tr("error:%1").arg("the new departure date couldn't be such early or late"));
            return;
        }

        if(status=="CAN"){
            QString sql1 = QString("CALL cancel_fliarrangement('%1','%2')")
                    .arg(flight_id).arg(olddeparture_time).arg(newdeparture_time).arg(status).arg(discount);;
            ok = query.exec(sql1);
            if(ok) {
                QMessageBox::information(this,tr("hint:"),tr("Cancel successfully"));
                w->fliarrangeRefresh();
                this->close();

            } else {
                QMessageBox::critical(this, tr("warning:"), tr("Cancel failed."));
            }
            return;
        }

        if(newdeparture.daysTo(olddeparture)==0){
            sql1=QString("UPDATE flight_arrangement "
                         "SET `status`='%1',`discount`= %2"
                         "WHERE flight_id = '%3' AND departure_date = '%4' ")
                        .arg(status).arg(discount).arg(flight_id).arg(olddeparture_time);
            ok = query.exec(sql1);
            if(ok){
                QMessageBox::information(this,tr("hint:"),tr("Modify successfully"));
                w->fliarrangeRefresh();
                this->close();
            }
            else{
                QMessageBox::warning(this,tr("error:"),tr("Modify Failure"));

            }

            return;
        }

\
        sql1=QString("CALL modify_fliarrangement('%1','%2','%3','%4',%5)")
                    .arg(flight_id).arg(olddeparture_time).arg(newdeparture_time).arg(status).arg(discount);
        ok = query.exec(sql1);

        if(ok){
            QMessageBox::information(this,tr("hint:"),tr("Modify successfully"));
            w->fliarrangeRefresh();
            this->close();
        }
        else{
            QMessageBox::warning(this,tr("error:"),tr("Modify Failure"));

        }

    }
    else if(ui->buttonBox->button(QDialogButtonBox::Cancel) == button)
    {

        QProgressDialog dialog(tr("Returning to the mainwindow"),tr("cancel"), 0, 3000, this);
        dialog.setWindowTitle(tr("process"));
        dialog.setWindowModality(Qt::WindowModal);
        dialog.show();
        for(int k = 0; k < 3000; k++)
        {
            dialog.setValue(k);
            QCoreApplication::processEvents();
            if(dialog.wasCanceled())
            {
                break;
            }
        }
        dialog.setValue(3000);
        this->close();
       }
}
