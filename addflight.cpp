#include "addflight.h"
#include "ui_addflight.h"
extern QSqlDatabase db;
extern mainplatformwindow *w;
extern QTableView *usertable;
addflight::addflight(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addflight)
{
    ui->setupUi(this);
    airport = new QStandardItemModel(0,5,this);
    theselection = new QItemSelectionModel(airport);
    //connect(theselection,SIGNAL(currentChanged(QModelIndex,QModelIndex)),this,SLOT(on_currentChanged(QModelIndex,QModelIndex)));
    ui->tableView_airport->setModel(airport);
    ui->tableView_airport->setSelectionModel(theselection);
    ui->tableView_airport->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableView_airport->setSelectionBehavior(QAbstractItemView::SelectItems);
    QStringList headerList ={tr("airport_id"),tr("arrival_time"),tr("departure_time"),tr("upward"),tr("downward")};
    airport->setHorizontalHeaderLabels(headerList);
    ui->tableView_airport->resizeColumnsToContents();
    departure=false;
    arrival=false;

}

addflight::~addflight()
{
    delete ui;
}
/*
void addflight::on_actAppend_triggered(){
    //在表格最后添加行
        QList<QStandardItem*>    aItemList; //容器类
        QStandardItem   *aItem;
        for(int i=0;i<3;i++) //不包含最后1列
        {
            aItem=new QStandardItem("0"); //创建Item
            aItemList<<aItem;   //添加到容器
        }
        //获取最后一列的表头文字
        QString str=airport->headerData(airport->columnCount()-1,Qt::Horizontal,Qt::DisplayRole).toString();
        aItem=new QStandardItem(str); //创建 Item
        aItem->setCheckable(true);
        aItemList<<aItem;   //添加到容器
        airport->insertRow(airport->rowCount(),aItemList); //插入一行，需要每个Cell的Item
        QModelIndex curIndex=airport->index(airport->rowCount()-1,0);//创建最后一行的ModelIndex
        theselection->clearSelection();//清空选择项
        theselection->setCurrentIndex(curIndex,QItemSelectionModel::Select);//设置刚插入的行为当前选择行
}

void addflight::on_actDelete_triggered()
{ //删除行
    QModelIndex curIndex=theselection->currentIndex () ;//获取模型索引
    if (curIndex. row () ==airport->rowCount () -1) //最后一行
        airport->removeRow (curIndex.row () ) ; //删除最后一行
    else {
        airport->removeRow (curIndex.row () );//删除一行，并重新设置当前选择行
        theselection->setCurrentIndex (curIndex, QItemSelectionModel::Select);
    }
}
*/
void addflight::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->button(QDialogButtonBox::Ok) == button)
    {


        my_fli tran;
        tran.flight_id = ui->flight_id->text();
        tran.schedule = ui->schedule->text();
        tran.plane_type = ui->planetype->text();
        tran.company_id = ui->company_id->text();

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
            sql1 = QString("INSERT INTO flight (flight_id,schedule,plane_type,company_id)"
                      "VALUES('%1','%2','%3','%4')")
                .arg(tran.flight_id).arg(tran.schedule).arg(tran.plane_type).arg(tran.company_id);
            if(type == true){
                    sql2 = QString("INSERT INTO seat (flight_id,type,`business_No.`,`economy_No.`)"
                                  "VALUES('%1',%2,%3,%4)")//SET TYPE = 1 IF DOUBLE AISLE
                            .arg(tran.flight_id).arg(1).arg(business).arg(economy);
                }
            else{
                sql2 = QString("INSERT INTO seat (flight_id,type,`business_No.`,`economy_No.`)"
                              "VALUES('%1',%2,%3,%4)")//SET TYPE = 1 IF DOUBLE AISLE
                        .arg(tran.flight_id).arg(0).arg(business).arg(economy);
            }
            query.exec(sql1);
            query.exec(sql2);
            for(int i=0;i<price.size();i++){
                if(price[i][2]=="business"){//if business, set class=1
                    sql4 = QString("INSERT INTO price (flight_id,start_id,end_id,class,price)"
                              "VALUES('%1',%2,%3,%4,%5)")
                        .arg(tran.flight_id).arg(price[i][4]).arg(price[i][5]).arg(1).arg(price[i][3]);
                }
                else{
                    sql4 = QString("INSERT INTO price (flight_id,start_id,end_id,class,price)"
                              "VALUES('%1',%2,%3,%4,%5)")
                        .arg(tran.flight_id).arg(price[i][4]).arg(price[i][5]).arg(0).arg(price[i][3]);
                }
                query.exec(sql4);
            }


            int count = ui->tableView_airport->model()->rowCount();
            if(!(departure&&arrival)){
                if(!QSqlDatabase::database().rollback()){
                    qDebug()<<QSqlDatabase::database().lastError();
                }
                QMessageBox::warning(this,tr("Failure"),tr("error:%1").arg(QSqlDatabase::database().lastError().text()));
                return;
            }
            for(int i =0;i<count;i++){
                QModelIndex index0 = ui->tableView_airport->model()->index(i,0);
                QModelIndex index1 = ui->tableView_airport->model()->index(i,1);
                QModelIndex index2 = ui->tableView_airport->model()->index(i,2);
                QString airport_id = ui->tableView_airport->model()->data(index0).toString();
                QString arrival_time = ui->tableView_airport->model()->data(index1).toString();
                QString departure_time = ui->tableView_airport->model()->data(index2).toString();

                if(arrival_time==""){
                    sql3 = QString("INSERT INTO airline (flight_id,airport_id,departure_time,`order`)"
                                  "VALUES('%1','%2','%3')")
                            .arg(tran.flight_id).arg(airport_id).arg(departure_time).arg(i);
                }
                else if(departure_time==""){
                    sql3 = QString("INSERT INTO airline (flight_id,airport_id,arrival_time,`order`)"
                                  "VALUES('%1','%2','%3')")
                            .arg(tran.flight_id).arg(airport_id).arg(arrival_time).arg(-1);
                }
                else
                    sql3 = QString("INSERT INTO airline (flight_id,airport_id,arrival_time,departure_time,`order`)"
                              "VALUES('%1','%2','%3','%4')")
                        .arg(tran.flight_id).arg(airport_id).arg(arrival_time).arg(departure_time).arg(i);
                query.exec(sql3);
            }

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
                    QMessageBox::warning(this,tr("Failure"),tr("error:%1").arg(QSqlDatabase::database().lastError().text()));
                }
             }
            else{
                QMessageBox::information(this,tr("hint:"),tr("add successfully"));
                this->close();
            }
        }
//        if(ok1){
//            bool ok2 = sql_query2(tran.flight_id);
//            if(ok2){
//                bool ok_seat = sql_query3(tran.flight_id);
//                if(ok_seat){
//                    QMessageBox::information(this,tr("hint:"),tr("add successfully"));
//                    w->flightRefresh();
//                    this->close();
//                }
//                else{
//                    QString sql2;
//                    sql2 = QString("delete from flight where flight_id ='%1'")
//                            .arg(tran.flight_id);
//                    QSqlQuery query2;
//                    bool ok3 = query2.exec(sql2);
//                    sql2 = QString("delete from airline where flight_id ='%1'")
//                            .arg(tran.flight_id);
//                    bool ok4 = query2.exec(sql2);
//                    sql2 = QString("delete from seat where flight_id ='%1'")
//                            .arg(tran.flight_id);
//                    bool ok5 = query2.exec(sql2);
//                    if(!ok3||!ok4||!ok5){
//                        QMessageBox::information(this,tr("Detrimental false"),tr("check the flight table in databse"));
//                        this->close();
//                    }
//                    else{
//                        QMessageBox::information(this,tr("hint:"),tr("failure"));
//                        this->close();
//                    }

//                }

//            }
//            else{
//                QString sql2;
//                sql2 = QString("delete from flight where flight_id ='%1'")
//                        .arg(tran.flight_id);
//                QSqlQuery query2;
//                bool ok3 = query2.exec(sql2);
//                sql2 = QString("delete from airline where flight_id ='%1'")
//                        .arg(tran.flight_id);
//                bool ok4 = query2.exec(sql2);
//                if(!ok3||!ok4){
//                    QMessageBox::information(this,tr("Detrimental false"),tr("check the flight table in databse"));
//                    this->close();
//                }
//                else{
//                    QMessageBox::information(this,tr("hint:"),tr("failure"));
//                    this->close();
//                }
//            }
//        }
//        else{
//            QMessageBox::information(this,tr("hint:"),tr("failure"));
//            this->close();
//        }
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
//bool addflight::sql_query2(QString flight_id){
//    int count = ui->tableView_airport->model()->rowCount();
//    int NULL1 = 0;
//    int NULL2 = 0;
//    for(int i =0;i<count;i++){
//        QModelIndex index0 = ui->tableView_airport->model()->index(i,0);
//        QModelIndex index1 = ui->tableView_airport->model()->index(i,1);
//        QModelIndex index2 = ui->tableView_airport->model()->index(i,2);
//        QString airport_id = ui->tableView_airport->model()->data(index0).toString();
//        QString arrival_time = ui->tableView_airport->model()->data(index1).toString();
//        QString departure_time = ui->tableView_airport->model()->data(index2).toString();

//        QString sql;
//        if(arrival_time==""){
//            sql = QString("INSERT INTO airline (flight_id,airport_id,departure_time)"
//                          "VALUES('%1','%2','%3')")
//                    .arg(flight_id).arg(airport_id).arg(departure_time);
//            NULL1 ++;
//        }
//        else if(departure_time==""){
//            sql = QString("INSERT INTO airline (flight_id,airport_id,arrival_time)"
//                          "VALUES('%1','%2','%3')")
//                    .arg(flight_id).arg(airport_id).arg(arrival_time);
//            NULL2 ++;
//        }
//        else
//            sql = QString("INSERT INTO airline (flight_id,airport_id,arrival_time,departure_time)"
//                      "VALUES('%1','%2','%3','%4')")
//                .arg(flight_id).arg(airport_id).arg(arrival_time).arg(departure_time);
//        QSqlQuery query;
//        bool ok = query.exec(sql);
//        if(!ok)
//            return false;
//    }
//    if(NULL1==1&&NULL2==1)
//        return true;
//    else
//        return false;
//}

void addflight::my_pass_get(my_pass tran){
    if(tran.order==0){
        if(departure==true){
            QMessageBox::information(this,tr("failure:"),tr("The departure has already existed"));
        }
        else{
            int count = 0;
            ui->tableView_airport->model()->insertRow(count);
            ui->tableView_airport->model()->setData(ui->tableView_airport->model()->index(count,0),tran.airport_id);
            ui->tableView_airport->model()->setData(ui->tableView_airport->model()->index(count,1),tran.arrival_time);
            ui->tableView_airport->model()->setData(ui->tableView_airport->model()->index(count,2),tran.departure_time);
            ui->tableView_airport->model()->setData(ui->tableView_airport->model()->index(count,3),"upward");
            ui->tableView_airport->model()->setData(ui->tableView_airport->model()->index(count,4),"downward");
            departure = true;
        }
    }
    else if(tran.order==-1){
        if(arrival==true){
            QMessageBox::information(this,tr("failure:"),tr("The arrival has already existed"));
        }
        else{
            int count = ui->tableView_airport->model()->rowCount();
            ui->tableView_airport->model()->insertRow(count);
            ui->tableView_airport->model()->setData(ui->tableView_airport->model()->index(count,0),tran.airport_id);
            ui->tableView_airport->model()->setData(ui->tableView_airport->model()->index(count,1),tran.arrival_time);
            ui->tableView_airport->model()->setData(ui->tableView_airport->model()->index(count,2),tran.departure_time);
            ui->tableView_airport->model()->setData(ui->tableView_airport->model()->index(count,3),"upward");
            ui->tableView_airport->model()->setData(ui->tableView_airport->model()->index(count,4),"downward");
            arrival=true;
        }
    }
    else{
        int count;
        if(arrival==true)
            count = ui->tableView_airport->model()->rowCount()-1;
        else
            count = ui->tableView_airport->model()->rowCount();
        ui->tableView_airport->model()->insertRow(count);
        ui->tableView_airport->model()->setData(ui->tableView_airport->model()->index(count,0),tran.airport_id);
        ui->tableView_airport->model()->setData(ui->tableView_airport->model()->index(count,1),tran.arrival_time);
        ui->tableView_airport->model()->setData(ui->tableView_airport->model()->index(count,2),tran.departure_time);
        ui->tableView_airport->model()->setData(ui->tableView_airport->model()->index(count,3),"upward");
        ui->tableView_airport->model()->setData(ui->tableView_airport->model()->index(count,4),"downward");
    }



}
//bool addflight::sql_query3(QString flight_id){
//    QString sql;
//    if(type == true){
//            sql = QString("INSERT INTO seat (flight_id,type,`business_No.`,`economy_No.`)"
//                          "VALUES('%1',%2,%3,%4)")//SET TYPE = 1 IF DOUBLE AISLE
//                    .arg(flight_id).arg(1).arg(business).arg(economy);
//        }
//    else{
//        sql = QString("INSERT INTO seat (flight_id,type,`business_No.`,`economy_No.`)"
//                      "VALUES('%1',%2,%3,%4)")//SET TYPE = 1 IF DOUBLE AISLE
//                .arg(flight_id).arg(0).arg(business).arg(economy);
//    }
//    QSqlQuery query;
//    bool ok = query.exec(sql);
//    return ok;
//}
void addflight::my_seat_get(my_seat tran){
    type = tran.type;
    business = tran.business;
    economy = tran.economy;
}

void addflight::on_ADD_clicked()
{
    addflight_passingairport* a = new addflight_passingairport();
    a-> show();
    connect(a,SIGNAL(sendPass(my_pass)),this,SLOT(my_pass_get(my_pass)));
}

void addflight::on_pushButton_2_clicked()
{
    int curRow = ui->tableView_airport->currentIndex().row();
    QModelIndex index1 = ui->tableView_airport->model()->index(curRow,1);
    QModelIndex index2 = ui->tableView_airport->model()->index(curRow,2);
    QString temp1=ui->tableView_airport->model()->data(index1).toString();
    QString temp2=ui->tableView_airport->model()->data(index2).toString();
    int ok = QMessageBox::warning(this,tr("Delete the current row!"),tr("Are you sure?"),QMessageBox::Yes,QMessageBox::No);
    if(ok==QMessageBox::No){
        return;
    }
    else{
        if(temp1==NULL) departure=false;
        if(temp2==NULL) arrival = false;
        ui->tableView_airport->model()->removeRow(curRow);
    }
}

void addflight::on_seat_clicked()
{
    addseat *a = new addseat;
    a->show();
    connect(a,SIGNAL(sendseat(my_seat)),this,SLOT(my_seat_get(my_seat)));
}

void addflight::on_tableView_airport_clicked(const QModelIndex &index)
{
    if (index.isValid()&&index.column()==3){//upward
        if(index.row()==0){
            QMessageBox::information(this,tr("failure:"),tr("The current row has already be the first one"));
        }
        else if(index.row()==ui->tableView_airport->model()->rowCount()-1){
            QMessageBox::information(this,tr("failure:"),tr("The arrival should always be the last one"));
        }
        else if(index.row()==1){
            QMessageBox::information(this,tr("failure:"),tr("The departure should always be the first one"));
        }
        else{
            for(int i=0;i<3;i++){
                QModelIndex index0 = ui->tableView_airport->model()->index(index.row()-1,i);
                QModelIndex index1 = ui->tableView_airport->model()->index(index.row(),i);
                QString temp0=ui->tableView_airport->model()->data(index0).toString();
                QString temp1=ui->tableView_airport->model()->data(index1).toString();
                ui->tableView_airport->model()->setData(index0,temp1);
                ui->tableView_airport->model()->setData(index1,temp0);
            }
        }
    }
    else if(index.isValid()&&index.column()==4){//downward
        if(index.row()==ui->tableView_airport->model()->rowCount()-1){
            QMessageBox::information(this,tr("failure:"),tr("The current row has already be the last one"));
        }
        else if(index.row()==0){
            QMessageBox::information(this,tr("failure:"),tr("The departure should always be the first one"));
        }
        else if(index.row()==ui->tableView_airport->model()->rowCount()-2){
            QMessageBox::information(this,tr("failure:"),tr("The arrival should always be the last one"));
        }
        else{
            for(int i=0;i<3;i++){
                QModelIndex index0 = ui->tableView_airport->model()->index(index.row()+1,i);
                QModelIndex index1 = ui->tableView_airport->model()->index(index.row(),i);
                QString temp0=ui->tableView_airport->model()->data(index0).toString();
                QString temp1=ui->tableView_airport->model()->data(index1).toString();
                ui->tableView_airport->model()->setData(index0,temp1);
                ui->tableView_airport->model()->setData(index1,temp0);
            }
        }
    }
}
void addflight::my_price_get(std::vector<std::vector<QString>> whole_price){
   for(int i=0;i<price.size();i++){
       price[i][3]=whole_price[i][3];
   }
}

void addflight::on_price_clicked()
{
        price.clear();
        int count=ui->tableView_airport->model()->rowCount();
        for(int i=0;i<count;i++){
            for(int j=i+1;j<count;j++){
                std::vector<QString> row1;
                std::vector<QString> row2;
                QModelIndex index = ui->tableView_airport->model()->index(i,0);
                QString temp=ui->tableView_airport->model()->data(index).toString();
                row1.push_back(temp);
                row2.push_back(temp);
                index = ui->tableView_airport->model()->index(j,0);
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


//    QStringList price_list;
//    int count=ui->tableView_airport->model()->rowCount();
//    for(int i=0;i<count;i++){
//        QModelIndex index = ui->tableView_airport->model()->index(i,0);
//        QString temp=ui->tableView_airport->model()->data(index).toString();
//        price_list.push_back(temp);
//    }


    add_price* a = new add_price(price);
    //add_price* a = new add_price(price_list);
    a-> show();
    connect(a,SIGNAL(sendprice(std::vector<std::vector<QString>>)),this,SLOT(my_price_get(std::vector<std::vector<QString>>)));

}
