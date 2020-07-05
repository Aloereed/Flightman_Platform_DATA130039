#include "genarran.h"
#include <QProgressDialog>
#include <QApplication>
#include <QSqlDriver>
extern QSqlDatabase db;
GenArran::GenArran(int weekno, QWidget *formptr,QDate _startdate,int _weekday)
{
    week=weekno;
    form=formptr;
    startdate=_startdate;
    weekday=_weekday;
    isStop=false;
}

void GenArran::run(){
    bool ok=true;
    QSqlQuery starttran;
    if(week<=0){
        QMessageBox::information(form,tr("hint:"),tr("invalid week number"));
    }
    else{
        QDate now = startdate;
        int days = weekday*week;
        QProgressDialog progress;
        progress.setWindowModality(Qt::WindowModal);
        progress.setWindowTitle(QObject::tr("Creating Realtime Flight Info..."));
        progress.setLabelText(QObject::tr("Preparing database..."));
        progress.setMinimum(0);
        //progress.setMaximum(days+1);
        qApp->processEvents();

        starttran.exec("SET AUTOCOMMIT=0");
        if(QSqlDatabase::database().transaction()){
            QSqlQuery query1;
            QSqlQuery query;
            for(int i=1;i<=days&&(!isStop);i++,now=now.addDays(1)){
                QApplication::processEvents();
                QString day = QString::number(i%7,10);
                QString sql1 = QString("SELECT * FROM seat_oneclick_view WHERE schedule like '%%1%'").arg(day);
                ok&=query1.exec(sql1);
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
                    ok&=query.exec(sql2);
                    for(int i=0;i!=order+1;++i){
                        sql3=QString("INSERT INTO seat_amount (flight_id,`order`,departure_date,`type`,seats_left) VALUES('%1',%2,'%3',%4,%5)").arg(flight_id).arg(i).arg(departure_time).arg(0).arg(row_bus*(type?6:4));
                        ok&=query.exec(sql3);
                    }
                    for(int i=0;i!=order+1;++i){
                        sql5=QString("INSERT INTO seat_amount (flight_id,`order`,departure_date,`type`,seats_left) VALUES('%1',%2,'%3',%4,%5)").arg(flight_id).arg(i).arg(departure_time).arg(1).arg(row_eco*(type?9:6));
                        ok&=query.exec(sql5);
                    }

                    //db.transaction();
                    if(type==1){            
                        int j=0;                      
                        QVariantList seats;
                        for(;j<row_bus;j++){
                            QApplication::processEvents();
                            QString row = QString::number(j+1,10);
                            QStringList bus={"A","C","D","G","H","K"};
                            for(int k=0;k<6;k++){
                                seats<<row+bus[k];
                            }
                        }
                        for(j=0;j<row_eco;j++){

                            QApplication::processEvents();
                            QString row = QString::number(j+1+row_bus,10);
                            QStringList eco={"A","B","C","D","E","G","H","J","K"};
                            for(int k=0;k<9;k++){
                                seats<<row+eco[k];
                            }
                        }
                            for(int i=0;i!=order+1;++i){
                                sql4=QString("INSERT INTO `seat_arrangement` (flight_id,`order`,departure_date,seat_id) VALUES ");
                                for(int j=0;j!=seats.size();++j)
                                    sql4+=QString("('%1',%2,'%3',\'").arg(flight_id).arg(QString::number(i)).arg(departure_time)+seats[j].toString()+"\'),";
                                sql4=sql4.left(sql4.size()-1);

                                ok&=query.exec(sql4);
                                //qDebug()<<query.lastError();
                                //query.addBindValue(seats);

                            }

                        seats.clear();

                    }

                    else if(type==0){
                        int j=0;
                        QVariantList seats;
                        for(;j<row_bus;j++){
                            QApplication::processEvents();
                            QString row = QString::number(j+1,10);
                            QStringList bus={"A","C","J","L"};
                            for(int k=0;k<4;k++){
                                seats<<row+bus[k];
                            }
                        }
                        for(j=0;j<row_eco;j++){
                            QApplication::processEvents();
                            QString row = QString::number(j+1+row_bus,10);
                            QStringList eco={"A","B","C","J","K","L"};
                            for(int k=0;k<6;k++){
                                seats<<row+eco[k];
                            }
                        }

                            for(int i=0;i!=order+1;++i){
                                sql4=QString("INSERT INTO `seat_arrangement` (flight_id,`order`,departure_date,seat_id) VALUES ");
                                for(int j=0;j!=seats.size();++j)
                                    sql4+=QString("('%1',%2,'%3',\'").arg(flight_id).arg(QString::number(i)).arg(departure_time)+seats[j].toString()+"\'),";
                                sql4=sql4.left(sql4.size()-1);
                                query.prepare(sql4);
                                //query.addBindValue(seats);
                                ok&=query.exec();

                            }

                        seats.clear();
                        if(!ok){
                            QMessageBox::warning(form,tr("Failure to commit"),tr("Query went wrong:%1").arg(query.lastError().text()+query1.lastError().text()));
                            progress.cancel();
                        }
                    }
                    if(progress.wasCanceled()){
                        if(!QSqlDatabase::database().rollback()){
                            QMessageBox::warning(form,tr("Failure"),tr("error:%1").arg(QSqlDatabase::database().lastError().text()));
                        }
                      isStop=true;
                      starttran.exec("SET AUTOCOMMIT=1");
                      return;
                    }
                }
                //progress.setValue(i);
                progress.setLabelText(QObject::tr("Preparing database...%1/%2").arg(i).arg(days));
                qApp->processEvents();
            }
            progress.setLabelText(QObject::tr("Writing database..."));
            progress.setCancelButton(NULL);
            if(!ok||!QSqlDatabase::database().commit()){
                qDebug()<<QSqlDatabase::database().lastError();
                QMessageBox::warning(form,tr("Failure to commit"),tr("Query went wrong or database met error:%1").arg(QSqlDatabase::database().lastError().text()));
                if(!QSqlDatabase::database().rollback()){
                    QMessageBox::warning(form,tr("Failure to rollback"),tr("error:%1").arg(QSqlDatabase::database().lastError().text()));
                }
                progress.close();
             }
            else{
                progress.close();
                qApp->processEvents();
                QMessageBox::information(form,tr("hint:"),tr("success"));
            }
        }


    }
    starttran.exec("SET AUTOCOMMIT=1");
    this->quit();
    closeThread();
    return;
}
DropArran::DropArran(QWidget *formptr)
{
    form=formptr;
    isStop=false;
}

void DropArran::run(){

    QSqlQuery query1;
    QSqlQuery starttran;
    starttran.exec("SET AUTOCOMMIT=0");
    if(QSqlDatabase::database().transaction()){
        QString sql1,sql2;
        QSqlQuery query2;
;
        QProgressDialog progress;
        progress.setWindowModality(Qt::WindowModal);
        progress.setWindowTitle(QObject::tr("Deleting Realtime Flight Info..."));
        progress.setLabelText(QObject::tr("Preparing database..."));
        progress.setMinimum(0);
        progress.setMaximum(0);
        sql1=QString("truncate table seat_arrangment");
        query1.exec(sql1);
        sql2=QString("truncate table seat_amount");
        query2.exec(sql2);
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
            progress.close();
         }
        else{
            progress.close();
            QMessageBox::information(form,tr("hint:"),tr("success"));
        }
    }
    starttran.exec("SET AUTOCOMMIT=1");
    this->closeThread();
    this->quit();
    return;
}
