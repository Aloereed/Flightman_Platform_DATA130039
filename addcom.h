#ifndef ADDCOM_H
#define ADDCOM_H

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
class addcom;
}

class addcom : public QWidget
{
    Q_OBJECT

public:
    explicit addcom(QWidget *parent = nullptr);
    ~addcom();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::addcom *ui;
};
typedef struct ComInfo
{
    QString ID;
    QString name;
    QString account;

}my_comp;
#endif // ADDCOM_H
