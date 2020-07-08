#ifndef ADDANNOUNCEMENT_H
#define ADDANNOUNCEMENT_H

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
#include <QDateTime>
#include "mainplatformwindow.h"


namespace Ui {
class addannouncement;
}

class addannouncement : public QWidget
{
    Q_OBJECT

public:
    explicit addannouncement(QWidget *parent = nullptr);
    ~addannouncement();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::addannouncement *ui;
};

#endif // ADDANNOUNCEMENT_H
