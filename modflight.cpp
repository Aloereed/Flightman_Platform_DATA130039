#include "modflight.h"
#include "ui_modflight.h"
extern QSqlDatabase db;
extern mainplatformwindow *w;
extern QTableView *usertable;
modflight::modflight(QWidget *parent,QString flight_id_input,QString schedule_input,
                     QString company_id_input ,QString plane_type_input) :
    QWidget(parent),
    ui(new Ui::modflight)
{
    ui->setupUi(this);
    flight_id=flight_id_input;
    schedule=schedule_input;
    company_id=company_id_input;
    plane_type=plane_type_input;
    ui->flight_id->setText(flight_id);
    ui->schedule->setText(schedule);
    ui->company_id->setText(company_id);
    ui->planetype->setText(plane_type);
    ui->flight_id->setReadOnly(true);

    stop_over = new QSqlTableModel(this);
    stop_over->setTable("airline");
    stop_over->setEditStrategy(QSqlTableModel::OnManualSubmit);
    stop_over->setFilter(QObject::tr("flight_id='%1'").arg(flight_id));
    stop_over->select();
    ui->tableView_airport->setModel(stop_over);
}

modflight::~modflight()
{
    delete ui;
}

void modflight::on_ADD_clicked()
{
    int rowNum = stop_over->rowCount();
    stop_over->insertRow(rowNum);
    stop_over->setData(stop_over->index(rowNum,0),flight_id);
}


void modflight::on_pushButton_2_clicked()
{
    int curRow = ui->tableView_airport->currentIndex().row();

    int ok = QMessageBox::warning(this,tr("Delete the current row!"),tr("Sure?"),
                            QMessageBox::Yes,QMessageBox::No);
       if(ok == QMessageBox::No)
       {
          return;
       }
       else
           stop_over->removeRow(curRow);
}

void modflight::on_seat_clicked()
{
    addseat *a = new addseat(flight_id);
    a->show();
    connect(a,SIGNAL(sendseat(my_seat)),this,SLOT(my_seat_get(my_seat)));
}
void modflight::my_seat_get(my_seat tran){
    type = tran.type;
    business = tran.business;
    economy = tran.economy;
}

void modflight::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->button(QDialogButtonBox::Ok) == button)
    {

        flight_id = ui->flight_id->text();
        schedule = ui->schedule->text();
        plane_type = ui->planetype->text();
        company_id = ui->company_id->text();

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

        if(QSqlDatabase::database().transaction()){
            QSqlQuery query;
            QString sql1;
            QString sql2;
            sql1 = QString("UPDATE flight SET schedule='%1',plane_type='%2',company_id='%3' WHERE flight_id='%3'")
                    .arg(schedule).arg(plane_type).arg(company_id).arg(flight_id);
            if(type == true)
                sql2 = QString("UPDATE seat SET type='%1',`business_No.`='%2',`economy_No.`='%3' WHERE flight_id='%3'")
                    .arg(1).arg(business).arg(economy).arg(flight_id);
            else
                sql2 = QString("UPDATE seat SET type='%1',`business_No.`='%2',`economy_No.`='%3' WHERE flight_id='%3'")
                    .arg(0).arg(business).arg(economy).arg(flight_id);
            query.exec(sql1);
            query.exec(sql2);
            stop_over->submitAll();
            if(!QSqlDatabase::database().commit()){
                qDebug()<<QSqlDatabase::database().lastError();
                if(!QSqlDatabase::database().rollback()){
                    qDebug()<<QSqlDatabase::database().lastError();
                }
             }
            else{
                QMessageBox::information(this,tr("hint:"),tr("modify successfully"));
                w->flightRefresh();
                this->close();
            }
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
