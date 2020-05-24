#ifndef ADDAIRPORT_H
#define ADDAIRPORT_H

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
class addairport;
}

class addairport : public QWidget
{
    Q_OBJECT

public:
    explicit addairport(QWidget *parent = nullptr);
    ~addairport();

private slots:
    void on_buttonBox_airport_clicked(QAbstractButton *button);

private:
    Ui::addairport *ui;
};
typedef struct AirportInfo
{
    QString airport_id;
    QString airport_name;
    QString city;

}my_airp;
#endif // ADDAIRPORT_H
