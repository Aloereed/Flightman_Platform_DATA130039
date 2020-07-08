#include "modifyann.h"
#include "ui_modifyann.h"
extern QSqlDatabase db;
extern mainplatformwindow *w;

modifyann::modifyann(QString userID_input,QString datetime_input,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::modifyann)
{
    ui->setupUi(this);
    userID=userID_input;
    datetime=datetime_input;

}

modifyann::~modifyann()
{
    delete ui;
}

void modifyann::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->button(QDialogButtonBox::Ok) == button)
    {

        QString message = ui->textEdit->toPlainText();

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
        sql = QString("UPDATE announcement SET text = '%1' WHERE userID='%2' AND time='%3'")
                .arg(message).arg(userID).arg(datetime);
        QSqlQuery query;
        bool ok = query.exec(sql);
        if(ok){
            QMessageBox::information(this,tr("hint:"),tr("modify successfully"));
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
