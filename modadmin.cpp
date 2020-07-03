#include "modadmin.h"
#include "ui_modadmin.h"

extern QSqlDatabase db;
extern mainplatformwindow *w;

modadmin::modadmin(QWidget *parent,QString ID_input,QString name_input,QString type_input) :
    QWidget(parent),
    ui(new Ui::modadmin),ID(ID_input),name(name_input),type(type_input)
{
    ui->setupUi(this);
    ui->setupUi(this);
    ui->ID->setText(ID);
    ui->ID->setReadOnly(true);
    ui->name->setText(name);
    if(type=="√")
        ui->checktypeBox->setChecked(true);
    else
        ui->checktypeBox->setChecked(false);
}

modadmin::~modadmin()
{
    delete ui;
}

void modadmin::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->button(QDialogButtonBox::Ok) == button)
    {
        QByteArray bytePwd = ui->password->text().toLatin1();
        QByteArray bytePwdMd5 = QCryptographicHash::hash(bytePwd, QCryptographicHash::Md5);
        QString strPwdMd5 = bytePwdMd5.toHex();

        ID = ui->ID->text();
        name = ui->name->text();
        type = QString(ui->checktypeBox->isChecked()?"1":"0");
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
            sql = QString("UPDATE admin SET adminName='%1',satype='%2' WHERE adminID='%3'" )
                   .arg(name).arg(type).arg(ID);
        else
            sql = QString("UPDATE admin SET adminName='%1',satype='%2' ,password='%3' WHERE adminID='%4'")
               .arg(name).arg(type).arg(passwordmd5).arg(ID);
        QSqlQuery query;
        bool ok = query.exec(sql);
        if(ok){
            QMessageBox::information(this,tr("hint:"),tr("modify successfully"));
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
