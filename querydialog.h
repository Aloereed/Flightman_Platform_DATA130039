#ifndef QUERYDIALOG_H
#define QUERYDIALOG_H

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

namespace Ui {
class QueryDialog;
}

class QueryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QueryDialog(QWidget *parent = nullptr);
    explicit QueryDialog(QString table_input,QStringList sqllist_input,QStringList indexlist_input,QWidget *parent = nullptr);

    ~QueryDialog();

private slots:
    void on_pushButton_clicked();

    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::QueryDialog *ui;
    int count;
    QStringList indexlist;
    QStringList sqllist;
    QString table;
    std::vector<queryplugin*> ptr;
};

#endif // QUERYDIALOG_H
