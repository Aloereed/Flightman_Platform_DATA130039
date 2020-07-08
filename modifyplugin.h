#ifndef MODIFYPLUGIN_H
#define MODIFYPLUGIN_H

#include <QWidget>

namespace Ui {
class modifyplugin;
}

class modifyplugin : public QWidget
{
    Q_OBJECT

public:
    explicit modifyplugin(QWidget *parent = nullptr);
    explicit modifyplugin(QStringList indexlist, QWidget *parent = nullptr);
    int index();
    QString text();
    ~modifyplugin();

private:
    Ui::modifyplugin *ui;

};

#endif // MODIFYPLUGIN_H
