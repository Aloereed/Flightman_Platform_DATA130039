#include "mainplatformwindow.h"
#include "ui_mainplatformwindow.h"
#include "loginwindow.h"
#include "adduser.h"
#include "moduser.h"
#include "addcom.h"
#include "modcom.h"
#include "addflight.h"
#include "addairport.h"
#include "modairport.h"
#include "stopover.h"
#include "modflight.h"
#include "querydialog.h"
#include "genarran.h"
#include "addfliarrange.h"
#include "addadmin.h"
#include "modadmin.h"
#include "show_seat_a.h"
#include "QRibbon/QRibbon.h"
#include<QCheckBox>
#include<QToolTip>
#include <QSysInfo>
extern QSqlDatabase db;
extern QByteArray readTextFile(const QString&);
extern mainplatformwindow *w;
QueryDialog *q;
loginwindow *l;
extern QTranslator translator;
extern my_admin tranadmin;
extern QSettings settings;

stopover *stop_over;
adduser *a;
addflight *add_flight;
addairport *add_airport;
addfliarrange *add_fliarrange;
addadmin *add_admin;
show_seat_a *show_seat;

modadmin *modification_admin;
moduser *modification_user;
modcom *modification_comp;
modflight *modification_flight;
modairport *ma;
addcom *add_comp;
QTableView *usertable;
QTableView *flighttable;
QTableView *comptable;
QTimer *localtimer;
QSignalMapper *usermapper;
QSignalMapper *usermapper_mod;
QSignalMapper *compmapper;
QSignalMapper *compmapper_mod;
const QString osVersion()
{
    return QSysInfo::prettyProductName()+" "+QSysInfo::buildAbi()+", with kernel: "+QSysInfo::kernelType()+" "+QSysInfo::kernelVersion();
}

mainplatformwindow::mainplatformwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mainplatformwindow)
{
    ui->setupUi(this);
    QRibbon::install(this);
    ui->comboBox_2->setCurrentIndex(settings.value("themeno",1).toInt());

    ui->comboBox->setCurrentIndex(settings.value("Langcase",2).toInt());
    _init();
}

void mainplatformwindow::_init(){
    //settings.sync();

    ui->label->setText(tr("System Version:")+osVersion());

    QSqlQuery query = QSqlQuery("select @@VERSION");

    query.next();

    ui->label_2->setText(QString(tr("Server Version:\n "))+query.value(0).toString());

    query=QSqlQuery("select user()");

    query.next();

    ui->label_3->setText(QString(tr("Login system user: "))+query.value(0).toString());

    ui->label_4->setText(QString(tr("Language: "))+QLocale::system().name());

    QSqlQueryModel *infomodel = new QSqlQueryModel;

    infomodel->setQuery(QString("show status"));

    ui->tableView_2->setModel(infomodel);

    ui->tableView_2->resizeColumnsToContents();


    usertable=ui->tableView;

    flighttable=ui->tableView_3;

    comptable=ui->tableView_4;

    userRefresh();

    flightRefresh();

    fliarrangeRefresh();

    //compRefresh();

    localtimer = new QTimer(this);

    connect(localtimer, SIGNAL(timeout()), this, SLOT(updatetime()));

    localtimer->start();

    adminRefresh();
    on_horizontalSlider_2_valueChanged(1);
    on_horizontalSlider_valueChanged(1);
    ui->spinBox->setValue(settings.value("itemsperpage",20).toInt());
    qApp->setStyleSheet(readTextFile(settings.value("theme",":/qss/Aqua.qss").toString()));

}


void mainplatformwindow::updatetime(){
    ui->statusBar->showMessage(tr("Welcome, ")+tranadmin.name+"! "+QDateTime::currentDateTime().toString());
}


mainplatformwindow::~mainplatformwindow()
{
    delete ui;
    //settings.sync();
}



void mainplatformwindow::on_listWidget_clicked(const QModelIndex &index)
{

}




void mainplatformwindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(NULL);
}
void mainplatformwindow::on_actionLog_out_triggered(){
    db.close();
    this->close();
    l=new loginwindow;
    l->setWindowIcon(QIcon(":/icon.png"));
    l->show();
}
/*
void mainplatformwindow::on_listWidget_user_clicked(const QModelIndex &index)
{
    int currentrow = ui -> listWidget_4 -> currentRow();
    if(currentrow == 2){
        QVariantMap userinfo;

        QString ID = ui -> IDEdit -> text();
        QString name = ui -> nameEdit => text();
        Qstring membership = ui ->
    }
}
void mainplatformwindow::ExecAdd(QVariantMap userinfo){
    QString ID = userinfo.value("ID").toString();
    QString name = userinfo.value("name").toString();
    QString membership = userinfo.value("membership").toString();
    QString account = userinfo.value("account").toString();

    QString sql;
    sql = QString("INSERT INTO user (ID,name,membership,account)"
                  "VALUES('%1','%2','%3',''%4)")
            .arg(ID).arg(name).arg(membership).arg(account);

    QSqlQuery query;
    bool ok = query.exec(sql);
    if(ok){
        QMessageBox::information(this,tr("hint:"),tr("add successfully"));
    }
    else{
        QMessageBox::information(this,tr("hint:"),tr("failure"));
    }

}*/

void mainplatformwindow::on_listWidget_user_currentRowChanged(int currentRow)
{

}
void mainplatformwindow::fliarrangeRefresh(){
    myfliarrangemodel *fliarrangemodel = new myfliarrangemodel;
    fliarrangemodel->setQuery("select * from flight_arrangement");
    ui->tableView_7->setModel(fliarrangemodel);

    fliarrangemodel->insertColumn(4);
    fliarrangemodel->setHeaderData(4,Qt::Horizontal,QString::fromUtf8(tr("Seat amount").toUtf8()));
    fliarrangemodel->insertColumn(5);
    fliarrangemodel->setHeaderData(5,Qt::Horizontal,QString::fromUtf8(tr("Seat arrangement").toUtf8()));
    fliarrangemodel->insertColumn(6);
    fliarrangemodel->setHeaderData(6,Qt::Horizontal,QString::fromUtf8(tr("Modify").toUtf8()));
    fliarrangemodel->insertColumn(7);
    fliarrangemodel->setHeaderData(7,Qt::Horizontal,QString::fromUtf8(tr("Delete").toUtf8()));
    fliarrangemodel->setHeaderData(0,Qt::Horizontal,QString::fromUtf8(tr("Departure Date").toUtf8()));
    fliarrangemodel->setHeaderData(1,Qt::Horizontal,QString::fromUtf8(tr("Flight ID").toUtf8()));
    fliarrangemodel->setHeaderData(2,Qt::Horizontal,QString::fromUtf8(tr("Status").toUtf8()));
    fliarrangemodel->setHeaderData(3,Qt::Horizontal,QString::fromUtf8(tr("Discount").toUtf8()));
    ui->tableView_7->resizeColumnsToContents();

}
void mainplatformwindow::airportRefresh(int page){
    QSqlQuery query = QSqlQuery("select count(1) from airport");
    ui->statusBar->showMessage(tr("Querying..."));
    int itemsperpage=settings.value("itemsperpage",20).toInt();
    query.next();
    ui->horizontalSlider_2->setMaximum(query.value(0).toInt()/itemsperpage+1);
    int item2 = itemsperpage*(page-1);
    mycompmodel *apmodel = new mycompmodel;
    apmodel->setQuery("select * from airport limit "+QString::number(item2)+","+QString::number(itemsperpage));
    ui->tableView_5->setModel(apmodel);
    apmodel->insertColumn(3);//这里是在模型的第4列后面插入一列
    apmodel->setHeaderData(3,Qt::Horizontal,QString::fromUtf8(tr("Modify").toUtf8()));
    apmodel->insertColumn(4);
    apmodel->setHeaderData(4,Qt::Horizontal,QString::fromUtf8(tr("Delete").toUtf8()));
    apmodel->setHeaderData(0,Qt::Horizontal,QString::fromUtf8(tr("Airport IATA Code").toUtf8()));
    apmodel->setHeaderData(1,Qt::Horizontal,QString::fromUtf8(tr("Airport Name").toUtf8()));
    apmodel->setHeaderData(2,Qt::Horizontal,QString::fromUtf8(tr("Airport City").toUtf8()));
    ui->tableView_5->resizeColumnsToContents();
}
void mainplatformwindow::adminRefresh(){
    if(tranadmin.satype){
        myadminmodel *admodel = new myadminmodel;
        admodel->setQuery("select adminID,adminName,satype from admin");
        ui->tableView_6->setModel(admodel);
        admodel->insertColumn(3);//这里是在模型的第4列后面插入一列
        admodel->setHeaderData(3,Qt::Horizontal,QString::fromUtf8(tr("Modify").toUtf8()));
        admodel->insertColumn(4);
        admodel->setHeaderData(4,Qt::Horizontal,QString::fromUtf8(tr("Delete").toUtf8()));
        admodel->setHeaderData(0,Qt::Horizontal,QString::fromUtf8(tr("Admin ID").toUtf8()));
        admodel->setHeaderData(1,Qt::Horizontal,QString::fromUtf8(tr("Admin Name").toUtf8()));
        admodel->setHeaderData(2,Qt::Horizontal,QString::fromUtf8(tr("SA Type").toUtf8()));
        ui->tableView_6->resizeColumnsToContents();
    }else
        ui->label_5->setText("Only For Super Administrator, but you are not a SA.");
}
void mainplatformwindow::flightRefresh(){
    myflightmodel *flightmodel = new myflightmodel;
    flightmodel->setQuery("select flight_id,schedule,plane_type,depap_id,departure_time,arrap_id,arrival_time,company_id from flight a inner join (select dep.flight_id flight_id,dep.airport_id depap_id,dep.departure_time,arr.airport_id arrap_id,arr.arrival_time \
                          From (select * from airline where arrival_time is null)  dep,(select * from airline where departure_time is null) arr \
                          where dep.flight_id=arr.flight_id) b using (flight_id)");
    flighttable->setModel(flightmodel);
    flightmodel->insertColumn(8);
    flightmodel->setHeaderData(8,Qt::Horizontal,QString::fromUtf8(tr("Stopover").toUtf8()));
    flightmodel->insertColumn(9);//这里是在模型的第4列后面插入一列
    flightmodel->setHeaderData(9,Qt::Horizontal,QString::fromUtf8(tr("Modify").toUtf8()));
    flightmodel->insertColumn(10);
    flightmodel->setHeaderData(10,Qt::Horizontal,QString::fromUtf8(tr("Delete").toUtf8()));
    flightmodel->setHeaderData(0,Qt::Horizontal,QString::fromUtf8(tr("Flight ID").toUtf8()));
    flightmodel->setHeaderData(1,Qt::Horizontal,QString::fromUtf8(tr("Schedule").toUtf8()));
    flightmodel->setHeaderData(2,Qt::Horizontal,QString::fromUtf8(tr("Plane Type").toUtf8()));
    flightmodel->setHeaderData(3,Qt::Horizontal,QString::fromUtf8(tr("Depature Airport ID").toUtf8()));
    flightmodel->setHeaderData(4,Qt::Horizontal,QString::fromUtf8(tr("Depature Time").toUtf8()));
    flightmodel->setHeaderData(5,Qt::Horizontal,QString::fromUtf8(tr("Arrival Airport ID").toUtf8()));
    flightmodel->setHeaderData(6,Qt::Horizontal,QString::fromUtf8(tr("Arrival Time").toUtf8()));
    flightmodel->setHeaderData(7,Qt::Horizontal,QString::fromUtf8(tr("Airline Company ID").toUtf8()));
    flighttable->resizeColumnsToContents();
}
void mainplatformwindow::compRefresh(int page){
    QSqlQuery query = QSqlQuery("select count(1) from company");
    ui->statusBar->showMessage(tr("Querying..."));
    int itemsperpage=settings.value("itemsperpage",20).toInt();
    query.next();
    ui->horizontalSlider->setMaximum(query.value(0).toInt()/itemsperpage+1);
    //compmapper= new QSignalMapper;
    //compmapper_mod = new QSignalMapper;
    mycompmodel *compmodel = new mycompmodel;
    int item2 = itemsperpage*(page-1);
    compmodel->setQuery("select * from company limit "+QString::number(item2)+","+QString::number(itemsperpage));
    comptable->setModel(compmodel);

    compmodel->insertColumn(3);//这里是在模型的第4列后面插入一列
    compmodel->setHeaderData(3,Qt::Horizontal,QString::fromUtf8(tr("Modify").toUtf8()));
    compmodel->insertColumn(4);
    compmodel->setHeaderData(4,Qt::Horizontal,QString::fromUtf8(tr("Delete").toUtf8()));
    compmodel->setHeaderData(0,Qt::Horizontal,QString::fromUtf8(tr("Airline Company Code").toUtf8()));
    compmodel->setHeaderData(1,Qt::Horizontal,QString::fromUtf8(tr("Airline Company Name").toUtf8()));
    compmodel->setHeaderData(2,Qt::Horizontal,QString::fromUtf8(tr("Account").toUtf8()));
    //QPushButton *button[compmodel->rowCount()];
    //QPushButton *button2[compmodel->rowCount()] ;
    comptable->resizeColumnsToContents();
    ui->statusBar->showMessage(tr("Completing..."));
    /*for(int i=0; i<compmodel->rowCount();i++)
     {
        // set custom property
        button[i]->setProperty("id",compmodel->data(compmodel->index(i,0)).toString()); // set custom property
        button2[i]->setProperty("id", compmodel->data(compmodel->index(i,0)).toString()); // set custom property
        // set click event
        QApplication::processEvents();
        //connect(button[i], SIGNAL(clicked()), this, SLOT(onTableBtnClicked()));
        connect(button[i],SIGNAL(clicked()),compmapper_mod,SLOT(map()));
        connect(button2[i], SIGNAL(clicked()), compmapper, SLOT(map()));
        compmapper->setMapping(button2[i], button2[i]->property("id").toString());
        compmapper_mod->setMapping(button[i], button[i]->property("id").toString());
     }
    connect(compmapper, SIGNAL(mapped(QString)), this, SLOT(onCompTableDelBtnClicked(QString)));
    connect(compmapper_mod, SIGNAL(mapped(QString)), this, SLOT(onCompTableModBtnClicked(QString)));*/
    ui->statusBar->showMessage(tr("Completed!"));
}
void mainplatformwindow::userRefresh(){
    //usermapper= new QSignalMapper;
    //usermapper_mod=new QSignalMapper;
    myusermodel *usermodel = new myusermodel;
    usermodel->setQuery("select ID,name,membership,account from user");
    usertable->setModel(usermodel);
    usermodel->insertColumn(4);//这里是在模型的第4列后面插入一列
    usermodel->setHeaderData(4,Qt::Horizontal,QString::fromUtf8(tr("Modify").toUtf8()));
    usermodel->insertColumn(5);
    usermodel->setHeaderData(5,Qt::Horizontal,QString::fromUtf8(tr("Delete").toUtf8()));
    usermodel->setHeaderData(0,Qt::Horizontal,QString::fromUtf8(tr("ID").toUtf8()));
    usermodel->setHeaderData(1,Qt::Horizontal,QString::fromUtf8(tr("Name").toUtf8()));
    usermodel->setHeaderData(2,Qt::Horizontal,QString::fromUtf8(tr("Membership").toUtf8()));
    usermodel->setHeaderData(3,Qt::Horizontal,QString::fromUtf8(tr("Account").toUtf8()));
    //QPushButton *button[usermodel->rowCount()];
    //QPushButton *button2[usermodel->rowCount()] ;
    /*
    for(int i=0; i<usermodel->rowCount();i++)
     {
        // add button to the last column
        QCheckBox *membershipCheck = new QCheckBox(this);
        if(usermodel->data(usermodel->index(i,2)).toInt()==1){
            membershipCheck->setChecked(true);
        }
        //button[i] = new QPushButton(tr("Modify"));
        //button2[i] = new QPushButton(tr("Delete"));

        ui->tableView->setIndexWidget(usermodel->index(i, 2), membershipCheck);
        //ui->tableView->setIndexWidget(usermodel->index(i, 4), button[i]);
        //ui->tableView->setIndexWidget(usermodel->index(i, 5), button2[i]);
        membershipCheck->setEnabled(false);
     }*/
    usertable->resizeColumnsToContents();
    ui->statusBar->showMessage(tr("Querying..."));
    for(int i=0; i<usermodel->rowCount();i++)
     {

        // set custom property
        //button[i]->setProperty("id", usermodel->data(usermodel->index(i,0)).toString()); // set custom property
        //button[i]->setProperty("remark", tr("Modify %1-th.").arg(i));
        //button2[i]->setProperty("id", usermodel->data(usermodel->index(i,0)).toString()); // set custom property
        //button2[i]->setProperty("remark", tr("Delete %1-th.").arg(i));

        // set click event
        //connect(button, SIGNAL(clicked()), this, SLOT(onTableBtnClicked()));
        //connect(button[i],SIGNAL(clicked()),usermapper_mod,SLOT(map()));
        //connect(button2[i], SIGNAL(clicked()), usermapper, SLOT(map()));
        //usermapper_mod->setMapping(button[i],button[i]->property("id").toString());
        //usermapper->setMapping(button2[i], button2[i]->property("id").toString());
        // notice every time insert the button at the last line
     }

    //connect(usermapper, SIGNAL(mapped(QString)), this, SLOT(onTableDelBtnClicked(QString)));
    //connect(usermapper_mod, SIGNAL(mapped(QString)), this, SLOT(onTableModBtnClicked(QString)));
    ui->statusBar->showMessage(tr("Completed!"));
}
void mainplatformwindow::on_listWidget_user_itemClicked(QListWidgetItem *item)
{
    if(item->text()==tr("Add")){
        a=new adduser;
        a->show();
    }else if(item->text()==tr("Refresh")){
        userRefresh();
    }else if(item->text()==tr("Search")){
        q=new QueryDialog;
        q->show();
    }
}

void mainplatformwindow::on_tableView_3_clicked(const QModelIndex &index)
{
      if (index.isValid()&&index.column()==8){
          int row = index.row();
          QAbstractItemModel* model = ui->tableView_3->model();
          QString flight_id = model->data(model->index(row,0)).toString();
          stop_over = new stopover(nullptr,flight_id);
          stop_over->show();
      }
      else if(index.isValid()&&index.column()==10){
          int row = index.row();
          QAbstractItemModel* model = ui->tableView_3->model();
          QString flight_id = model->data(model->index(row,0)).toString();
          if(QSqlDatabase::database().transaction()){
              QSqlQuery query;
              query.exec(tr("delete from airline where flight_id = \'")+flight_id+"\'");
              query.exec(tr("delete from flight where flight_id = \'")+flight_id+"\'");
              query.exec(tr("delete from seat where flight_id = \'")+flight_id+"\'");
              if(!QSqlDatabase::database().commit()){
                  qDebug()<<QSqlDatabase::database().lastError();
                  if(!QSqlDatabase::database().rollback()){
                      qDebug()<<QSqlDatabase::database().lastError();
                  }

              }
              else{
                  flightRefresh();
              }
          }

      }
      else if(index.isValid()&&index.column()==9){
          int row = index.row();
          QAbstractItemModel* model = ui->tableView_3->model();
          QString flight_id = model->data(model->index(row,0)).toString();
          QString schedule = model->data(model->index(row,1)).toString();
          QString plane_type = model->data(model->index(row,2)).toString();
          QString company_id = model->data(model->index(row,7)).toString();
          modification_flight = new modflight(nullptr,flight_id,schedule,plane_type,company_id);
          modification_flight->show();
      }

}

void mainplatformwindow::on_listWidget_3_itemClicked(QListWidgetItem *item)
{
    if(item->text()==tr("Add")){
        add_flight=new addflight;
        add_flight->show();
    }else if(item->text()==tr("Refresh")){
        flightRefresh();
    }else if(item->text()==tr("Search")){
        q=new QueryDialog;
        q->show();
    }
}

/*
bool mainplatformwindow::onTableDelBtnClicked(QString id){
    QSqlQuery query;
    bool status = query.exec(tr("delete from user where ID = \'")+QString(id)+"\'");
    if(status)
        userRefresh();
    else
        QMessageBox::critical(this,tr("Delete failed."),tr("Delete failed."));
    return status;
}

bool mainplatformwindow::onTableModBtnClicked(QString id){
    QSqlQuery query;
    bool status = query.exec(QString("select * FROM user WHERE ID = \'")+QString(id)+"\'");
    query.next();
    if(status){
        QString ID = query.value(0).toString();
        QString name = query.value(1).toString();
        QString membership = query.value(2).toString();
        QString account = query.value(3).toString();
        modification_user = new moduser(nullptr,ID,name,membership,account);
        modification_user->show();
    }
    else
        QMessageBox::critical(this,tr("modification failed."),tr("modification failed."));
    return status;


}

bool mainplatformwindow::onCompTableDelBtnClicked(QString id){
    QSqlQuery query;
    bool status = query.exec(tr("delete from company where company_id = \'")+QString(id)+"\'");
    if(status)
        compRefresh();
    else
        QMessageBox::critical(this,tr("Delete failed."),tr("Delete failed."));
    return status;
}
bool mainplatformwindow::onCompTableModBtnClicked(QString id){
    QSqlQuery query;
    bool status = query.exec(QString("select * FROM company WHERE company_ID = \'")+QString(id)+"\'");
    query.next();
    if(status){
        QString ID = query.value(0).toString();
        QString name = query.value(1).toString();
        QString account = query.value(2).toString();
        modification_comp = new modcom(nullptr,ID,name,account);
        modification_comp->show();
    }
    else
        QMessageBox::critical(this,tr("modification failed."),tr("modification failed."));
    return status;
}
*/
void mainplatformwindow::on_listWidget_5_itemClicked(QListWidgetItem *item)
{
    if(item->text()==tr("Add")){
        add_comp=new addcom;
        add_comp->show();
    }else if(item->text()==tr("Refresh")){
        ui->horizontalSlider->setValue(1);
    }else if(item->text()==tr("Search")){
        QStringList sqllist = {"company_id","company_name","company_account"};
        QStringList indexlist = {"Code","Name","Account"};
        QString table="company";
        q=new QueryDialog(table,sqllist,indexlist);
        q->show();
    }
}


void mainplatformwindow::on_horizontalSlider_valueChanged(int value)
{
    ui->statusBar->showMessage(tr("Querying..."));
    ui->plainTextEdit->setText(QString::number(value));
    compRefresh(value);
}


void mainplatformwindow::on_tableView_clicked(const QModelIndex &index)
{
    if (index.isValid()&&index.column()==4) {//user_modification
        int row = index.row();
        QAbstractItemModel* model = ui->tableView->model();
        QString ID = model->data(model->index(row,0)).toString();
        QString name = model->data(model->index(row,1)).toString();
        QString membership = model->data(model->index(row,2)).toString();
        QString account = model->data(model->index(row,3)).toString();
        modification_user = new moduser(nullptr,ID,name,membership,account);
        modification_user->show();
    }
    else if(index.isValid()&&index.column()==5){//user_delete
        int row = index.row();
        QAbstractItemModel* model = ui->tableView->model();
        QString ID = model->data(model->index(row,0)).toString();
        QSqlQuery query;
        bool status = query.exec(tr("delete from user where ID = \'")+ID+"\'");
        if(status)
            userRefresh();
        else
            QMessageBox::critical(this,tr("Delete failed."),tr("Delete failed."));
    }
}

void mainplatformwindow::on_tableView_4_clicked(const QModelIndex &index)
{
    if (index.isValid()&&index.column()==3) {//company_modification
        int row = index.row();
        QAbstractItemModel* model = ui->tableView_4->model();
        QString ID = model->data(model->index(row,0)).toString();
        QString name = model->data(model->index(row,1)).toString();
        QString account = model->data(model->index(row,2)).toString();
        modification_comp = new modcom(nullptr,ID,name,account);
        modification_comp->show();
    }
    else if(index.isValid()&&index.column()==4){//company_delete
        int row = index.row();
        QAbstractItemModel* model = ui->tableView_4->model();
        QString ID = model->data(model->index(row,0)).toString();
        QSqlQuery query;
        bool status = query.exec(tr("delete from company where company_id = \'")+ID+"\'");
        if(status)
            compRefresh();
        else
            QMessageBox::critical(this,tr("Delete failed."),tr("Delete failed."));
    }
}

QVariant mycompmodel::data(const QModelIndex &item, int role) const{
    QVariant value = QSqlQueryModel::data(item, role);
    if (role == Qt::BackgroundColorRole){
        if(item.column()==3)
            return QVariant::fromValue(QColor(225,225,225));
        else if(item.column()==4)
            return QVariant::fromValue(QColor(225,225,225));//第一个属性的字体颜色为红色
    }
    if (role == Qt::DisplayRole){
        if(item.column()==3)
            return QVariant::fromValue(tr("Modify"));
        else if(item.column()==4)
            return QVariant::fromValue(tr("Delete"));
    }
    return value;
}
QVariant myflightmodel::data(const QModelIndex &item, int role) const{
    QVariant value = QSqlQueryModel::data(item, role);
    if (role == Qt::BackgroundColorRole){
        if(item.column()==8||item.column()==9||item.column()==10)
            return QVariant::fromValue(QColor(225,225,225));
    }
    if (role == Qt::DisplayRole){
        if(item.column()==9)
            return QVariant::fromValue(tr("Modify"));
        else if(item.column()==10)
            return QVariant::fromValue(tr("Delete"));
        else if(item.column()==8)
            return QVariant::fromValue(tr("Stopover"));
    }
    if(role == Qt::ToolTipRole){
        if (item.isValid()&&(item.column()==5||item.column()==3)) {
            QSqlQuery query = QSqlQuery(QString("select airport_name from airport where airport_id=\'")+item.data().toString()+"\'");
            query.next();
            return QVariant::fromValue(query.value(0).toString());
        }else if(item.isValid()&&(item.column()==7)){
            QSqlQuery query = QSqlQuery(QString("select company_name from company where company_id=\'")+item.data().toString()+"\'");
            query.next();
            return QVariant::fromValue(query.value(0).toString());
        }
    }
    return value;
}
QVariant myusermodel::data(const QModelIndex &item, int role) const{
    QVariant value = QSqlQueryModel::data(item, role);
    if (role == Qt::BackgroundColorRole){
        if(item.column()==4)
            return QVariant::fromValue(QColor(225,225,225));
        else if(item.column()==5)
            return QVariant::fromValue(QColor(225,225,225));//第一个属性的字体颜色为灰色
    }
    if (role == Qt::DisplayRole){
        if(item.column()==2){
            if(QSqlQueryModel::data(item).toInt()==0)
                return QVariant::fromValue(QString("×"));
            else
                return QVariant::fromValue(QString("√"));
        }
        if(item.column()==4)
            return QVariant::fromValue(tr("Modify"));
        else if(item.column()==5)
            return QVariant::fromValue(tr("Delete"));
    }
    return value;
}
QVariant myfliarrangemodel::data(const QModelIndex &item, int role) const{
    QVariant value = QSqlQueryModel::data(item, role);
    if (role == Qt::BackgroundColorRole){
        if(item.column()==4||item.column()==5||item.column()==6||item.column()==7)
            return QVariant::fromValue(QColor(225,225,225)); //第一个属性的字体颜色为灰色
    }
    if (role == Qt::DisplayRole){
        if(item.column()==4)
            return QVariant::fromValue(tr("Click to View"));
        else if(item.column()==5)
            return QVariant::fromValue(tr("     Click to View"));
        else if(item.column()==6)
            return QVariant::fromValue(tr("Modify"));
        else if(item.column()==7)
            return QVariant::fromValue(tr("Delete"));
    }
    return value;
}
QVariant myadminmodel::data(const QModelIndex &item, int role) const{
    QVariant value = QSqlQueryModel::data(item, role);
    if (role == Qt::BackgroundColorRole){
        if(item.column()==3)
            return QVariant::fromValue(QColor(225,225,225));
        else if(item.column()==4)
            return QVariant::fromValue(QColor(225,225,225));//第一个属性的字体颜色为灰色
    }
    if (role == Qt::DisplayRole){
        if(item.column()==2){
            if(QSqlQueryModel::data(item).toInt()==0)
                return QVariant::fromValue(QString("×"));
            else
                return QVariant::fromValue(QString("√"));
        }
        if(item.column()==3)
            return QVariant::fromValue(tr("Modify"));
        else if(item.column()==4)
            return QVariant::fromValue(tr("Delete"));
    }
    return value;
}

void mainplatformwindow::on_plainTextEdit_returnPressed()
{
    ui->horizontalSlider->setValue(ui->plainTextEdit->text().toInt());
}

void mainplatformwindow::on_pushButton_clicked()
{
    ui->horizontalSlider->setValue(ui->horizontalSlider->value()-1);
}

void mainplatformwindow::on_pushButton_2_clicked()
{
    ui->horizontalSlider->setValue(ui->horizontalSlider->value()+1);
}





void mainplatformwindow::on_horizontalSlider_2_valueChanged(int value)
{
    ui->statusBar->showMessage(tr("Querying..."));
    ui->plainTextEdit_2->setText(QString::number(value));
    airportRefresh(value);
}

void mainplatformwindow::on_pushButton_3_clicked()
{
        ui->horizontalSlider_2->setValue(ui->horizontalSlider_2->value()-1);
}

void mainplatformwindow::on_plainTextEdit_2_returnPressed()
{
        ui->horizontalSlider_2->setValue(ui->plainTextEdit_2->text().toInt());
}

void mainplatformwindow::on_pushButton_4_clicked()
{
       ui->horizontalSlider_2->setValue(ui->horizontalSlider_2->value()+1);
}

void mainplatformwindow::on_listWidget_6_itemClicked(QListWidgetItem *item)
{
    if(item->text()==tr("Add")){
        add_airport=new addairport;
        add_airport->show();
    }else if(item->text()==tr("Refresh")){
        ui->horizontalSlider->setValue(1);
    }else if(item->text()==tr("Search")){
        QStringList sqllist = {"airport_id","airport_name","city"};
        QStringList indexlist = {"ID","Name","City"};
        QString table="airport";
        q=new QueryDialog(table,sqllist,indexlist);
        q->show();
    }
}

void mainplatformwindow::on_tableView_5_clicked(const QModelIndex &index)
{
    if (index.isValid()&&index.column()==3) {//company_modification
        int row = index.row();
        QAbstractItemModel* model = ui->tableView_5->model();
        QString ID = model->data(model->index(row,0)).toString();
        QString name = model->data(model->index(row,1)).toString();
        QString account = model->data(model->index(row,2)).toString();
        ma = new modairport(nullptr,ID,name,account);
        ma->show();
    }
    else if(index.isValid()&&index.column()==4){//company_delete
        int row = index.row();
        QAbstractItemModel* model = ui->tableView_5->model();
        QString ID = model->data(model->index(row,0)).toString();
        QSqlQuery query;
        bool status = query.exec(tr("delete from airport where airport_id = \'")+ID+"\'");
        if(status)
            on_horizontalSlider_2_valueChanged(1);
        else
            QMessageBox::critical(this,tr("Delete failed."),tr("Delete failed."));
    }
}

void mainplatformwindow::on_comboBox_activated(int index)
{
    switch (index){
    case 0:{
        qApp->removeTranslator(&translator);

        ui->retranslateUi(this);
        settings.setValue("Langcase",0);
        break;
    }
    case 1:{
        qApp->removeTranslator(&translator);
        QString langdir=":/platform_zh_CN.qm";
        translator.load(langdir);
        qApp->installTranslator(&translator);

        ui->retranslateUi(this);
        settings.setValue("Langcase",1);
        break;
    }
    default :{
        qApp->removeTranslator(&translator);
        QString langdir=":/platform_"+QLocale::system().name()+".qm";
        translator.load(langdir);
        qApp->installTranslator(&translator);

        ui->retranslateUi(this);
        settings.setValue("Langcase",2);
    }
    }
    //settings.sync();
    _init();
}

void mainplatformwindow::on_pushButton_5_clicked()
{
    int week = ui->lineEdit->text().toInt();
    GenArran gen(week,this);
    gen.run();
    gen.quit();
}

void mainplatformwindow::on_pushButton_6_clicked()
{
    DropArran drop(this);
    drop.run();
}

void mainplatformwindow::on_listWidget_7_itemClicked(QListWidgetItem *item)
{
    if(item->text()==tr("Add")){
        add_fliarrange=new addfliarrange;
        add_fliarrange->show();
    }else if(item->text()==tr("Refresh")){


    }else if(item->text()==tr("Search")){

    }

}

void mainplatformwindow::on_spinBox_valueChanged(int arg1)
{
    settings.setValue("itemsperpage",arg1);
    //settings.sync();
    flightRefresh();
    airportRefresh();
}
void mainplatformwindow::on_actionE_xit_triggered(){
    qApp->quit();
}

void mainplatformwindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    if(item->text()==tr("Add")&&tranadmin.satype){
        add_admin = new addadmin;
        add_admin->show();
    }else if(item->text()==tr("Refresh")){
        adminRefresh();
    }
}

void mainplatformwindow::on_tableView_6_clicked(const QModelIndex &index)
{
    if (index.isValid()&&index.column()==3) {//admin modification
        int row = index.row();
        QAbstractItemModel* model = ui->tableView_6->model();
        QString ID = model->data(model->index(row,0)).toString();
        QString name = model->data(model->index(row,1)).toString();
        QString type = model->data(model->index(row,2)).toString();
        modification_admin = new modadmin(nullptr,ID,name,type);
        modification_admin->show();
    }
    else if(index.isValid()&&index.column()==4){//admin delete
        int row = index.row();
        QAbstractItemModel* model = ui->tableView_6->model();
        QString ID = model->data(model->index(row,0)).toString();
        QSqlQuery query;
        bool status = query.exec(tr("delete from admin where adminID = \'")+ID+"\'");
        if(status)
            adminRefresh();
        else
            QMessageBox::critical(this,tr("Delete failed."),tr("Delete failed."));
    }
}

void mainplatformwindow::on_comboBox_2_currentIndexChanged(int index)
{

}

void mainplatformwindow::on_comboBox_2_activated(int index)
{
    switch (index){
        case 0:{
            settings.setValue("theme",":/qss/ElegantDark.qss");
            settings.setValue("themeno",0);
            QString style_sheet = readTextFile(settings.value("theme",":/qss/Aqua.qss").toString());
            qApp->setStyleSheet(style_sheet);
            break;
        }
        case 1:{
            settings.setValue("theme",":/qss/Aqua.qss");
            settings.setValue("themeno",1);
            QString style_sheet = readTextFile(settings.value("theme",":/qss/Aqua.qss").toString());
            qApp->setStyleSheet(style_sheet);
            break;
        }
    }
}

void mainplatformwindow::on_tableView_7_clicked(const QModelIndex &index)
{
    if(index.isValid()&&index.column()==4){//seat_amount
        int row = index.row();
        QAbstractItemModel* model = ui->tableView_7->model();
        QString ID = model->data(model->index(row,1)).toString();
        QString departure_date = model->data(model->index(row,0)).toString();
        show_seat=new show_seat_a("seat_amount",departure_date,ID);
        show_seat->show();
    }
    else if(index.isValid()&&index.column()==5){//seat_arrangement
        int row = index.row();
        QAbstractItemModel* model = ui->tableView_7->model();
        QString ID = model->data(model->index(row,1)).toString();
        QString departure_date = model->data(model->index(row,0)).toString();
        show_seat=new show_seat_a("seat_arrangement",departure_date,ID);
        show_seat->show();

    }
}
