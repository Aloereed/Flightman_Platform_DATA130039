#ifndef ADDFLIARRANGE_H
#define ADDFLIARRANGE_H

#include <QWidget>
#include <QAbstractButton>
#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QDate>
#include <QDateTime>
#include "mainplatformwindow.h"

namespace Ui {
class addfliarrange;
}

class addfliarrange : public QWidget
{
    Q_OBJECT

public:
    explicit addfliarrange(QWidget *parent = nullptr);
    ~addfliarrange();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::addfliarrange *ui;
    QDate now;
};
typedef struct fliarrangeInfo
{
    QString year;
    QString month;
    QString day;
    QString flight_id;
    QString status;
    QString dicount;

}my_fliarrange;

#endif // ADDFLIARRANGE_H
