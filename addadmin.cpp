#include "addadmin.h"
#include "ui_addadmin.h"
extern QSqlDatabase db;
extern mainplatformwindow *w;

addadmin::addadmin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addadmin)
{
    ui->setupUi(this);
    ui->password->setEchoMode(QLineEdit::Password);
}

addadmin::~addadmin()
{
    delete ui;
}

void addadmin::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->button(QDialogButtonBox::Ok) == button)
    {
        QByteArray bytePwd = ui->password->text().toLatin1();
        QByteArray bytePwdMd5 = QCryptographicHash::hash(bytePwd, QCryptographicHash::Md5);
        QString strPwdMd5 = bytePwdMd5.toHex();
        my_Admin tran;
        tran.ID = ui->ID->text();
        tran.name = ui->name->text();
        tran.type = QString(ui->checktypeBox->isChecked()?"1":"0");
        tran.passwordmd5=strPwdMd5;

        QProgressDialog dialog(tr("Adding"),tr("cancel"), 0, 30000, this);
        dialog.setWindowTitle(tr("process"));
        dialog.setWindowModality(Qt::WindowModal);
        dialog.show();
        for(int k = 0; k < 30000; k++)
        {
            dialog.setValue(k);
            QCoreApplication::processEvents();
            if(dialog.wasCanceled())
            {
                break;
            }
        }
        dialog.setValue(30000);

        QString sql;
        sql = QString("INSERT INTO admin (adminID,adminName,satype,adminpwd)"
                      "VALUES('%1','%2','%3','%4')")
                .arg(tran.ID).arg(tran.name).arg(tran.type).arg(tran.passwordmd5);
        QSqlQuery query;
        bool ok = query.exec(sql);
        if(ok){
            QMessageBox::information(this,tr("hint:"),tr("add successfully"));
            w->adminRefresh();
            this->close();
        }
        else{
            QMessageBox::information(this,tr("hint:"),tr("failure"));
            this->close();
        }
    }
    else if(ui->buttonBox->button(QDialogButtonBox::Cancel) == button)
    {
        QProgressDialog dialog(tr("Returning to the mainwindow"),tr("cancel"), 0, 3000, this);
        dialog.setWindowTitle(tr("process"));
        dialog.setWindowModality(Qt::WindowModal);
        dialog.show();
        for(int k = 0; k < 3000; k++)
        {
            dialog.setValue(k);
            QCoreApplication::processEvents();
            if(dialog.wasCanceled())
            {
                break;
            }
        }
        dialog.setValue(3000);
        this->close();
       }

}
