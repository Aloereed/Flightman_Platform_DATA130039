#include "addannouncement.h"
#include "ui_addannouncement.h"
extern QSqlDatabase db;
extern mainplatformwindow *w;
addannouncement::addannouncement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addannouncement)
{
    ui->setupUi(this);
    ui->all->setChecked(true);
}

addannouncement::~addannouncement()
{
    delete ui;
}

void addannouncement::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->button(QDialogButtonBox::Ok) == button)
    {
        QString userID;
        if(ui->all->isChecked()){
            userID=QString("000000000000000000");
        }
        else{
            userID=ui->lineEdit->text();
        }
        QString message = ui->textEdit->toPlainText();
        QDateTime datetime = QDateTime::currentDateTime();
        QString time = datetime.toString("yyyy-MM-dd hh:mm:ss");


        QProgressDialog dialog(tr("Delivering"),tr("cancel"), 0, 30000, this);
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
        sql = QString("INSERT INTO announcement (userID,time,text)"
                      "VALUES('%1','%2','%3')")
                .arg(userID).arg(time).arg(message);
        QSqlQuery query;
        bool ok = query.exec(sql);
        if(ok){
            QMessageBox::information(this,tr("hint:"),tr("deliver successfully"));
            w->annoucementRefresh();
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
