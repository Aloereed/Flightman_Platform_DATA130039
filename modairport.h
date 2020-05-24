#ifndef MODAIRPORT_H
#define MODAIRPORT_H

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
class modairport;
}

class modairport : public QWidget
{
    Q_OBJECT

public:
    explicit modairport(QWidget *parent = nullptr,QString airport_id=NULL,QString airport_name=NULL,QString city=NULL);
    ~modairport();

private slots:
    void on_buttonBox_airport_clicked(QAbstractButton *button);

private:
    Ui::modairport *ui;
    QString airport_id;
    QString airport_name;
    QString city;

};

#endif // MODAIRPORT_H
