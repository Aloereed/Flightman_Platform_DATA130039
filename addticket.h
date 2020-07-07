#ifndef ADDTICKET_H
#define ADDTICKET_H

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
#include "mainplatformwindow.h"
#include "usercheck.h"
namespace Ui {
class addticket;
}
class addticketmodel:public QSqlQueryModel{
public:
    QVariant data(const QModelIndex &item, int role=Qt::DisplayRole) const;
};

class addticket : public QWidget
{
    Q_OBJECT

public:
    explicit addticket(QWidget *parent = nullptr);
    ~addticket();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    QString getRandomString(int length);

private:
    Ui::addticket *ui;
    QTableView *ticket;
    addticketmodel *add_ticket;
};

#endif // ADDTICKET_H
