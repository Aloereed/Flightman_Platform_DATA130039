#ifndef SHOWMESSAGE_H
#define SHOWMESSAGE_H

#include <QWidget>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QAbstractButton>
#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSqlQuery>
#include <QCryptographicHash>
#include <vector>
#include <QDateTime>
#include "mainplatformwindow.h"

namespace Ui {
class showMessage;
}

class showMessage : public QWidget
{
    Q_OBJECT

public:
    explicit showMessage(QString userID,QString datetime,QWidget *parent = nullptr);
    ~showMessage();

private slots:
    void on_pushButton_clicked();

private:
    Ui::showMessage *ui;
    QString userID;
    QString datetime;
};

#endif // SHOWMESSAGE_H
