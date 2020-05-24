#ifndef ADDFLIGHT_H
#define ADDFLIGHT_H

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
#include "mainplatformwindow.h"
#include "addflight_passingairport.h"
#include "addseat.h"
namespace Ui {
class addflight;
}

class addflight : public QWidget
{
    Q_OBJECT

private slots:
    //void on_actAppend_triggered();
    //void on_actDelete_triggered();
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
//    bool sql_query2(QString);
//    bool sql_query3(QString);

    void on_ADD_clicked();

public:
    explicit addflight(QWidget *parent = nullptr);
    ~addflight();
private slots:
    void my_pass_get(my_pass tran);
    void my_seat_get(my_seat tran);
    void on_pushButton_2_clicked();

    void on_seat_clicked();

private:
    Ui::addflight *ui;
    QStandardItemModel *airport;
    QItemSelectionModel *theselection;
    bool type;
    int business;
    int economy;
};
typedef struct FlightInfo
{
    QString flight_id;
    QString schedule;
    QString company_id;
    QString plane_type;


}my_fli;
#endif // ADDFLIGHT_H
