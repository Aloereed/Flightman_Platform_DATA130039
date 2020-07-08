#include "addfliarrange.h"
#include "ui_addfliarrange.h"
extern QSqlDatabase db;
extern mainplatformwindow *w;

addfliarrange::addfliarrange(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addfliarrange)
{
    ui->setupUi(this);
    now = QDate::currentDate();
    ui->year->setText(QString::number(now.year(),10));
    ui->month->setText(QString::number(now.month(),10));
    ui->day->setText(QString::number(now.day(),10));
    ui->discount->setText(QString(tr("1.0")));
    ui->plan->setChecked(true);
}

addfliarrange::~addfliarrange()
{
    delete ui;
}

void addfliarrange::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->button(QDialogButtonBox::Ok) == button)
    {
        my_fliarrange tran;
        tran.year = ui->year->text();
        tran.month = ui->month->text();
        tran.day = ui->day->text();
        tran.flight_id = ui->flight_id->text();
        if(ui->plan->isChecked()){
            tran.status = QString("PLA");
        }
        else{
            tran.status = QString("CAN");
        }
        tran.discount = ui->discount->text();
        QString departure_time = tran.year+QString("-")+tran.month+QString("-")+tran.day;
        QDate departure = QDate::fromString(departure_time,"yyyy-M-d");


        QProgressDialog dialog(tr("Adding"),tr("cancel"), 0, 30000, this);
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
#ifdef Q_OS_ANDROID
        QSqlQuery transaction;
        if(transaction.exec("start transaction")){
#else
        if(QSqlDatabase::database().transaction()){
#endif
            QSqlQuery query;
            int week = departure.dayOfWeek();
            QString sqlcheck;
            sqlcheck = QString("SELECT * FROM `flight` WHERE `flight_id` ='%1' AND "
                               "`schedule` LIKE '%%2%'").arg(tran.flight_id).arg(week);
            query.exec(sqlcheck);

            if(now.daysTo(departure)<0||!query.next()){
                if(!QSqlDatabase::database().rollback()){
                    qDebug()<<QSqlDatabase::database().lastError();
                }
                QMessageBox::warning(this,tr("Failure"),tr("error:%1").arg(QSqlDatabase::database().lastError().text()));
                return;
            }

            QString sql1;
            sql1=QString("INSERT INTO `flight_arrangement`(departure_date,flight_id,status,discount)"
                        "VALUES('%1','%2','%3',%4)").arg(departure_time).arg(tran.flight_id).arg(tran.status).arg(tran.discount);
            query.exec(sql1);

            QString sqlselect;
            sqlselect=QString("SELECT MAX(`order`),`type`,`business_No.`,`economy_No.` FROM flight_airline_seat WHERE "
                             "flight_id ='%1'").arg(tran.flight_id);
            query.exec(sqlselect);
            query.next();
            int order = query.value(0).toInt();
            int type = query.value(1).toInt();
            int business = query.value(2).toInt();
            int economy = query.value(3).toInt();

            QString sql2;
            for(int i = 0; i<=order;i++){
                sql2 = QString("INSERT INTO `seat_amount`(flight_id,`order`,departure_date,`type`,`seats_left`)"
                               "VALUES('%1',%2,'%3',%4,%5)").arg(tran.flight_id).arg(i).arg(departure_time).arg(0).arg(business);
                query.exec(sql2);
                sql2 = QString("INSERT INTO `seat_amount`(flight_id,`order`,departure_date,`type`,`seats_left`)"
                               "VALUES('%1',%2,'%3',%4,%5)").arg(tran.flight_id).arg(i).arg(departure_time).arg(1).arg(economy);
                query.exec(sql2);

            }

            QString sql3;
            if(type==1){
                int j=0;
                for(;j<business;j++){
                    QString row = QString::number(j+1,10);
                    QStringList bus={"A","C","D","G","H","K"};
                    for(int k=0;k<6;k++){
                        for(int i = 0; i<=order;i++){
                            sql3=QString("INSERT INTO `seat_arrangement` (`flight_id`,`order`,`departure_date`,`seat_id`)"
                                            "VALUES('%1',%2,'%3','%4')").arg(tran.flight_id).arg(i).arg(departure_time).arg(row+bus[k]);
                            query.exec(sql3);
                        }
                    }
                }
                for(j=0;j<economy;j++){
                    QString row = QString::number(j+1+business,10);
                    QStringList eco={"A","B","C","D","E","G","H","J","K"};
                    for(int k=0;k<9;k++){
                        for(int i = 0; i<=order;i++){
                            sql3=QString("INSERT INTO `seat_arrangement` (`flight_id`,`order`,`departure_date`,`seat_id`)"
                                            "VALUES('%1',%2,'%3','%4')").arg(tran.flight_id).arg(i).arg(departure_time).arg(row+eco[k]);
                            query.exec(sql3);
                        }
                    }
                }
            }
            else if(type==0){
                int j=0;
                for(;j<business;j++){
                    QString row = QString::number(j+1,10);
                    QStringList bus={"A","C","J","L"};
                    for(int k=0;k<6;k++){
                        for(int i = 0; i<=order;i++){
                            sql3=QString("INSERT INTO `seat_arrangement` (`flight_id`,`order`,`departure_date`,`seat_id`)"
                                            "VALUES('%1',%2,'%3','%4')").arg(tran.flight_id).arg(i).arg(departure_time).arg(row+bus[k]);
                            query.exec(sql3);
                        }
                    }
                }
                for(j=0;j<economy;j++){
                    QString row = QString::number(j+1+business,10);
                    QStringList eco={"A","B","C","J","K","L"};
                    for(int k=0;k<9;k++){
                        for(int i = 0; i<=order;i++){
                            sql3=QString("INSERT INTO `seat_arrangement` (`flight_id`,`order`,`departure_date`,`seat_id`)"
                                            "VALUES('%1',%2,'%3','%4')").arg(tran.flight_id).arg(i).arg(departure_time).arg(row+eco[k]);
                            query.exec(sql3);
                        }
                    }
                }
            }
#ifdef Q_OS_ANDROID
            if(!transaction.exec("commit")){
#else
            if(!QSqlDatabase::database().commit()){
#endif
                qDebug()<<QSqlDatabase::database().lastError();
#ifdef Q_OS_ANDROID
                if(!transaction.exec("rollback")){
#else
                if(!QSqlDatabase::database().rollback()){
#endif
                    QMessageBox::warning(this,tr("Failure"),tr("error:%1").arg(QSqlDatabase::database().lastError().text()));
                }
             }
            else{
                QMessageBox::information(this,tr("hint:"),tr("add successfully"));
                w->fliarrangeRefresh();
                this->close();
            }
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
