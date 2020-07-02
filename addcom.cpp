#include "addcom.h"
#include "ui_addcom.h"
extern QSqlDatabase db;
extern mainplatformwindow *w;
extern QTableView *usertable;
addcom::addcom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addcom)
{
    ui->setupUi(this);
}

addcom::~addcom()
{
    delete ui;
}

void addcom::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->button(QDialogButtonBox::Ok) == button)
    {
        my_comp tran;
        tran.ID = ui->ID->text();
        tran.name = ui->name->text();
        tran.account = ui->account->text();

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
        sql = QString("INSERT INTO company (company_id,company_name,company_account)"
                      "VALUES('%1','%2','%3')")
                .arg(tran.ID).arg(tran.name).arg(tran.account);
        QSqlQuery query;
        bool ok = query.exec(sql);
        if(ok){
            QMessageBox::information(this,tr("hint:"),tr("add successfully"));
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
