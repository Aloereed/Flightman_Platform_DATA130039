#include "addairport.h"
#include "ui_addairport.h"
extern QSqlDatabase db;
extern mainplatformwindow *w;
addairport::addairport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addairport)
{
    ui->setupUi(this);
}

addairport::~addairport()
{
    delete ui;
}

void addairport::on_buttonBox_airport_clicked(QAbstractButton *button)
{
    if(ui->buttonBox_airport->button(QDialogButtonBox::Ok) == button)
    {
        my_airp tran;
        tran.airport_id = ui->airport_id->text();
        tran.airport_name = ui->airport_name->text();
        tran.city = ui->city->text();

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
        sql = QString("INSERT INTO airport (airport_id,airport_name,city)"
                      "VALUES('%1','%2','%3')")
                .arg(tran.airport_id).arg(tran.airport_name).arg(tran.city);
        QSqlQuery query;
        bool ok = query.exec(sql);
        if(ok){
            QMessageBox::information(this,tr("hint:"),tr("add successfully"));
            w->on_horizontalSlider_2_valueChanged(1);
            this->close();
        }
        else{
            QMessageBox::information(this,tr("hint:"),tr("failure"));
            this->close();
        }
    }
    else if(ui->buttonBox_airport->button(QDialogButtonBox::Cancel) == button)
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
