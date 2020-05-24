#include "modcom.h"
#include "ui_modcom.h"

extern QSqlDatabase db;
extern mainplatformwindow *w;

modcom::modcom(QWidget *parent,QString ID,QString name,QString account) :
    QWidget(parent),
    ui(new Ui::modcom),ID(ID),name(name),account(account)
{
    ui->setupUi(this);
    ui->ID->setText(ID);
    ui->ID->setReadOnly(true);
    ui->name->setText(name);
    ui->account->setText(account);
}

modcom::~modcom()
{
    delete ui;
}

void modcom::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->button(QDialogButtonBox::Ok) == button)
    {

        ID = ui->ID->text();
        account = ui->account->text();
        name = ui->name->text();

        QProgressDialog dialog(tr("Modifying"),tr("cancel"), 0, 30000, this);
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
        sql = QString("UPDATE company SET company_name='%1',company_account='%2' WHERE company_id='%3'")
                .arg(name).arg(account).arg(ID);
        QSqlQuery query;
        bool ok = query.exec(sql);
        if(ok){
            QMessageBox::information(this,tr("hint:"),tr("modify successfully"));
            w->compRefresh();
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
