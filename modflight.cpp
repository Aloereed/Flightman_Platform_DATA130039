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
    stop_over->setSort(4,Qt::AscendingOrder);
    stop_over->select();
    ui->tableView_airport->setModel(stop_over);
}

modflight::~modflight()
{
    delete ui;
}
void modflight::my_pass_get(my_pass tran){
    int count = ui->tableView_airport->model()->rowCount();
    ui->tableView_airport->model()->insertRow(count);
    ui->tableView_airport->model()->setData(ui->tableView_airport->model()->index(count,0),flight_id);
    ui->tableView_airport->model()->setData(ui->tableView_airport->model()->index(count,1),tran.airport_id);
    ui->tableView_airport->model()->setData(ui->tableView_airport->model()->index(count,2),tran.arrival_time);
    ui->tableView_airport->model()->setData(ui->tableView_airport->model()->index(count,3),tran.departure_time);
    ui->tableView_airport->model()->setData(ui->tableView_airport->model()->index(count,4),tran.order);

}
void modflight::on_ADD_clicked()
{
    int rowNum = stop_over->rowCount();
    stop_over->insertRow(rowNum);
    stop_over->setData(stop_over->index(rowNum,0),flight_id);
//    addflight_passingairport* a = new addflight_passingairport;
//    a-> show();
//    connect(a,SIGNAL(sendPass(my_pass)),this,SLOT(my_pass_get(my_pass)));
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

#ifdef Q_OS_ANDROID
        QSqlQuery transaction;
        if(transaction.exec("start transaction")){
#else
        if(QSqlDatabase::database().transaction()){
#endif
            QSqlQuery query;
            QString sql1;
            QString sql2;
            QString sql3;
            QString sql4;
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
            sql3 = QString("DELETE FROM price WHERE flight_id='%1'").arg(flight_id);
            for(int i=0;i<price.size();i++){
                if(price[i][2]=="business"){//if business, set class=0
                       sql1 = QString("INSERT INTO price (flight_id,start_id,end_id,class,price)"
                                          "VALUES('%1',%2,%3,%4,%5)")
                                    .arg(flight_id).arg(price[i][4]).arg(price[i][5]).arg(0).arg(price[i][3]);
                }
                else{
                       sql4 = QString("INSERT INTO price (flight_id,start_id,end_id,class,price)"
                                          "VALUES('%1',%2,%3,%4,%5)")
                                    .arg(flight_id).arg(price[i][4]).arg(price[i][5]).arg(1).arg(price[i][3]);
                 }
                query.exec(sql4);
            }
            stop_over->submitAll();
#ifdef Q_OS_ANDROID
            if(!transaction.exec("commit")){
#else
            if(!QSqlDatabase::database().commit()){
#endif
                qDebug()<<QSqlDatabase::database().lastError();
#ifdef Q_OS_ANDROID
                if(!transaction.exec("rollback")){
#else
                if(!QSqlDatabase::database().rollback()){
#endif
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
void modflight::my_price_get(std::vector<std::vector<QString>> whole_price){
   for(int i=0;i<price.size();i++){
       price[i][3]=whole_price[i][3];
   }
}

void modflight::on_price_clicked()
{
    price.clear();
    int count=ui->tableView_airport->model()->rowCount();
    for(int i=1;i<count;i++){
                for(int j=i+1;j<count;j++){
                    std::vector<QString> row1;
                    std::vector<QString> row2;
                    QModelIndex index = ui->tableView_airport->model()->index(i,1);
                    QString temp=ui->tableView_airport->model()->data(index).toString();
                    row1.push_back(temp);
                    row2.push_back(temp);
                    index = ui->tableView_airport->model()->index(j,1);
                    temp=ui->tableView_airport->model()->data(index).toString();
                    row1.push_back(temp);
                    row2.push_back(temp);
                    row1.push_back("business");
                    row1.push_back("NULL");
                    row2.push_back("economy");
                    row2.push_back("NULL");
                    if(i==count-1){
                        row1.push_back(QString::number(-1));
                        row2.push_back(QString::number(-1));
                    }
                    else{
                        row1.push_back(QString::number(i));
                        row2.push_back(QString::number(i));
                    }
                    if(j==count-1){
                        row1.push_back(QString::number(-1));
                        row2.push_back(QString::number(-1));
                    }
                    else{
                        row1.push_back(QString::number(j));
                        row2.push_back(QString::number(j));
                    }


                    price.push_back(row1);
                    price.push_back(row2);
                }
            }
       for(int i =1;i<count;i++){
           int j = 0;
           std::vector<QString> row1;
           std::vector<QString> row2;
           QModelIndex index = ui->tableView_airport->model()->index(i,1);
           QString temp=ui->tableView_airport->model()->data(index).toString();
           row1.push_back(temp);
           row2.push_back(temp);
           index = ui->tableView_airport->model()->index(j,1);
           temp=ui->tableView_airport->model()->data(index).toString();
           row1.push_back(temp);
           row2.push_back(temp);
           row1.push_back("business");
           row1.push_back("NULL");
           row2.push_back("economy");
           row2.push_back("NULL");
           if(i==count-1){
               row1.push_back(QString::number(-1));
               row2.push_back(QString::number(-1));
           }
           else{
               row1.push_back(QString::number(i));
               row2.push_back(QString::number(i));
           }
           row1.push_back(QString::number(-1));
           row2.push_back(QString::number(-1));
           price.push_back(row1);
           price.push_back(row2);

       }


        add_price* a = new add_price(price);
        a-> show();
        connect(a,SIGNAL(sendprice(std::vector<std::vector<QString>>)),this,SLOT(my_price_get(std::vector<std::vector<QString>>)));
}
