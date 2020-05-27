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
    ~queryplugin();

private:
    Ui::queryplugin *ui;
};

#endif // QUERYPLUGIN_H
