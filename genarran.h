#ifndef GENARRAN_H
#define GENARRAN_H
#include <QThread>
#include <QSqlQuery>
#include <QDate>
#include <QDateTime>
#include <QVariant>
#include <QDebug>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
class GenArran:public QThread
{
    Q_OBJECT
public:
    GenArran(int weekno,QWidget* formptr,QDate _startdate=QDate::currentDate(),int weekday=7);
    void run();
    int week;
    int weekday;
    QDate startdate;
    QWidget *form;
    void closeThread(){
        isStop = true;
    }
private:
    volatile bool isStop;
};
class DropArran:public QThread
{
    Q_OBJECT
public:
    DropArran(QWidget* formptr);
    void run();
    QWidget *form;
    void closeThread(){
        isStop = true;
    }
private:
    volatile bool isStop;
};
#endif // GENARRAN_H
