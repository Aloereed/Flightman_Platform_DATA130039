#ifndef MODFLIARRANGE_H
#define MODFLIARRANGE_H

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

namespace Ui {
class modfliarrange;
}

class modfliarrange : public QWidget
{
    Q_OBJECT

public:
    explicit modfliarrange(QWidget *parent = nullptr);
    explicit modfliarrange(QWidget *parent = nullptr);
    ~modfliarrange();

private:
    Ui::modfliarrange *ui;
};

#endif // MODFLIARRANGE_H
