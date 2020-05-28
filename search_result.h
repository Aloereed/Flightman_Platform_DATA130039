#ifndef SEARCH_RESULT_H
#define SEARCH_RESULT_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QProgressDialog>
#include <QAbstractButton>
#include <QStandardItemModel>
#include <QSqlTableModel>
#include <QItemSelectionModel>
#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSqlQuery>
#include <QCryptographicHash>
#include "mainplatformwindow.h"

namespace Ui {
class search_result;
}

class search_result : public QWidget
{
    Q_OBJECT

public:
    explicit search_result(QSqlTableModel *search_input,QWidget *parent = nullptr);
    ~search_result();

private slots:
    void on_pushButton_clicked();

    void on_submit_clicked();

    void on_retract_clicked();

private:
    Ui::search_result *ui;
    QSqlTableModel *search;
};

#endif // SEARCH_RESULT_H
