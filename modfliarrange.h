#ifndef MODFLIARRANGE_H
#define MODFLIARRANGE_H

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
#include <QSqlTableModel>
#include <QDate>
#include "mainplatformwindow.h"

namespace Ui {
class modfliarrange;
}

class modfliarrange : public QWidget
{
    Q_OBJECT

public:
    explicit modfliarrange( QString departure_date,QString flight_id,QString status,
                            QString discount,QWidget *parent = nullptr);
    ~modfliarrange();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::modfliarrange *ui;
    QString year;
    QString month;
    QString day;
    QString flight_id;
    QString status;
    QString discount;
};

#endif // MODFLIARRANGE_H
