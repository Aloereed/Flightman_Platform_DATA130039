#ifndef USERCHECK_H
#define USERCHECK_H

#include <QWidget>
#include <QAbstractButton>
#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSqlQuery>
#include <QCryptographicHash>
#include "mainplatformwindow.h"

namespace Ui {
class usercheck;
}

class usercheck : public QWidget
{
    Q_OBJECT

public:
    explicit usercheck(QString flightID,QString departure_air,QString arrival_air,QString class_,QString departure_date,QString ticketID,
                        QString departure_datetime,QWidget *parent = nullptr);
    ~usercheck();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::usercheck *ui;
    QString flightID;
    QString departure_air;
    QString arrival_air;
    QString class_;
    QString user;
    QString departure_date;
    QString ticketID;
    QString departure_datetime;

    QString password;
};

#endif // USERCHECK_H
