#ifndef QUERYPLUGIN_H
#define QUERYPLUGIN_H

#include <QWidget>

namespace Ui {
class queryplugin;
}

class queryplugin : public QWidget
{
    Q_OBJECT

public:
    explicit queryplugin(QWidget *parent = nullptr);
    explicit queryplugin(QStringList indexlist, QWidget *parent = nullptr);
    QString sign_1();
    QString sign_2();
    int index();
    QString text();
    ~queryplugin();

private:
    Ui::queryplugin *ui;
};

#endif // QUERYPLUGIN_H
