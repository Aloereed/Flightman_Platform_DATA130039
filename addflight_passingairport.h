#ifndef ADDFLIGHT_PASSINGAIRPORT_H
#define ADDFLIGHT_PASSINGAIRPORT_H

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

typedef struct PassingInfo
{
    QString airport_id;
    QString arrival_time;
    QString departure_time;

}my_pass;

namespace Ui {
class addflight_passingairport;
}

class addflight_passingairport : public QWidget
{
    Q_OBJECT

public:
    explicit addflight_passingairport(QWidget *parent = nullptr);
    ~addflight_passingairport();
signals:
    void sendPass(my_pass);

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

    void on_radioButton_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_2_clicked();

private:
    Ui::addflight_passingairport *ui;
};

#endif // ADDFLIGHT_PASSINGAIRPORT_H
