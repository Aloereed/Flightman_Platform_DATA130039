#ifndef ADDSEAT_H
#define ADDSEAT_H

#include <QWidget>
#include <QAbstractButton>
#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QSignalMapper>
#include "mainplatformwindow.h"
typedef struct SeatInfo
{
    bool type;
    int business;
    int economy;

}my_seat;
namespace Ui {
class addseat;
}

class addseat : public QWidget
{
    Q_OBJECT

public:
    explicit addseat(QWidget *parent = nullptr);
    addseat(QString flight_id,QWidget *parent = nullptr);
    ~addseat();
signals:
    void sendseat(my_seat);
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::addseat *ui;
};

#endif // ADDSEAT_H
