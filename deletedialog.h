#ifndef DELETEDIALOG_H
#define DELETEDIALOG_H

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

namespace Ui {
class deletedialog;
}

class deletedialog : public QWidget
{
    Q_OBJECT

public:
    explicit deletedialog(QWidget *parent = nullptr);
    explicit deletedialog(QString table_input,QStringList sqllist_input,QStringList indexlist_input,QWidget *parent = nullptr);
    ~deletedialog();

private slots:
    void on_pushButton_clicked();

    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::deletedialog *ui;
    int count;
    QStringList indexlist;
    QStringList sqllist;
    QString table;
    std::vector<queryplugin*> ptr;
};

#endif // DELETEDIALOG_H
