#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QCryptographicHash>

QT_BEGIN_NAMESPACE
namespace Ui { class loginwindow; }
QT_END_NAMESPACE

class loginwindow : public QMainWindow
{
    Q_OBJECT

public:
    loginwindow(QWidget *parent = nullptr);
    ~loginwindow();

private slots:
    void on_commandLinkButton_clicked();

private:
    Ui::loginwindow *ui;
};
typedef struct AdminInfo
{
    QString ID;
    QString name;
    QString passwordmd5;
    bool satype;
}my_admin;
#endif // LOGINWINDOW_H
