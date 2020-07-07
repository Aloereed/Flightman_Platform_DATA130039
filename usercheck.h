#ifndef USERCHECK_H
#define USERCHECK_H

#include <QWidget>

namespace Ui {
class usercheck;
}

class usercheck : public QWidget
{
    Q_OBJECT

public:
    explicit usercheck(QWidget *parent = nullptr);
    ~usercheck();

private:
    Ui::usercheck *ui;
};

#endif // USERCHECK_H
