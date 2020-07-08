#include "showmessage.h"
#include "ui_showmessage.h"
extern QSqlDatabase db;
extern mainplatformwindow *w;

showMessage::showMessage(QString userID_input,QString datetime_input,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::showMessage)
{
    ui->setupUi(this);
    userID=userID_input;
    datetime=datetime_input;
    ui->textEdit->setReadOnly(true);

    QString sql = QString("SELECT text FROM announcement WHERE userID = '%1' AND time = '%2'").arg(userID).arg(datetime);
    QSqlQuery query;
    query.exec(sql);
    query.next();
    QString text = query.value(0).toString();
    ui->textEdit->setText(text);


}

showMessage::~showMessage()
{
    delete ui;
}

void showMessage::on_pushButton_clicked()
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
