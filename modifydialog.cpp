#include "modifydialog.h"
#include "ui_modifydialog.h"

modifydialog::modifydialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modifydialog)
{
    ui->setupUi(this);
    count=0;

}
modifydialog::modifydialog(QString table_input,QStringList sqllist_input,QStringList indexlist_input,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modifydialog)
{
    ui->setupUi(this);
    indexlist=indexlist_input;
    sqllist=sqllist_input;
    table=table_input;
    count=0;
    count_2=0;
}
modifydialog::~modifydialog()
{
    delete ui;
}

void modifydialog::on_pushButton_2_clicked()
{
    modifyplugin* ptr_temp = new modifyplugin(indexlist);
    count_2++;
    ui->verticalLayout_4->addWidget(ptr_temp);
    ptr2.push_back(ptr_temp);
}

void modifydialog::on_pushButton_clicked()
{
    queryplugin* ptr_temp = new queryplugin(indexlist);
    count++;
    ui->verticalLayout_3->addWidget(ptr_temp);
    ptr.push_back(ptr_temp);
}

void modifydialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->button(QDialogButtonBox::Ok) == button)
    {
        QString sql1;
        QString sql2;
        /*
        QSqlTableModel *search;
        search = new QSqlTableModel(this);
        search->setTable(table);
        search->setEditStrategy(QSqlTableModel::OnManualSubmit);

        */
        sql1 = QString("UPDATE %1 SET ").arg(table);

        for(int j=0;j<count_2;++j){
            if(j!=count_2-1){
            modifyplugin* ptr_temp=ptr2[j];
            QString index = sqllist[ptr_temp->index()];
                QString text = ptr_temp->text();
                sql1 += index+QString("=")+QString("'")+text+QString("'")+QString(",");
            }else{
                modifyplugin* ptr_temp=ptr2[j];
                QString index = sqllist[ptr_temp->index()];
                QString text = ptr_temp->text();
                sql1 += index+QString("=")+QString("'")+text+QString("'")+QString(" WHERE ");
            }
        }

        for(int i=0;i<count;i++){
            queryplugin* ptr_temp= ptr[i];
            if(i==0){
                QString index = sqllist[ptr_temp->index()];

                QString sign_2 = ptr_temp->sign_2();
                if(sign_2==tr("Contain")){
                    QString text = ptr_temp->text();
                    sql2 += QString(" ")+index+QString(" LIKE \'%")+text+QString("%\'")+QString(" ");
                }else{
                    if(sign_2=="=="){
                        sign_2="=";
                    }
                    QString text = ptr_temp->text();
                    sql2 = index+sign_2+QString("'")+text+QString("'")+QString(" ");
                }
            }
            else{
                QString sign_1 = ptr_temp->sign_1();
                QString index = sqllist[ptr_temp->index()];
                QString sign_2 = ptr_temp->sign_2();
                if(sign_2==tr("Contain")){
                    QString text = ptr_temp->text();
                    sql2 += sign_1+QString(" ")+index+QString(" LIKE \'%")+text+QString("%\'")+QString(" ");
                }
                else{
                    if(sign_2=="=="){
                        sign_2="=";
                    }
                    QString text = ptr_temp->text();
                    sql2 += sign_1+QString(" ")+index+sign_2+QString("'")+text+QString("'")+QString(" ");
                }
            }
        }
        sql1 += sql2;
        /*
        search->setFilter(sql2);
        search->select();
        */
        QSqlQuery query;
        bool ok = query.exec(sql1);
        if(ok){
            QMessageBox::information(this,tr("hint:"),tr("Success!"));
        }
        else{
            QMessageBox::information(this,tr("hint:"),tr("failure"));
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
    }
    this->close();
}
