#ifndef MODIFYDIALOG_H
#define MODIFYDIALOG_H

#include <QDialog>
#include <vector>
#include <QtWidgets>
#include <QAbstractButton>
#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSqlQuery>
#include <QCryptographicHash>
#include "mainplatformwindow.h"
#include "queryplugin.h"
#include "search_result.h"
#include"modifyplugin.h"
#include"queryplugin.h"
namespace Ui {
class modifydialog;
}

class modifydialog : public QDialog
{
    Q_OBJECT

public:
    explicit modifydialog(QWidget *parent = nullptr);
    explicit modifydialog(QString table_input,QStringList sqllist_input,QStringList indexlist_input,QWidget *parent = nullptr);
    ~modifydialog();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::modifydialog *ui;
    int count,count_2;
    QStringList indexlist;
    QStringList sqllist;
    QString table;
    std::vector<queryplugin*> ptr;
    std::vector<modifyplugin*> ptr2;
};

#endif // MODIFYDIALOG_H
