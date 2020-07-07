#include "usercheck.h"
#include "ui_usercheck.h"
extern QSqlDatabase db;
extern mainplatformwindow *w;

usercheck::usercheck(QString flightID_input,QString departure_air_input,QString arrival_air_input,QString class_input,
                     QString departure_date_input,QString ticketID_input,QString departure_datetime_input,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::usercheck)
{
    ui->setupUi(this);
    flightID=flightID_input;
    departure_air=departure_air_input;
    arrival_air=arrival_air_input;
    class_=class_input;
    departure_date=departure_date_input;
    ticketID=ticketID_input;
    departure_datetime=departure_datetime_input;

}

usercheck::~usercheck()
{
    delete ui;
}

void usercheck::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->button(QDialogButtonBox::Ok) == button)
    {
        QByteArray bytePwd = ui->password->text().toLatin1();
        QByteArray bytePwdMd5 = QCryptographicHash::hash(bytePwd, QCryptographicHash::Md5);
        QString strPwdMd5 = bytePwdMd5.toHex();

        user = ui->ID->text();
        QString sql;
        sql = QString("SELECT `password` FROM user WHERE ID = '%1'").arg(user);
        QSqlQuery query;
        query.exec(sql);
        query.next();
        QString psw = query.value(0).toString();
        if(psw == strPwdMd5){
            QProgressDialog dialog(tr("Processing"),tr("cancel"), 0, 30000, this);
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


            sql = QString("call addticket('%1','%2','%3',%4,'%5','%6','%7','%8')").arg(flightID).arg(departure_air).arg(arrival_air)
                    .arg(class_).arg(user).arg(departure_date).arg(ticketID).arg(departure_datetime);
            qDebug()<<sql;
            query.exec(sql);

            bool ok = query.exec(sql);
            if(ok){
                QMessageBox::information(this,tr("hint:"),tr("purchase successfully"));
                w->ticketRefresh();
                this->close();
            }
            else{
                QMessageBox::information(this,tr("hint:"),tr("failure"));
                this->close();
            }

        }
        else{
            QMessageBox::information(this,tr("hint:"),tr("Wrong UserID or Password"));

        }
    }
    else if(ui->buttonBox->button(QDialogButtonBox::Cancel) == button)
    {
        this->close();
        QProgressDialog dialog(tr("Returning to the former window"),tr("cancel"), 0, 3000, this);
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
       }

}
