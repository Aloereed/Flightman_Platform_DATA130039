#ifndef ADD_PRICE_H
#define ADD_PRICE_H

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

namespace Ui {
class add_price;
}

class add_price : public QWidget
{
    Q_OBJECT

public:
    explicit add_price(QWidget *parent = nullptr);
    explicit add_price(QStringList airportlist,QWidget *parent = nullptr);
    explicit add_price(std::vector<std::vector<QString>> whole_price,QWidget *parent = nullptr);
    ~add_price();

signals:
    void sendprice(std::vector<std::vector<QString>>);

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::add_price *ui;
    QStandardItemModel *price;
    QItemSelectionModel *theselection;
    std::vector<std::vector<QString>> price_vector;
};

#endif // ADD_PRICE_H
