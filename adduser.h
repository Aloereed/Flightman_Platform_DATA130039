#ifndef ADDUSER_H
#define ADDUSER_H

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
class adduser;
}

class adduser : public QWidget
{
    Q_OBJECT

public:
    explicit adduser(QWidget *parent = nullptr);
    ~adduser();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::adduser *ui;
};

typedef struct UserInfo
{
    QString ID;
    QString name;
    QString membership;
    QString account;
    QString passwordmd5;
}my_user;

#endif // ADDUSER_H
