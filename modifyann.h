#ifndef MODIFYANN_H
#define MODIFYANN_H

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
class modifyann;
}

class modifyann : public QWidget
{
    Q_OBJECT

public:
    explicit modifyann(QString userID,QString datetime,QWidget *parent = nullptr);
    ~modifyann();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::modifyann *ui;
    QString userID;
    QString datetime;
};

#endif // MODIFYANN_H
