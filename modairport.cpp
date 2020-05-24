#include "modairport.h"
#include "ui_modairport.h"
extern QSqlDatabase db;
extern mainplatformwindow *w;
modairport::modairport(QWidget *parent,QString airport_id,QString airport_name,QString city) :
    QWidget(parent),
    ui(new Ui::modairport)
{
    ui->setupUi(this);
    ui->airport_id->setText(airport_id);
    ui->airport_id->setReadOnly(true);
    ui->airport_name->setText(airport_name);
    ui->city->setText(city);
}

modairport::~modairport()
{
    delete ui;
}

void modairport::on_buttonBox_airport_clicked(QAbstractButton *button)
{
    if(ui->buttonBox_airport->button(QDialogButtonBox::Ok) == button)
    {

        airport_id = ui->airport_id->text();
        airport_name = ui->airport_name->text();
        city = ui->city->text();

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
        sql = QString("UPDATE airport SET airport_name='%1',city='%2' WHERE airport_id='%3'")
                .arg(airport_name).arg(city).arg(airport_id);
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
