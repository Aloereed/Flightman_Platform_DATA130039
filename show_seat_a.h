#ifndef SHOW_SEAT_A_H
#define SHOW_SEAT_A_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QProgressDialog>
#include <QAbstractButton>
#include <QStandardItemModel>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QItemSelectionModel>
#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSqlQuery>
#include <QCryptographicHash>

namespace Ui {
class show_seat_a;
}

class show_seat_a : public QWidget
{
    Q_OBJECT

public:
    explicit show_seat_a(QString tablename, QString departure_date, QString flight_id, QWidget *parent = nullptr);
    ~show_seat_a();

private:
    Ui::show_seat_a *ui;
    QString tablename;
    QString departure_date;
    QString flight_id;
    QSqlQueryModel *seat;
};

#endif // SHOW_SEAT_A_H
