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
#include<QFileInfo>
#include "mainplatformwindow.h"
QSqlDatabase db;
QTranslator translator;
mainplatformwindow *w;
my_admin tranadmin;
#ifdef Q_OS_ANDROID
#include <QStandardPaths>
QSettings settings(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)+"/plat_settings.ini", QSettings::NativeFormat);
#else
QSettings settings("FDU4021","FBMT");
#endif

loginwindow::loginwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::loginwindow)
{
    //settings.sync();
    ui->setupUi(this);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    QString langdir=":/platform_"+QLocale::system().name()+".qm";
    translator.load(langdir);
    qApp->installTranslator(&translator);
    ui->retranslateUi(this);
    if(settings.value("Platform/AdminisSaved").toBool()){
        ui->lineEdit->setText(settings.value("Platform/AdminID").toString());
        ui->lineEdit_2->setText(settings.value("Platform/AdminPWD").toString());
        ui->checkBox_2->setChecked(true);
    }

    //settings.sync();

}

loginwindow::~loginwindow()
{
    delete ui;
    settings.sync();
}


void loginwindow::on_commandLinkButton_clicked()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
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
          login.exec("select adminpwd,adminName,satype from admin where adminID=\'"+tranadmin.ID+"\'");
          login.next();
          if(tranadmin.passwordmd5==login.value(0).toString()){
          tranadmin.satype=login.value(2).toBool();
          tranadmin.name=login.value(1).toString();
          if(ui->checkBox_2->isChecked()){
              settings.setValue("Platform/AdminID",tranadmin.ID);
              settings.setValue("Platform/AdminPWD",ui->lineEdit_2->text());
              settings.sync();
          }
          settings.setValue("Platform/AdminisSaved",ui->checkBox_2->isChecked());
          settings.sync();
          QApplication::processEvents();
          w = new mainplatformwindow();
          w->setWindowIcon(QIcon(":/icon.png"));
          QApplication::processEvents();
          w->show();
          QApplication::processEvents();
          this->close();
          }else      {
              QMessageBox::critical(this,"ID or Password Wrong.","Your ID or Password is Wrong.");
              return;
          }
      }
      else
      {
            QMessageBox::critical(this,"Server or client meets trouble!",db.lastError().text());
          return;
      }
}
