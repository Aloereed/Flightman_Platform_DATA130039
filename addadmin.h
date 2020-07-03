#ifndef ADDADMIN_H
#define ADDADMIN_H

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
class addadmin;
}

class addadmin : public QWidget
{
    Q_OBJECT

public:
    explicit addadmin(QWidget *parent = nullptr);
    ~addadmin();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::addadmin *ui;
};
typedef struct adminInfo
{
    QString ID;
    QString name;
    QString type;
    QString passwordmd5;
}my_Admin;
#endif // ADDADMIN_H
