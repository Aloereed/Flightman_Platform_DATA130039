#ifndef MODADMIN_H
#define MODADMIN_H

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
class modadmin;
}

class modadmin : public QWidget
{
    Q_OBJECT

public:
    explicit modadmin(QWidget *parent = nullptr,QString ID=NULL,QString name=NULL,QString type=NULL);
    ~modadmin();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::modadmin *ui;
    QString ID;
    QString name;
    QString type;
    QString passwordmd5;
};

#endif // MODADMIN_H
