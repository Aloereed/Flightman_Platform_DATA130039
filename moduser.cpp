#include "moduser.h"
#include "ui_moduser.h"

extern QSqlDatabase db;
extern mainplatformwindow *w;

moduser::moduser(QWidget *parent,QString ID_input,QString name_input,QString membership_input,QString account_input) :
    QWidget(parent),
    ui(new Ui::moduser),ID(ID_input),name(name_input),membership(membership_input),account(account_input)
{
    ui->setupUi(this);
    ui->ID->setText(ID);
    ui->ID->setReadOnly(true);
    ui->name->setText(name);
    if(membership=="1")
        ui->checkVIPBox->setChecked(true);
    else
        ui->checkVIPBox->setChecked(false);
    ui->account->setText(account);

}

moduser::~moduser()
{
    delete ui;
}

void moduser::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->button(QDialogButtonBox::Ok) == button)
    {
        QByteArray bytePwd = ui->password->text().toLatin1();
        QByteArray bytePwdMd5 = QCryptographicHash::hash(bytePwd, QCryptographicHash::Md5);
        QString strPwdMd5 = bytePwdMd5.toHex();

        ID = ui->ID->text();
        account = ui->account->text();
        name = ui->name->text();
        membership = QString(ui->checkVIPBox->isChecked()?"1":"0");
        account = ui->account->text();
        passwordmd5=strPwdMd5;

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
        if(ui->password->text().isEmpty())
            sql = QString("UPDATE user SET name='%1',membership='%2',account='%3' WHERE ID='%4'" )
                   .arg(name).arg(membership).arg(account).arg(ID);
        else
            sql = QString("UPDATE user SET name=%1,membership=%2,account=%3,password=%4 WHERE ID=%5" )
               .arg(name).arg(membership).arg(account).arg(passwordmd5).arg(ID);
        QSqlQuery query;
        bool ok = query.exec(sql);
        if(ok){
            QMessageBox::information(this,tr("hint:"),tr("modify successfully"));
            w->userRefresh();
            this->close();
        }
        else{
            QMessageBox::information(this,tr("hint:"),tr("failure"));
            this->close();
        }
    }
    else if(ui->buttonBox->button(QDialogButtonBox::Cancel) == button)
    {
        this->close();
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
       }

}
