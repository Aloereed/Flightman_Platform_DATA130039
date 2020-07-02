#include "genarran.h"
#include <QProgressDialog>
#include <QApplication>
#include <QSqlDriver>
extern QSqlDatabase db;
GenArran::GenArran(int weekno, QWidget *formptr)
{
    week=weekno;
    form=formptr;
}

void GenArran::run(){
    bool ok;
    if(week<=0){
        QMessageBox::information(form,tr("hint:"),tr("invalid week number"));
    }
    else{
        QDate now = QDate::currentDate();
        int nowweek = now.dayOfWeek();
        while( nowweek !=1){
            now = now.addDays(1);
            nowweek = now.dayOfWeek();
        }
        int days = 7*week;
        QProgressDialog progress;
        progress.setWindowModality(Qt::WindowModal);
        progress.setWindowTitle(QObject::tr("Creating Realtime Flight Info..."));
        progress.setLabelText(QObject::tr("Preparing database..."));
        progress.setMinimum(0);
        //progress.setMaximum(days+1);
        qApp->processEvents();

        if(QSqlDatabase::database().transaction()){
            QSqlQuery query1;
            QSqlQuery query;
            for(int i=1;i<=days;i++,now=now.addDays(1)){
                QApplication::processEvents();
                QString day = QString::number(i%7,10);
                QString sql1 = QString("SELECT * FROM seat_oneclick_view WHERE schedule like '%%1%'").arg(day);
                query1.exec(sql1);
                progress.setMaximum(query1.size()+1);
                int recount=0;
                while(query1.next()){
                    QApplication::processEvents();
                    recount++;
                    progress.setValue(recount);
                    QString flight_id = query1.value(0).toString();
                    //QString airport_id = query1.value(1).toString();
                    int order=query1.value(5).toInt();
                    int type = query1.value(2).toInt();
                    int row_bus = query1.value(3).toInt();
                    int row_eco = query1.value(4).toInt();
                    QString departure_time = query1.value(2).toString();
                    QString sql2,sql3,sql4,sql5;
                    //QString name = QString("seat_")+flight_id+QString("_")+airport_id+QString("_")+now.toString("yyyy-MM-dd");
                    departure_time = now.toString("yyyy-MM-dd");//+QString(" ")+departure_time;

                    QString status = QString("PLA");
                    int discount = 1;

                    sql2 = QString("INSERT INTO flight_arrangement (flight_id,departure_date,`status`,discount)"
                            "VALUES(\'%1\',\'%2\',\'%3\',%4)")
                    .arg(flight_id).arg(departure_time).arg(status).arg(discount);
                    ok=query.exec(sql2);
                    sql3=QString("INSERT INTO seat_amount (flight_id,`order`,departure_date,`type`,seats_left) VALUES('%1',%2,'%3',%4,%5)").arg(flight_id).arg(order).arg(departure_time).arg(0).arg(row_bus*(type?6:4));
                    ok=query.exec(sql3);
                    sql5=QString("INSERT INTO seat_amount (flight_id,`order`,departure_date,`type`,seats_left) VALUES('%1',%2,'%3',%4,%5)").arg(flight_id).arg(order).arg(departure_time).arg(1).arg(row_eco*(type?9:6));
                    ok=query.exec(sql5);


                    if(type==1){
                        int j=0;
                        for(;j<row_bus;j++){
                            QApplication::processEvents();
                            QString row = QString::number(j+1,10);
                            QStringList bus={"A","C","D","G","H","K"};
                            for(int k=0;k<6;k++){
                                   for(int i=0;i!=order+1;++i){
                                    sql4=QString("INSERT INTO `seat_arrangement` (flight_id,`order`,depature_date,seat_id,status)"
                                                    "VALUES('%1',%2,'%3','%4',%5)").arg(flight_id).arg(QString::number(i)).arg(departure_time).arg(row+bus[k]).arg(0);
                                ok=query.exec(sql4);
                            }
                            }
                        }
                        for(j=0;j<row_eco;j++){
                            QApplication::processEvents();
                            QString row = QString::number(j+1+row_bus,10);
                            QStringList eco={"A","B","C","D","E","G","H","J","K"};
                            for(int k=0;k<9;k++){
                                for(int i=0;i!=order+1;++i){
                                sql4=QString("INSERT INTO `seat_arrangement` (flight_id,`order`,depature_date,seat_id,status)"
                                                "VALUES('%1',%2,'%3','%4',%5)").arg(flight_id).arg(QString::number(i)).arg(departure_time).arg(row+eco[k]).arg(0);
                                ok=query.exec(sql4);
                                }
                            }
                        }
                    }
                    else if(type==0){
                        int j=0;
                        for(;j<row_bus;j++){
                            QApplication::processEvents();
                            QString row = QString::number(j+1,10);
                            QStringList bus={"A","C","J","L"};
                            for(int k=0;k<4;k++){
                                for(int i=0;i!=order+1;++i){
                                sql4=QString("INSERT INTO `seat_arrangement` (flight_id,`order`,depature_date,seat_id,status)"
                                                "VALUES('%1',%2,'%3','%4',%5)").arg(flight_id).arg(QString::number(i)).arg(departure_time).arg(row+bus[k]).arg(0);
                                query.exec(sql4);
                                }
                            }
                        }
                        for(j=0;j<row_eco;j++){
                            QApplication::processEvents();
                            QString row = QString::number(j+1+row_bus,10);
                            QStringList eco={"A","B","C","J","K","L"};
                            for(int k=0;k<6&&order!=-1;k++){
                                for(int i=0;i!=order+1;++i){
                                sql4=QString("INSERT INTO `seat_arrangement` (flight_id,`order`,depature_date,seat_id,status)"
                                                "VALUES('%1',%2,'%3','%4',%5)").arg(flight_id).arg(QString::number(i)).arg(departure_time).arg(row+eco[k]).arg(0);
                                query.exec(sql4);
                                }
                            }
                        }
                    }
                }
                //progress.setValue(i);
                progress.setLabelText(QObject::tr("Preparing database...%1/%2").arg(i).arg(days));
                qApp->processEvents();
                if(progress.wasCanceled()){
                    if(!QSqlDatabase::database().rollback()){
                        QMessageBox::warning(form,tr("Failure"),tr("error:%1").arg(QSqlDatabase::database().lastError().text()));
                    }
                  return;
                }
            }
            progress.setLabelText(QObject::tr("Writing database..."));
            progress.setCancelButton(NULL);
            if(!QSqlDatabase::database().commit()){
                qDebug()<<QSqlDatabase::database().lastError();
                if(!QSqlDatabase::database().rollback()){
                    QMessageBox::warning(form,tr("Failure"),tr("error:%1").arg(QSqlDatabase::database().lastError().text()));
                }
             }
            else{
                progress.setValue(days+1);
                qApp->processEvents();
                QMessageBox::information(form,tr("hint:"),tr("success"));
            }
        }


    }
    this->quit();
}
DropArran::DropArran(QWidget *formptr)
{
    form=formptr;
}

void DropArran::run(){
    /*
    QSqlQuery query1;
    if(QSqlDatabase::database().transaction()){
        QString sql1,sql2;
        QSqlQuery query2;
        sql1=QString("Select CONCAT( 'drop table ','`',table_name,'`', ';') FROM information_schema.tables Where table_name LIKE 'seat_%';");
        query1.exec(sql1);
        QProgressDialog progress;
        progress.setWindowModality(Qt::WindowModal);
        progress.setWindowTitle(QObject::tr("Creating Realtime Flight Info..."));
        progress.setLabelText(QObject::tr("Preparing database..."));
        progress.setMinimum(0);
        int size=query1.size();
        progress.setMaximum(size+1);
        qApp->processEvents();
        int i=0;
        while(query1.next()){
            QApplication::processEvents();
            sql1=query1.value(0).toString();
            query2.exec(sql1);
            i++;
            progress.setValue(i);
            progress.setLabelText(QObject::tr("Preparing database...%1/%2").arg(i).arg(size));
            qApp->processEvents();
            if(progress.wasCanceled())
              return;
        }
        sql2=QString("truncate table flight_arrangment");
        query2.exec(sql2);
        progress.setLabelText(QObject::tr("Writing database..."));
        qApp->processEvents();
        progress.setCancelButton(NULL);
        if(!QSqlDatabase::database().commit()){
            qDebug()<<QSqlDatabase::database().lastError();
            if(!QSqlDatabase::database().rollback()){
                QMessageBox::warning(form,tr("Failure"),tr("error:%1").arg(QSqlDatabase::database().lastError().text()));
            }
         }
        else{
            progress.setValue(size+1);
            QMessageBox::information(form,tr("hint:"),tr("success"));
        }
    }
    */
    this->quit();
}
