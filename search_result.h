#ifndef SEARCH_RESULT_H
#define SEARCH_RESULT_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QProgressDialog>

namespace Ui {
class search_result;
}

class search_result : public QWidget
{
    Q_OBJECT

public:
    explicit search_result(QString sql,QWidget *parent = nullptr);
    ~search_result();

private slots:
    void on_pushButton_clicked();

private:
    Ui::search_result *ui;
};

#endif // SEARCH_RESULT_H
