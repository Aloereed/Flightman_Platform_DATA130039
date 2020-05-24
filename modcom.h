#ifndef MODCOM_H
#define MODCOM_H

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
class modcom;
}

class modcom : public QWidget
{
    Q_OBJECT

public:
    explicit modcom(QWidget *parent = nullptr,QString ID=NULL,QString name=NULL,QString account=NULL);
    ~modcom();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::modcom *ui;
    QString ID;
    QString name;
    QString account;
};

#endif // MODCOM_H
