#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStringList>
#include <QSettings>
#include <QDebug>
#include<QMessageBox>
#include <QTranslator>
#include <QDateTime>
#include<QProgressDialog>
#include "mainplatformwindow.h"
QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
QTranslator translator;
mainplatformwindow *w;
loginwindow::loginwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::loginwindow)
{
    ui->setupUi(this);
    translator.load("./platform_"+QLocale::system().name()+".qm");

    qApp->installTranslator(&translator);

    ui->retranslateUi(this);
}

loginwindow::~loginwindow()
{
    delete ui;
}


void loginwindow::on_commandLinkButton_clicked()
{
    ui->progressBar->setMaximum(0);
/*    if(ui->checkBox->isChecked()==true){

        ui->editHostname->setText("gz.aloereed.cc,1433\\SQLEXPRESS");
        ui->editDatabase->setText("flights");
        ui->editPassword->setText("sql4021");
        ui->editUsername->setText("sa");

        ui->editHostname->setText("sql.db247.vhostgo.com");
        ui->editDatabase->setText("flights4021");
        ui->editPassword->setText("sql4021");
        ui->editUsername->setText("flights4021");

    }*/
    if(ui->checkBox->isChecked()==true){
        ui->editHostname->setText("cdb-aaxnskss.cd.tencentcdb.com");         //主机名
        ui->portSpinBox->setValue(10095);                    //端口
        ui->editDatabase->setText("flight");          //数据库名
        ui->editUsername->setText("root");              //用户名
        ui->editPassword->setText("sqlfd4021");
    }
    QString serverIp=ui->editHostname->text();
    QString databaseName=ui->editDatabase->text();
    QString user=ui->editUsername->text();
    QString password=ui->editPassword->text();
    int port=ui->portSpinBox->value();
    ui->progressBar->setValue(20);

    if(serverIp==""||databaseName==""||user==""||password=="")
    {
        QMessageBox::critical(this,"Server is wrong, reset please.","critical");
        return;

    }
    db.setHostName(serverIp);         //主机名
    db.setPort(port);                    //端口
    db.setDatabaseName(databaseName);          //数据库名
    db.setUserName(user);              //用户名
    db.setPassword(password);
    //QSqlDatabase db1 = QSqlDatabase::addDatabase("QODBC");

   /* db.setDatabaseName(QString("DRIVER={SQL SERVER};"
                                 "SERVER=%1;"
                                 "DATABASE=%2;"
                                  "UID=%3;"
                                  "PWD=%4;").arg(serverIp)
                                            .arg(databaseName)
                                            .arg(user)
                                            .arg(password));*/
      if (db.open())
      {
          for(int p=50;p<50;p++)
              ui->progressBar->setValue(98);
          ui->progressBar->setValue(98);
          QApplication::processEvents();
          w = new mainplatformwindow();
          QApplication::processEvents();
          w->show();
          QApplication::processEvents();
          this->hide();
      }
      else
      {
          QMessageBox::critical(this,"Server is wrong, reset please.",db.lastError().text());
          return;
      }
}
