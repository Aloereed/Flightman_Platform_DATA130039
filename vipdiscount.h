#ifndef VIPDISCOUNT_H
#define VIPDISCOUNT_H

#include <QDialog>

namespace Ui {
class VIPDiscount;
}

class VIPDiscount : public QDialog
{
    Q_OBJECT

public:
    explicit VIPDiscount(QWidget *parent = nullptr);
    ~VIPDiscount();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::VIPDiscount *ui;
};

#endif // VIPDISCOUNT_H
