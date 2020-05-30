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
    GenArran(int weekno,QWidget* formptr);
    void run();
    int week;
    QWidget *form;
};
class DropArran:public QThread
{
    Q_OBJECT
public:
    DropArran(QWidget* formptr);
    void run();
    QWidget *form;
};
#endif // GENARRAN_H
