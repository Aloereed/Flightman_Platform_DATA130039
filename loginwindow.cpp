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
my_admin tranadmin;
QSettings settings("FDU4021","FBMT");
loginwindow::loginwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::loginwindow)
{
    ui->setupUi(this);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    QString langdir=QApplication::applicationDirPath()+"/platform_"+QLocale::system().name()+".qm";
    translator.load(langdir);
    qApp->installTranslator(&translator);
    ui->retranslateUi(this);
    if(settings.value("AdminisSaved").toBool()){
        ui->lineEdit->setText(settings.value("AdminID").toString());
        ui->lineEdit_2->setText(settings.value("AdminPWD").toString());
        ui->checkBox_2->setChecked(true);
    }
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
          QByteArray bytePwd = ui->lineEdit_2->text().toLatin1();
          QByteArray bytePwdMd5 = QCryptographicHash::hash(bytePwd, QCryptographicHash::Md5);
          QString strPwdMd5 = bytePwdMd5.toHex();
          tranadmin.ID = ui->lineEdit->text();
          tranadmin.passwordmd5=strPwdMd5;
          QSqlQuery login;
          login.exec("select adminpwd,adminName from admin where adminID=\'"+tranadmin.ID+"\'");
          login.next();
          if(tranadmin.passwordmd5==login.value(0).toString()){
          tranadmin.name=login.value(1).toString();
          if(ui->checkBox_2->isChecked()){
              settings.setValue("AdminID",tranadmin.ID);
              settings.setValue("AdminPWD",ui->lineEdit_2->text());
          }
          settings.setValue("AdminisSaved",ui->checkBox_2->isChecked());
          QApplication::processEvents();
          w = new mainplatformwindow();
          QApplication::processEvents();
          w->show();
          QApplication::processEvents();
          this->hide();
          }else      {
              QMessageBox::critical(this,"ID or Password Wrong.","Your ID or Password is Wrong.");
              return;
          }
      }
      else
      {
          QMessageBox::critical(this,"Server is wrong, reset please.",db.lastError().text());
          return;
      }
}
