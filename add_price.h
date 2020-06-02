#ifndef ADD_PRICE_H
#define ADD_PRICE_H

#include <QWidget>

namespace Ui {
class add_price;
}

class add_price : public QWidget
{
    Q_OBJECT

public:
    explicit add_price(QWidget *parent = nullptr);
    ~add_price();

private:
    Ui::add_price *ui;
};

#endif // ADD_PRICE_H
