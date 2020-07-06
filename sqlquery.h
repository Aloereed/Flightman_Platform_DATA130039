#ifndef SQLQUERY_H
#define SQLQUERY_H

#include <QWidget>

namespace Ui {
class SQLQuery;
}

class SQLQuery : public QWidget
{
    Q_OBJECT

public:
    explicit SQLQuery(QWidget *parent = nullptr);
    ~SQLQuery();

private:
    Ui::SQLQuery *ui;
};

#endif // SQLQUERY_H
