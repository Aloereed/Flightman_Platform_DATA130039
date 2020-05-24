#ifndef STOPOVER_H
#define STOPOVER_H

#include <QWidget>
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
class stopover;
}

class stopover : public QWidget
{
    Q_OBJECT

public:
    explicit stopover(QWidget *parent = nullptr,QString flight_id_input = NULL);
    ~stopover();

private slots:
    void on_back_clicked();

    void on_submit_clicked();

    void on_retract_clicked();

    void on_delete_2_clicked();

    void on_add_clicked();

private:
    Ui::stopover *ui;
    QSqlTableModel *stop_over;
    QString flight_id;
};

#endif // STOPOVER_H
