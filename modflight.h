#ifndef MODFLIGHT_H
#define MODFLIGHT_H

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
#include "mainplatformwindow.h"
#include "addflight_passingairport.h"
#include "addseat.h"
#include "add_price.h"
namespace Ui {
class modflight;
}

class modflight : public QWidget
{
    Q_OBJECT

public:
    explicit modflight(QWidget *parent = nullptr,QString flight_id_input = NULL,QString schedule_input=NULL,
                            QString company_id_input =NULL,QString plane_type_input = NULL);
    ~modflight();

private slots:
    void my_seat_get(my_seat tran);
    void my_pass_get(my_pass tran);
    void my_price_get(std::vector<std::vector<QString>>);
    void on_ADD_clicked();

    void on_pushButton_2_clicked();

    void on_seat_clicked();

    void on_buttonBox_clicked(QAbstractButton *button);



    void on_price_clicked();

private:
    Ui::modflight *ui;
    QSqlTableModel *stop_over;
    QString flight_id;
    QString schedule;
    QString company_id;
    QString plane_type;
    std::vector<std::vector<QString>> price;
    bool type;
    int business;
    int economy;
};

#endif // MODFLIGHT_H
