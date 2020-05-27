#ifndef QUERYDIALOG_H
#define QUERYDIALOG_H

#include <QDialog>

namespace Ui {
class QueryDialog;
}

class QueryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QueryDialog(QWidget *parent = nullptr);
    ~QueryDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::QueryDialog *ui;
};

#endif // QUERYDIALOG_H
