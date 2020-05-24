#ifndef MODUSER_H
#define MODUSER_H

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
class moduser;
}

class moduser : public QWidget
{
    Q_OBJECT

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

public:
    explicit moduser(QWidget *parent = nullptr,QString ID=NULL,QString name=NULL,QString membership=NULL,QString account=NULL);
    ~moduser();

private:
    Ui::moduser *ui;
    QString ID;
    QString name;
    QString membership;
    QString account;
    QString passwordmd5;
};

#endif // MODUSER_H
