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
#include "vipdiscount.h"
#include "modfliarrange.h"
#include "show_seat_a.h"
#include "addticket.h"
#include "addannouncement.h"
#include "modifyann.h"
#include "showmessage.h"
#include "seat_selection.h"
#include "modifydialog.h"

#include "sqlbrowser/browser.h"

#include "deletedialog.h"

#ifdef WIN32
#include "QRibbon/QRibbon.h"
#endif
#include<QCheckBox>
#include<QToolTip>
#include <QSysInfo>
#include <QSqlDriver>
extern QSqlDatabase db;
extern QByteArray readTextFile(const QString&);
extern mainplatformwindow *w;
QueryDialog *q;
deletedialog *d;
VIPDiscount *vipd;
modifydialog *m;
loginwindow *l;
extern QTranslator translator;
extern QTranslator translatorqt;
extern my_admin tranadmin;
extern QSettings settings;

stopover *stop_over;
adduser *a;
addflight *add_flight;
addairport *add_airport;
addfliarrange *add_fliarrange;
addadmin *add_admin;
show_seat_a *show_seat;
addticket *add_ticket;
addannouncement *add_ann;

modfliarrange *modification_fliarrange;
modadmin *modification_admin;
moduser *modification_user;
modcom *modification_comp;
modflight *modification_flight;
modairport *ma;
modifyann *mod_ann;

showMessage * show_message;

addcom *add_comp;
QTableView *usertable;
QTableView *flighttable;
QTableView *comptable;
QTableView *tickettable;
QTableView *announcementtable;

QTimer *localtimer;
QTimer *localtimer2;
QSignalMapper *usermapper;
QSignalMapper *usermapper_mod;
QSignalMapper *compmapper;
QSignalMapper *compmapper_mod;
const QString osVersion() {
    return QSysInfo::prettyProductName() + " " + QSysInfo::buildAbi() + ", with kernel: " + QSysInfo::kernelType() + " " + QSysInfo::kernelVersion();
}

mainplatformwindow::mainplatformwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mainplatformwindow) {
    ui->setupUi(this);
    ui->fontComboBox->setCurrentFont((settings.value("Platform/UIFont",this->font()).value<QFont>()));
    ui->comboBox->setCurrentIndex(settings.value("Platform/Langcase", 2).toInt());
    on_comboBox_activated(settings.value("Platform/Langcase", 2).toInt());
    QScroller::grabGesture(ui->tableView_2, QScroller::TouchGesture);
    QScroller::grabGesture(ui->tableView_3, QScroller::TouchGesture);
    QScroller::grabGesture(ui->tableView_4, QScroller::TouchGesture);
    QScroller::grabGesture(ui->tableView_5, QScroller::TouchGesture);
    QScroller::grabGesture(ui->tableView_6, QScroller::TouchGesture);
    QScroller::grabGesture(ui->tableView_7, QScroller::TouchGesture);
    QScroller::grabGesture(ui->tableView_8, QScroller::TouchGesture);
    QScroller::grabGesture(ui->tableView_9, QScroller::TouchGesture);
    _init();
}

void mainplatformwindow::_init() {
    //settings.sync();
#ifdef WIN32
    QRibbon::install(this);
#endif
    qApp->setStyleSheet(readTextFile(settings.value("Platform/theme", ":/qss/Aqua.qss").toString()));
    ui->comboBox_2->setCurrentIndex(settings.value("Platform/themeno", 1).toInt());

    qDebug()<<db.driver()->hasFeature(QSqlDriver::Transactions);
    ui->label->setText(tr("System Version:") + osVersion());
    QApplication::processEvents();
    QSqlQuery query = QSqlQuery("select @@VERSION");
    QApplication::processEvents();
    query.next();
    QApplication::processEvents();
    ui->label_2->setText(QString(tr("Server Version:\n ")) + query.value(0).toString());
    QApplication::processEvents();
    query = QSqlQuery("select user()");
    QApplication::processEvents();
    query.next();
    QApplication::processEvents();
    ui->label_3->setText(QString(tr("Login system user: ")) + query.value(0).toString());
    QApplication::processEvents();
    ui->label_4->setText(QString(tr("Language: ")) + QLocale::system().name());
    QApplication::processEvents();


    usertable = ui->tableView;
    QApplication::processEvents();
    flighttable = ui->tableView_3;
    QApplication::processEvents();
    comptable = ui->tableView_4;
    QApplication::processEvents();
    tickettable = ui->tableView_8;
    QApplication::processEvents();

    announcementtable=ui->tableView_9;

    userRefresh();
    QApplication::processEvents();
    flightRefresh();
    QApplication::processEvents();

    QApplication::processEvents();


    ui->pushButton_13->setEnabled(tranadmin.satype);


    //compRefresh();

    localtimer = new QTimer(this);
    localtimer2 =new QTimer(this);
    connect(localtimer, SIGNAL(timeout()), this, SLOT(updatetime()));
    connect(localtimer2, SIGNAL(timeout()), this, SLOT(updatest()));

    localtimer->start(1000);
    localtimer2->start(1000);
    QApplication::processEvents();
    adminRefresh();
    QApplication::processEvents();
    on_horizontalSlider_3_valueChanged(1);//Flight Arrangement Refresh
    QApplication::processEvents();
    on_horizontalSlider_2_valueChanged(1);//Airport Refresh
    QApplication::processEvents();
    on_horizontalSlider_valueChanged(1);//Airline Refresh
    on_horizontalSlider_4_valueChanged(1); //Ticket Refresh
    on_horizontalSlider_5_valueChanged(1); //Annoucement Refresh
    QApplication::processEvents();

    ui->spinBox->setValue(settings.value("Platform/itemsperpage", 20).toInt());
    QApplication::processEvents();
    ui->dateEdit->setDate(QDate::currentDate());
    QApplication::processEvents();
    ui->dateEdit->setMinimumDate(QDate::currentDate());
    QApplication::processEvents();
}


void mainplatformwindow::updatetime() {
    ui->statusBar->showMessage(tr("Welcome, ") + tranadmin.name + "! " + QDateTime::currentDateTime().toString());
}
void mainplatformwindow::updatest() {
    QSqlQueryModel *statistics =new QSqlQueryModel();
    statistics->setQuery("select * from statistics_view");
    ui->staView->setModel(statistics);
    statistics->setHeaderData(0, Qt::Horizontal, QString::fromUtf8(tr("Variables").toUtf8()));
    statistics->setHeaderData(1, Qt::Horizontal, QString::fromUtf8(tr("Value").toUtf8()));
    QSqlQueryModel *infomodel = new QSqlQueryModel;
    QApplication::processEvents();
    infomodel->setQuery(QString("show status"));
    QApplication::processEvents();
    ui->tableView_2->setModel(infomodel);
    QApplication::processEvents();
    ui->tableView_2->resizeColumnsToContents();
    ui->staView->resizeColumnsToContents();
    QApplication::processEvents();

}

mainplatformwindow::~mainplatformwindow() {
    delete ui;
    settings.sync();
}



void mainplatformwindow::on_listWidget_clicked(const QModelIndex &index) {

}




void mainplatformwindow::on_actionAbout_Qt_triggered() {
    QMessageBox::aboutQt(NULL);
}
void mainplatformwindow::on_actionLog_out_triggered() {
    db.close();
    this->close();
    l = new loginwindow;
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

void mainplatformwindow::on_listWidget_user_currentRowChanged(int currentRow) {

}
void mainplatformwindow::fliarrangeRefresh(int page) {
    QSqlQuery query = QSqlQuery("select count(1) from flight_arrangement");
    ui->statusBar->showMessage(tr("Querying..."));
    int itemsperpage = settings.value("Platform/itemsperpage", 20).toInt();
    query.next();
    ui->horizontalSlider_3->setMaximum(query.value(0).toInt() / itemsperpage + 1);
    int item2 = itemsperpage * (page - 1);
    myfliarrangemodel *fliarrangemodel = new myfliarrangemodel;
    fliarrangemodel->setQuery("select * from `flight_arrangement` limit " + QString::number(item2) + "," + QString::number(itemsperpage));
    ui->tableView_7->setModel(fliarrangemodel);

    fliarrangemodel->insertColumn(4);
    fliarrangemodel->setHeaderData(4, Qt::Horizontal, QString::fromUtf8(tr("Seat amount").toUtf8()));
    fliarrangemodel->insertColumn(5);
    fliarrangemodel->setHeaderData(5, Qt::Horizontal, QString::fromUtf8(tr("Seat arrangement").toUtf8()));
    fliarrangemodel->insertColumn(6);
    fliarrangemodel->setHeaderData(6, Qt::Horizontal, QString::fromUtf8(tr("Modify").toUtf8()));
    fliarrangemodel->insertColumn(7);
    fliarrangemodel->setHeaderData(7, Qt::Horizontal, QString::fromUtf8(tr("Delete").toUtf8()));
    fliarrangemodel->setHeaderData(0, Qt::Horizontal, QString::fromUtf8(tr("Departure Date").toUtf8()));
    fliarrangemodel->setHeaderData(1, Qt::Horizontal, QString::fromUtf8(tr("Flight ID").toUtf8()));
    fliarrangemodel->setHeaderData(2, Qt::Horizontal, QString::fromUtf8(tr("Status").toUtf8()));
    fliarrangemodel->setHeaderData(3, Qt::Horizontal, QString::fromUtf8(tr("Discount").toUtf8()));
    ui->tableView_7->resizeColumnsToContents();

}
void mainplatformwindow::airportRefresh(int page) {
    QSqlQuery query = QSqlQuery("select count(1) from airport");
    ui->statusBar->showMessage(tr("Querying..."));
    int itemsperpage = settings.value("Platform/itemsperpage", 20).toInt();
    query.next();
    ui->horizontalSlider_2->setMaximum(query.value(0).toInt() / itemsperpage + 1);
    int item2 = itemsperpage * (page - 1);
    mycompmodel *apmodel = new mycompmodel;
    apmodel->setQuery("select * from airport limit " + QString::number(item2) + "," + QString::number(itemsperpage));
    ui->tableView_5->setModel(apmodel);
    apmodel->insertColumn(3);//这里是在模型的第4列后面插入一列
    apmodel->setHeaderData(3, Qt::Horizontal, QString::fromUtf8(tr("Modify").toUtf8()));
    apmodel->insertColumn(4);
    apmodel->setHeaderData(4, Qt::Horizontal, QString::fromUtf8(tr("Delete").toUtf8()));
    apmodel->setHeaderData(0, Qt::Horizontal, QString::fromUtf8(tr("Airport IATA Code").toUtf8()));
    apmodel->setHeaderData(1, Qt::Horizontal, QString::fromUtf8(tr("Airport Name").toUtf8()));
    apmodel->setHeaderData(2, Qt::Horizontal, QString::fromUtf8(tr("Airport City").toUtf8()));
    ui->tableView_5->resizeColumnsToContents();
}
void mainplatformwindow::annoucementRefresh(int page){
    QSqlQuery query = QSqlQuery("select count(1) from announcement_show");
    ui->statusBar->showMessage(tr("Querying..."));
    int itemsperpage = settings.value("Platform/itemsperpage", 20).toInt();
    query.next();
    ui->horizontalSlider_5->setMaximum(query.value(0).toInt() / itemsperpage + 1);
    int item2 = itemsperpage * (page - 1);
    myannouncementmodel *anmodel = new myannouncementmodel;
    anmodel->setQuery("select * from announcement_show limit " + QString::number(item2) + "," + QString::number(itemsperpage));
    ui->tableView_9->setModel(anmodel);
    anmodel->insertColumn(3);
    anmodel->setHeaderData(3, Qt::Horizontal, QString::fromUtf8(tr("Modify").toUtf8()));
    anmodel->insertColumn(4);
    anmodel->setHeaderData(4, Qt::Horizontal, QString::fromUtf8(tr("Delete").toUtf8()));
    anmodel->setHeaderData(0, Qt::Horizontal, QString::fromUtf8(tr("User ID").toUtf8()));
    anmodel->setHeaderData(1, Qt::Horizontal, QString::fromUtf8(tr("Delivery Time").toUtf8()));
    anmodel->setHeaderData(2, Qt::Horizontal, QString::fromUtf8(tr("Message").toUtf8()));
    ui->tableView_9->resizeColumnsToContents();

}
void mainplatformwindow::adminRefresh() {
    if(tranadmin.satype) {
        myadminmodel *admodel = new myadminmodel;
        admodel->setQuery("select adminID,adminName,satype from admin");
        ui->tableView_6->setModel(admodel);
        admodel->insertColumn(3);//这里是在模型的第4列后面插入一列
        admodel->setHeaderData(3, Qt::Horizontal, QString::fromUtf8(tr("Modify").toUtf8()));
        admodel->insertColumn(4);
        admodel->setHeaderData(4, Qt::Horizontal, QString::fromUtf8(tr("Delete").toUtf8()));
        admodel->setHeaderData(0, Qt::Horizontal, QString::fromUtf8(tr("Admin ID").toUtf8()));
        admodel->setHeaderData(1, Qt::Horizontal, QString::fromUtf8(tr("Admin Name").toUtf8()));
        admodel->setHeaderData(2, Qt::Horizontal, QString::fromUtf8(tr("SA Type").toUtf8()));
        ui->tableView_6->resizeColumnsToContents();
    } else {
        ui->label_5->setText("Only For Super Administrator, but you are not a SA.");
    }
}
void mainplatformwindow::flightRefresh() {
    myflightmodel *flightmodel = new myflightmodel;
    flightmodel->setQuery("select flight_id,schedule,plane_type,depap_id,departure_time,arrap_id,arrival_time,company_id from flight a inner join (select dep.flight_id flight_id,dep.airport_id depap_id,dep.departure_time,arr.airport_id arrap_id,arr.arrival_time \
                          From (select * from airline where arrival_time is null)  dep,(select * from airline where departure_time is null) arr \
                          where dep.flight_id=arr.flight_id) b using (flight_id)");
    flighttable->setModel(flightmodel);
    flightmodel->insertColumn(8);
    flightmodel->setHeaderData(8, Qt::Horizontal, QString::fromUtf8(tr("Stopover").toUtf8()));
    flightmodel->insertColumn(9);
    flightmodel->setHeaderData(9, Qt::Horizontal, QString::fromUtf8(tr("Price").toUtf8()));
    flightmodel->insertColumn(10);//这里是在模型的第4列后面插入一列
    flightmodel->setHeaderData(10, Qt::Horizontal, QString::fromUtf8(tr("Modify").toUtf8()));
    flightmodel->insertColumn(11);
    flightmodel->setHeaderData(11, Qt::Horizontal, QString::fromUtf8(tr("Delete").toUtf8()));
    flightmodel->setHeaderData(0, Qt::Horizontal, QString::fromUtf8(tr("Flight ID").toUtf8()));
    flightmodel->setHeaderData(1, Qt::Horizontal, QString::fromUtf8(tr("Schedule").toUtf8()));
    flightmodel->setHeaderData(2, Qt::Horizontal, QString::fromUtf8(tr("Plane Type").toUtf8()));
    flightmodel->setHeaderData(3, Qt::Horizontal, QString::fromUtf8(tr("Depature Airport ID").toUtf8()));
    flightmodel->setHeaderData(4, Qt::Horizontal, QString::fromUtf8(tr("Depature Time").toUtf8()));
    flightmodel->setHeaderData(5, Qt::Horizontal, QString::fromUtf8(tr("Arrival Airport ID").toUtf8()));
    flightmodel->setHeaderData(6, Qt::Horizontal, QString::fromUtf8(tr("Arrival Time").toUtf8()));
    flightmodel->setHeaderData(7, Qt::Horizontal, QString::fromUtf8(tr("Airline Company ID").toUtf8()));
    flighttable->resizeColumnsToContents();
}
void mainplatformwindow::ticketRefresh(int page) {
    QSqlQuery query = QSqlQuery("select count(1) from ticket_all_view");
    ui->statusBar->showMessage(tr("Querying..."));
    int itemsperpage=settings.value("Platform/itemsperpage",20).toInt();
    query.next();
    ui->horizontalSlider_4->setMaximum(query.value(0).toInt()/itemsperpage+1);

    myticketmodel *ticketmodel = new myticketmodel;
    int item2 = itemsperpage*(page-1);
    ticketmodel->setQuery("select * from ticket_all_view limit "+QString::number(item2)+","+QString::number(itemsperpage));
    QApplication::processEvents();
    tickettable->setModel(ticketmodel);
    QApplication::processEvents();
    ticketmodel->insertColumn(12);
    ticketmodel->setHeaderData(12,Qt::Horizontal,QString::fromUtf8(tr("Refund").toUtf8()));
    ticketmodel->insertColumn(13);
    ticketmodel->setHeaderData(13,Qt::Horizontal,QString::fromUtf8(tr("Delete").toUtf8()));
    ticketmodel->insertColumn(14);
    ticketmodel->setHeaderData(14,Qt::Horizontal,QString::fromUtf8(tr("Check In").toUtf8()));


    ticketmodel->setHeaderData(0,Qt::Horizontal,QString::fromUtf8(tr("Ticket ID").toUtf8()));
    ticketmodel->setHeaderData(1,Qt::Horizontal,QString::fromUtf8(tr("User ID").toUtf8()));
    ticketmodel->setHeaderData(2,Qt::Horizontal,QString::fromUtf8(tr("Flight ID").toUtf8()));
    ticketmodel->setHeaderData(3,Qt::Horizontal,QString::fromUtf8(tr("Departure Datetime").toUtf8()));
    ticketmodel->setHeaderData(4,Qt::Horizontal,QString::fromUtf8(tr("Class").toUtf8()));
    ticketmodel->setHeaderData(5,Qt::Horizontal,QString::fromUtf8(tr("Purchase Date").toUtf8()));
    ticketmodel->setHeaderData(6,Qt::Horizontal,QString::fromUtf8(tr("Actual Payment").toUtf8()));
    ticketmodel->setHeaderData(7,Qt::Horizontal,QString::fromUtf8(tr("Departure Airport").toUtf8()));
    ticketmodel->setHeaderData(8,Qt::Horizontal,QString::fromUtf8(tr("Arrival Airport").toUtf8()));
    ticketmodel->setHeaderData(9,Qt::Horizontal,QString::fromUtf8(tr("Refund Date").toUtf8()));
    ticketmodel->setHeaderData(10,Qt::Horizontal,QString::fromUtf8(tr("Actual Refund").toUtf8()));
    ticketmodel->setHeaderData(11,Qt::Horizontal,QString::fromUtf8(tr("Seat ID").toUtf8()));





    tickettable->resizeColumnsToContents();
    ui->statusBar->showMessage(tr("Completing..."));
    ui->statusBar->showMessage(tr("Completed!"));
}


void mainplatformwindow::compRefresh(int page) {
    QSqlQuery query = QSqlQuery("select count(1) from company");
    ui->statusBar->showMessage(tr("Querying..."));
    int itemsperpage = settings.value("Platform/itemsperpage", 20).toInt();
    query.next();
    ui->horizontalSlider->setMaximum(query.value(0).toInt() / itemsperpage + 1);
    //compmapper= new QSignalMapper;
    //compmapper_mod = new QSignalMapper;
    mycompmodel *compmodel = new mycompmodel;
    int item2 = itemsperpage * (page - 1);
    compmodel->setQuery("select * from company limit " + QString::number(item2) + "," + QString::number(itemsperpage));
    comptable->setModel(compmodel);

    compmodel->insertColumn(3);//这里是在模型的第4列后面插入一列
    compmodel->setHeaderData(3, Qt::Horizontal, QString::fromUtf8(tr("Modify").toUtf8()));
    compmodel->insertColumn(4);
    compmodel->setHeaderData(4, Qt::Horizontal, QString::fromUtf8(tr("Delete").toUtf8()));
    compmodel->setHeaderData(0, Qt::Horizontal, QString::fromUtf8(tr("Airline Company Code").toUtf8()));
    compmodel->setHeaderData(1, Qt::Horizontal, QString::fromUtf8(tr("Airline Company Name").toUtf8()));
    compmodel->setHeaderData(2, Qt::Horizontal, QString::fromUtf8(tr("Account").toUtf8()));
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
void mainplatformwindow::userRefresh() {
    //usermapper= new QSignalMapper;
    //usermapper_mod=new QSignalMapper;
    myusermodel *usermodel = new myusermodel;
    usermodel->setQuery("select ID,name,membership,account from user");
    usertable->setModel(usermodel);
    usermodel->insertColumn(4);//这里是在模型的第4列后面插入一列
    usermodel->setHeaderData(4, Qt::Horizontal, QString::fromUtf8(tr("Modify").toUtf8()));
    usermodel->insertColumn(5);
    usermodel->setHeaderData(5, Qt::Horizontal, QString::fromUtf8(tr("Delete").toUtf8()));
    usermodel->setHeaderData(0, Qt::Horizontal, QString::fromUtf8(tr("ID").toUtf8()));
    usermodel->setHeaderData(1, Qt::Horizontal, QString::fromUtf8(tr("Name").toUtf8()));
    usermodel->setHeaderData(2, Qt::Horizontal, QString::fromUtf8(tr("Membership").toUtf8()));
    usermodel->setHeaderData(3, Qt::Horizontal, QString::fromUtf8(tr("Account").toUtf8()));
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

    for(int i = 0; i < usermodel->rowCount(); i++) {

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
void mainplatformwindow::on_listWidget_user_itemClicked(QListWidgetItem *item) {
    if(item->text() == tr("Add")) {
        a = new adduser;
        a->show();
    } else if(item->text() == tr("Refresh")) {
        userRefresh();
    } else if(item->text() == tr("Search")) {

      QStringList sqllist = {"ID", "name", "membership","account"};
      QStringList indexlist = {"User ID", "Name", "Membership","Account"};
      QString table = "user";
      q = new QueryDialog(table, sqllist, indexlist);
      q->show();
    }else if(item->text()==tr("Delete")){
        QStringList sqllist = {"ID", "name", "membership","account"};
        QStringList indexlist = {"User ID", "Name", "Membership","Account"};
        QString table = "user";
        d = new deletedialog(table, sqllist, indexlist);
        d->show();
    }else if(item->text()==tr("Modify")){
        QStringList sqllist = {"ID", "name", "membership","account"};
        QStringList indexlist = {"User ID", "Name", "Membership","Account"};
        QString table = "user";

        m = new modifydialog(table, sqllist, indexlist);
        m->show();
    }else if(item->text() == tr("VIP Discount")){
        vipd = new VIPDiscount();
        vipd->show();
    }
}
QString getAirportName(QString apcode){
    QSqlQuery query;
    query.exec("select airport_name from airport where airport_id=\'"+apcode+"\'");
    query.next();
    return query.value(0).toString();
}
void mainplatformwindow::on_tableView_3_clicked(const QModelIndex &index) {
    if(index.isValid() && index.column() == 8) {
        int row = index.row();
        QAbstractItemModel* model = ui->tableView_3->model();
        QString flight_id = model->data(model->index(row, 0)).toString();
        stop_over = new stopover(nullptr, flight_id);
        stop_over->show();
    } else if(index.isValid() && index.column() == 11) {
        QMessageBox::StandardButton btn;
        btn = QMessageBox::question(this, tr("himt:"), tr("Are you sure to delete?"), QMessageBox::Yes|QMessageBox::No);
        if (btn == QMessageBox::Yes) {
            int row = index.row();
            QAbstractItemModel* model = ui->tableView_3->model();
            QString flight_id = model->data(model->index(row, 0)).toString();

    #ifdef Q_OS_ANDROID
            QSqlQuery transaction;
            if(transaction.exec("start transaction")){
    #else
            if(QSqlDatabase::database().transaction()){
    #endif
                QSqlQuery query;
                query.exec(tr("delete from airline where flight_id = \'") + flight_id + "\'");
                query.exec(tr("delete from flight where flight_id = \'") + flight_id + "\'");
                query.exec(tr("delete from seat where flight_id = \'") + flight_id + "\'");
                query.exec(tr("delete from price where flight_id = \'") + flight_id + "\'");


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
                        qDebug() << QSqlDatabase::database().lastError();
                    }

                } else {
                    flightRefresh();
                }
            }
        }

    } else if(index.isValid() && index.column() == 10) {
        int row = index.row();
        QAbstractItemModel* model = ui->tableView_3->model();
        QString flight_id = model->data(model->index(row, 0)).toString();
        QString schedule = model->data(model->index(row, 1)).toString();
        QString plane_type = model->data(model->index(row, 2)).toString();
        QString company_id = model->data(model->index(row, 7)).toString();
        modification_flight = new modflight(nullptr, flight_id, schedule, company_id, plane_type);
        modification_flight->show();
    } else if(index.isValid() && index.column() == 9){
        int row=index.row();
        QAbstractItemModel* model=ui->tableView_3->model();
        QString flightid=model->data(model->index(row,0)).toString();
        QSqlQuery query;
        query.exec("select * from mod_price where flight_id=\'"+flightid+"\'");
        QString result;
        while(query.next()){
            result+=tr("From %1(%2) to %3(%4), Class %6,Original Price is ￥%5\n")
                          .arg(getAirportName(query.value(1).toString()))
                          .arg(query.value(1).toString()).arg(getAirportName(query.value(2).toString()))
                          .arg(query.value(2).toString()).arg(query.value(3).toString()).arg(query.value(4).toInt()?tr("Economy"):tr("Business"));

        }
        QMessageBox::information(NULL,tr("Price of %1").arg(flightid),result==""?tr("There is no price information yet."):result);
    }

}

void mainplatformwindow::on_listWidget_3_itemClicked(QListWidgetItem *item) {
    if(item->text() == tr("Add")) {
        add_flight = new addflight;
        add_flight->show();
    } else if(item->text() == tr("Refresh")) {
        flightRefresh();
    } else if(item->text() == tr("Search")) {
        QStringList indexlist = {"Flight Id", "Schedule", "Plane Type","Departure Airport ID","Departure Time","Arrival Airport ID","Arrival Time","Company Id"};
        QStringList sqllist = {"flight_id", "schedule", "plane_type","depap_id","departure_time","arrap_id","arrival_time","company_id"};
        QString table = QString("flight_for_query");
        q = new QueryDialog(table, sqllist, indexlist);
        q->show();
    }else if(item->text()==tr("Delete")){

        QStringList indexlist = {"Flight Id", "Schedule", "Plane Type","Departure Airport ID","Departure Time","ArrivalAirport ID","Arrival Time","Company Id"};
        QStringList sqllist = {"flight_id", "schedule", "plane_type","depap_id","departure_time","arrap_id","arrival_time","company_id"};
        QString table = QString("flight_for_query");
        d = new deletedialog(table, sqllist, indexlist);
        d->show();

    }else if(item->text()==tr("Modify")){
        QStringList indexlist = {"Flight Id", "Schedule", "Plane Type","Departure Airport ID","Departure Time","ArrivalAirport ID","Arrival Time","Company Id"};
        QStringList sqllist = {"flight_id", "schedule", "plane_type","depap_id","departure_time","arrap_id","arrival_time","company_id"};
        QString table = QString("flight_for_query");

        m = new modifydialog(table, sqllist, indexlist);
        m->show();
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
void mainplatformwindow::on_listWidget_5_itemClicked(QListWidgetItem *item) {
    if(item->text() == tr("Add")) {
        add_comp = new addcom;
        add_comp->show();
    } else if(item->text() == tr("Refresh")) {
        ui->horizontalSlider->setValue(1);
        on_horizontalSlider_valueChanged(1);
    } else if(item->text() == tr("Search")) {
        QStringList sqllist = {"company_id", "company_name", "company_account"};
        QStringList indexlist = {"Code", "Name", "Account"};
        QString table = "company";
        q = new QueryDialog(table, sqllist, indexlist);
        q->show();
    }else if(item->text()==tr("Delete")){
        QStringList sqllist = {"company_id", "company_name", "company_account"};
        QStringList indexlist = {"Code", "Name", "Account"};
        QString table = "company";
        d = new deletedialog(table, sqllist, indexlist);
        d->show();
    }else if(item->text()==tr("Modify")){
        QStringList sqllist = {"company_id", "company_name", "company_account"};
        QStringList indexlist = {"Code", "Name", "Account"};
        QString table = "company";

        m = new modifydialog(table, sqllist, indexlist);
        m->show();
    }

}


void mainplatformwindow::on_horizontalSlider_valueChanged(int value) {
    ui->statusBar->showMessage(tr("Querying..."));
    ui->plainTextEdit->setText(QString::number(value));
    compRefresh(value);
}


void mainplatformwindow::on_tableView_clicked(const QModelIndex &index) {
    if(index.isValid() && index.column() == 4) { //user_modification
        int row = index.row();
        QAbstractItemModel* model = ui->tableView->model();
        QString ID = model->data(model->index(row, 0)).toString();
        QString name = model->data(model->index(row, 1)).toString();
        QString membership = model->data(model->index(row, 2)).toString();
        QString account = model->data(model->index(row, 3)).toString();
        modification_user = new moduser(nullptr, ID, name, membership, account);
        modification_user->show();
    } else if(index.isValid() && index.column() == 5) { //user_delete
        QMessageBox::StandardButton btn;
        btn = QMessageBox::question(this, tr("himt:"), tr("Are you sure to delete?"), QMessageBox::Yes|QMessageBox::No);
        if (btn == QMessageBox::Yes) {
        int row = index.row();
        QAbstractItemModel* model = ui->tableView->model();
        QString ID = model->data(model->index(row, 0)).toString();
        QSqlQuery query;
        bool status = query.exec(QString("delete from user where ID = \'") + ID + "\'");

        if(status) {
            userRefresh();
        } else {
            QMessageBox::critical(this, tr("Delete failed."), tr("Delete failed."));
        }
        }
    }
}

void mainplatformwindow::on_tableView_4_clicked(const QModelIndex &index) {
    if(index.isValid() && index.column() == 3) { //company_modification
        int row = index.row();
        QAbstractItemModel* model = ui->tableView_4->model();
        QString ID = model->data(model->index(row, 0)).toString();
        QString name = model->data(model->index(row, 1)).toString();
        QString account = model->data(model->index(row, 2)).toString();
        modification_comp = new modcom(nullptr, ID, name, account);
        modification_comp->show();
    } else if(index.isValid() && index.column() == 4) { //company_delete
        QMessageBox::StandardButton btn;
        btn = QMessageBox::question(this, tr("himt:"), tr("Are you sure to delete?"), QMessageBox::Yes|QMessageBox::No);
        if (btn == QMessageBox::Yes) {
        int row = index.row();
        QAbstractItemModel* model = ui->tableView_4->model();
        QString ID = model->data(model->index(row, 0)).toString();
        QSqlQuery query;
        bool status = query.exec(tr("delete from company where company_id = \'") + ID + "\'");

        if(status) {
            compRefresh();
        } else {
            QMessageBox::critical(this, tr("Delete failed."), tr("Delete failed."));
        }
        }
    }
}

QVariant mycompmodel::data(const QModelIndex &item, int role) const {
    QVariant value = QSqlQueryModel::data(item, role);
    if (Qt::TextAlignmentRole == role ) {
        value = Qt::AlignCenter;
    }
    if(role == Qt::BackgroundColorRole) {
        if(item.column() == 3) {
            return QVariant::fromValue(QColor(225, 225, 225));
        } else if(item.column() == 4) {
            return QVariant::fromValue(QColor(225, 225, 225));    //第一个属性的字体颜色为红色
        }
    }

    if(role == Qt::DisplayRole) {
        if(item.column() == 3) {
            return QVariant::fromValue(tr("Modify"));
        } else if(item.column() == 4) {
            return QVariant::fromValue(tr("Delete"));
        }
    }

    return value;
}
QVariant myticketmodel::data(const QModelIndex &item, int role) const {
    QVariant value = QSqlQueryModel::data(item, role);
    if (role == Qt::BackgroundColorRole) {
        if(item.column()==13||item.column()==12||item.column()==14)
            return QVariant::fromValue(QColor(225,225,225));
    }
    if (Qt::TextAlignmentRole == role ) {
        value = Qt::AlignCenter;
    }
    if (role == Qt::DisplayRole) {
        if(item.column()==12)
            return QVariant::fromValue(tr("Refund"));
        if(item.column()==13)
            return QVariant::fromValue(tr("Delete"));
        if(item.column()==14)
            return QVariant::fromValue(tr("Check In"));

    }
    return value;
}

QVariant myannouncementmodel::data(const QModelIndex &item, int role) const{
    QVariant value = QSqlQueryModel::data(item, role);

    if(role == Qt::BackgroundColorRole) {
        if(item.column() == 3) {
            return QVariant::fromValue(QColor(225, 225, 225));
        } else if(item.column() == 4) {
            return QVariant::fromValue(QColor(225, 225, 225));    //第一个属性的字体颜色为红色
        }
    }

    if(role == Qt::DisplayRole) {
        if(item.column() == 3) {
            return QVariant::fromValue(tr("Modify"));
        } else if(item.column() == 4) {
            return QVariant::fromValue(tr("Delete"));
        }
    }
    if (Qt::TextAlignmentRole == role ) {
        value = Qt::AlignCenter;
    }
    return value;
}

QVariant myflightmodel::data(const QModelIndex &item, int role) const {
    QVariant value = QSqlQueryModel::data(item, role);

    if(role == Qt::BackgroundColorRole) {
        if(item.column() == 8 || item.column() == 9 || item.column() == 10|| item.column() == 11) {
            return QVariant::fromValue(QColor(225, 225, 225));
        }
    }
    if (Qt::TextAlignmentRole == role ) {
        value = Qt::AlignCenter;
    }
    if(role == Qt::DisplayRole) {
        if(item.column() == 10) {
            return QVariant::fromValue(tr("Modify"));
        } else if(item.column() == 11) {
            return QVariant::fromValue(tr("Delete"));
        } else if(item.column() == 8) {
            return QVariant::fromValue(tr("Stopover"));
        }else if(item.column()==9){
            return QVariant::fromValue(tr("Price"));

        }
    }

    if(role == Qt::ToolTipRole) {
        if(item.isValid() && (item.column() == 5 || item.column() == 3)) {
            QSqlQuery query = QSqlQuery(QString("select airport_name from airport where airport_id=\'") + item.data().toString() + "\'");
            query.next();
            return QVariant::fromValue(query.value(0).toString());
        } else if(item.isValid() && (item.column() == 7)) {
            QSqlQuery query = QSqlQuery(QString("select company_name from company where company_id=\'") + item.data().toString() + "\'");
            query.next();
            return QVariant::fromValue(query.value(0).toString());
        }
    }

    return value;
}
QVariant myusermodel::data(const QModelIndex &item, int role) const {
    QVariant value = QSqlQueryModel::data(item, role);

    if(role == Qt::BackgroundColorRole) {
        if(item.column() == 4) {
            return QVariant::fromValue(QColor(225, 225, 225));
        } else if(item.column() == 5) {
            return QVariant::fromValue(QColor(225, 225, 225));    //第一个属性的字体颜色为灰色
        }
    }
    if (Qt::TextAlignmentRole == role ) {
        value = Qt::AlignCenter;
    }

    if(role == Qt::DisplayRole) {
        if(item.column() == 2) {
            if(QSqlQueryModel::data(item).toInt() == 0) {
                return QVariant::fromValue(QString("×"));
            } else {
                return QVariant::fromValue(QString("√"));
            }
        }

        if(item.column() == 4) {
            return QVariant::fromValue(tr("Modify"));
        } else if(item.column() == 5) {
            return QVariant::fromValue(tr("Delete"));
        }
    }

    return value;
}
QVariant myfliarrangemodel::data(const QModelIndex &item, int role) const {
    QVariant value = QSqlQueryModel::data(item, role);

    if(role == Qt::BackgroundColorRole) {
        if(item.column() == 4 || item.column() == 5 || item.column() == 6 || item.column() == 7) {
            return QVariant::fromValue(QColor(225, 225, 225));    //第一个属性的字体颜色为灰色
        }
    }
    if (Qt::TextAlignmentRole == role ) {
        value = Qt::AlignCenter;
    }
    if(role == Qt::DisplayRole) {
        if(item.column() == 4) {
            return QVariant::fromValue(tr("Click to View"));
        } else if(item.column() == 5) {
            return QVariant::fromValue(tr("Click to View"));
        } else if(item.column() == 6) {
            return QVariant::fromValue(tr("Modify"));
        } else if(item.column() == 7) {
            return QVariant::fromValue(tr("Delete"));
        }
    }



    return value;
}
QVariant myadminmodel::data(const QModelIndex &item, int role) const {
    QVariant value = QSqlQueryModel::data(item, role);

    if(role == Qt::BackgroundColorRole) {
        if(item.column() == 3) {
            return QVariant::fromValue(QColor(225, 225, 225));
        } else if(item.column() == 4) {
            return QVariant::fromValue(QColor(225, 225, 225));    //第一个属性的字体颜色为灰色
        }
    }

    if(role == Qt::DisplayRole) {
        if(item.column() == 2) {
            if(QSqlQueryModel::data(item).toInt() == 0) {
                return QVariant::fromValue(QString("×"));
            } else {
                return QVariant::fromValue(QString("√"));
            }
        }

        if(item.column() == 3) {
            return QVariant::fromValue(tr("Modify"));
        } else if(item.column() == 4) {
            return QVariant::fromValue(tr("Delete"));
        }
    }
    if (Qt::TextAlignmentRole == role ) {
        value = Qt::AlignCenter;
    }
    return value;
}

void mainplatformwindow::on_plainTextEdit_returnPressed() {
    ui->horizontalSlider->setValue(ui->plainTextEdit->text().toInt());
}

void mainplatformwindow::on_pushButton_clicked() {
    ui->horizontalSlider->setValue(ui->horizontalSlider->value() - 1);
}

void mainplatformwindow::on_pushButton_2_clicked() {
    ui->horizontalSlider->setValue(ui->horizontalSlider->value() + 1);
}





void mainplatformwindow::on_horizontalSlider_2_valueChanged(int value) {
    ui->statusBar->showMessage(tr("Querying..."));
    ui->plainTextEdit_2->setText(QString::number(value));
    airportRefresh(value);
}

void mainplatformwindow::on_pushButton_3_clicked() {
    ui->horizontalSlider_2->setValue(ui->horizontalSlider_2->value() - 1);
}

void mainplatformwindow::on_plainTextEdit_2_returnPressed() {
    ui->horizontalSlider_2->setValue(ui->plainTextEdit_2->text().toInt());
}

void mainplatformwindow::on_pushButton_4_clicked() {
    ui->horizontalSlider_2->setValue(ui->horizontalSlider_2->value() + 1);
}

void mainplatformwindow::on_listWidget_6_itemClicked(QListWidgetItem *item) {
    if(item->text() == tr("Add")) {
        add_airport = new addairport;
        add_airport->show();
    } else if(item->text() == tr("Refresh")) {
        ui->horizontalSlider_2->setValue(1);
        on_horizontalSlider_2_valueChanged(1);
    } else if(item->text() == tr("Search")) {
        QStringList sqllist = {"airport_id", "airport_name", "city"};
        QStringList indexlist = {"ID", "Name", "City"};
        QString table = "airport";
        q = new QueryDialog(table, sqllist, indexlist);
        q->show();
    }else if(item->text()==tr("Delete")){
        QStringList sqllist = {"airport_id", "airport_name", "city"};
        QStringList indexlist = {"ID", "Name", "City"};
        QString table = "airport";
        d = new deletedialog(table, sqllist, indexlist);
        d->show();
    }else if(item->text()==tr("Modify")){
        QStringList sqllist = {"airport_id", "airport_name", "city"};
        QStringList indexlist = {"ID", "Name", "City"};
        QString table = "airport";

        m = new modifydialog(table, sqllist, indexlist);
        m->show();
    }


}

void mainplatformwindow::on_tableView_5_clicked(const QModelIndex &index) {
    if(index.isValid() && index.column() == 3) { //company_modification
        int row = index.row();
        QAbstractItemModel* model = ui->tableView_5->model();
        QString ID = model->data(model->index(row, 0)).toString();
        QString name = model->data(model->index(row, 1)).toString();
        QString account = model->data(model->index(row, 2)).toString();
        ma = new modairport(nullptr, ID, name, account);
        ma->show();
    } else if(index.isValid() && index.column() == 4) { //company_delete
        QMessageBox::StandardButton btn;
        btn = QMessageBox::question(this, tr("himt:"), tr("Are you sure to delete?"), QMessageBox::Yes|QMessageBox::No);
        if (btn == QMessageBox::Yes) {
        int row = index.row();
        QAbstractItemModel* model = ui->tableView_5->model();
        QString ID = model->data(model->index(row, 0)).toString();
        QSqlQuery query;
        bool status = query.exec(tr("delete from airport where airport_id = \'") + ID + "\'");

        if(status) {
            on_horizontalSlider_2_valueChanged(1);
        } else {
            QMessageBox::critical(this, tr("Delete failed."), tr("Delete failed."));
        }
        }
    }
}

void mainplatformwindow::on_comboBox_activated(int index) {
    switch(index) {
    case 0: {
        qApp->removeTranslator(&translator);
        qApp->removeTranslator(&translatorqt);
        ui->retranslateUi(this);
        settings.setValue("Platform/Langcase", 0);
        break;
    }

    case 1: {
        qApp->removeTranslator(&translator);
        qApp->removeTranslator(&translatorqt);
        QString langdir = ":/platform_zh_CN.qm";
        translator.load(langdir);
        translatorqt.load("./translations/qt_zh_CN.qm");
        qApp->installTranslator(&translatorqt);
        qApp->installTranslator(&translator);

        ui->retranslateUi(this);
        settings.setValue("Platform/Langcase", 1);
        break;
    }

    default : {
        qApp->removeTranslator(&translator);
        qApp->removeTranslator(&translatorqt);
        QString langdir = ":/platform_" + QLocale::system().name() + ".qm";
        translator.load(langdir);
        translatorqt.load("./translations/qt_"+ QLocale::system().name() + ".qm");
        qApp->installTranslator(&translator);
        qApp->installTranslator(&translatorqt);

        ui->retranslateUi(this);
        settings.setValue("Platform/Langcase", 2);
    }
    }

    //settings.sync();
    _init();
}

void mainplatformwindow::on_pushButton_5_clicked() {
    int week = ui->lineEdit->text().toInt();
    GenArran gen(week, this, ui->dateEdit->date(), (ui->comboBox_3->currentIndex() ? 1 : 7));
    gen.run();
    gen.quit();
}

void mainplatformwindow::on_pushButton_6_clicked() {
    DropArran drop(this);
    drop.run();
}

void mainplatformwindow::on_listWidget_7_itemClicked(QListWidgetItem *item) {
    if(item->text() == tr("Add")) {
        add_fliarrange = new addfliarrange;
        add_fliarrange->show();
    } else if(item->text() == tr("Refresh")) {
        ui->horizontalSlider_3->setValue(1);
        on_horizontalSlider_3_valueChanged(1);
    } else if(item->text() == tr("Search")) {
        QStringList sqllist = {"departure_date", "flight_id", "status","discount"};
        QStringList indexlist = {"Departure Date", "Flight ID", "Status","Discount"};
        QString table = "flight_arrangement";
        q = new QueryDialog(table, sqllist, indexlist);
        q->show();

    }else if(item->text()==tr("Delete")){
        QStringList sqllist = {"departure_date", "flight_id", "status","discount"};
        QStringList indexlist = {"Departure Date", "Flight ID", "Status","Discount"};
        QString table = "flight_arrangement";
        d = new deletedialog(table, sqllist, indexlist);
        d->show();
    }else if(item->text()==tr("Modify")){
        QStringList sqllist = {"departure_date", "flight_id", "status","discount"};
        QStringList indexlist = {"Departure Date", "Flight ID", "Status","Discount"};
        QString table = "flight_arrangement";

        m = new modifydialog(table, sqllist, indexlist);
        m->show();
    }



}

void mainplatformwindow::on_spinBox_valueChanged(int arg1) {
    settings.setValue("Platform/itemsperpage", arg1);
    //settings.sync();
    flightRefresh();
    airportRefresh();
}
void mainplatformwindow::on_actionE_xit_triggered() {
    qApp->quit();
}

void mainplatformwindow::on_listWidget_itemClicked(QListWidgetItem *item) {
    if(item->text() == tr("Add") && tranadmin.satype) {
        add_admin = new addadmin;
        add_admin->show();
    } else if(item->text() == tr("Refresh")) {
        adminRefresh();
    }
}

void mainplatformwindow::on_tableView_6_clicked(const QModelIndex &index) {
    if(index.isValid() && index.column() == 3) { //admin modification
        int row = index.row();
        QAbstractItemModel* model = ui->tableView_6->model();
        QString ID = model->data(model->index(row, 0)).toString();
        QString name = model->data(model->index(row, 1)).toString();
        QString type = model->data(model->index(row, 2)).toString();
        modification_admin = new modadmin(nullptr, ID, name, type);
        modification_admin->show();
    } else if(index.isValid() && index.column() == 4) { //admin delete
        QMessageBox::StandardButton btn;
        btn = QMessageBox::question(this, tr("himt:"), tr("Are you sure to delete?"), QMessageBox::Yes|QMessageBox::No);
        if (btn == QMessageBox::Yes) {
        int row = index.row();
        QAbstractItemModel* model = ui->tableView_6->model();
        QString ID = model->data(model->index(row, 0)).toString();
        QSqlQuery query;
        bool status = query.exec(tr("delete from admin where adminID = \'") + ID + "\'");

        if(status) {
            adminRefresh();
        } else {
            QMessageBox::critical(this, tr("Delete failed."), tr("Delete failed."));
        }
        }
    }
}

void mainplatformwindow::on_comboBox_2_currentIndexChanged(int index) {

}

void mainplatformwindow::on_comboBox_2_activated(int index) {
    switch(index) {
    case 0: {
        settings.setValue("Platform/theme", ":/qss/ElegantDark.qss");
        settings.setValue("Platform/themeno", 0);
        QString style_sheet = readTextFile(settings.value("Platform/theme", ":/qss/Aqua.qss").toString());
        qApp->setStyleSheet(style_sheet);
        break;
    }

    case 1: {
        settings.setValue("Platform/theme", ":/qss/Aqua.qss");
        settings.setValue("Platform/themeno", 1);
        QString style_sheet = readTextFile(settings.value("Platform/theme", ":/qss/Aqua.qss").toString());
        qApp->setStyleSheet(style_sheet);
        break;
    }
    }
}

void mainplatformwindow::on_tableView_7_clicked(const QModelIndex &index) {
    if(index.isValid() && index.column() == 4) { //seat_amount
        int row = index.row();
        QAbstractItemModel* model = ui->tableView_7->model();
        QString ID = model->data(model->index(row, 1)).toString();
        QString departure_date = model->data(model->index(row, 0)).toString();
        show_seat = new show_seat_a("seat_amount", departure_date, ID);
        show_seat->show();
    } else if(index.isValid() && index.column() == 5) { //seat_arrangement
        int row = index.row();
        QAbstractItemModel* model = ui->tableView_7->model();
        QString ID = model->data(model->index(row, 1)).toString();
        QString departure_date = model->data(model->index(row, 0)).toString();
        show_seat = new show_seat_a("seat_arrangement", departure_date, ID);
        show_seat->show();
    } else if(index.isValid() && index.column() == 6) { //modify
        int row = index.row();
        QAbstractItemModel* model = ui->tableView_7->model();
        QString ID = model->data(model->index(row, 1)).toString();
        QString departure_date = model->data(model->index(row, 0)).toString();
        QString status = model->data(model->index(row, 2)).toString();
        QString discount = model->data(model->index(row, 3)).toString();
        modification_fliarrange = new modfliarrange(departure_date, ID, status, discount);
        modification_fliarrange->show();
    } else if(index.isValid() && index.column() == 7) { //cancel
        QMessageBox::StandardButton btn;
        btn = QMessageBox::question(this, tr("himt:"), tr("Are you sure to delete?"), QMessageBox::Yes|QMessageBox::No);
        if (btn == QMessageBox::Yes) {
        int row = index.row();
        QAbstractItemModel* model = ui->tableView_7->model();
        QString ID = model->data(model->index(row, 1)).toString();
        QString departure_date = model->data(model->index(row, 0)).toString();
        QString sql = QString("CALL del_fliarrangement('%1','%2')").arg(ID).arg(departure_date);
        QSqlQuery query;
        bool status = query.exec(sql);

        if(status) {
            fliarrangeRefresh();
        } else {
            QMessageBox::critical(this, tr("Delete failed."), tr("Delete failed."));
        }
        }

    }
}

void mainplatformwindow::on_pushButton_7_clicked() {
    ui->horizontalSlider_3->setValue(ui->horizontalSlider_3->value() - 1);
}

void mainplatformwindow::on_pushButton_8_clicked() {
    ui->horizontalSlider_3->setValue(ui->horizontalSlider_3->value() + 1);
}

void mainplatformwindow::on_plainTextEdit_3_returnPressed() {
    ui->horizontalSlider_3->setValue(ui->plainTextEdit_3->text().toInt());
}
void mainplatformwindow::on_horizontalSlider_3_valueChanged(int value) {
    ui->statusBar->showMessage(tr("Querying..."));
    ui->plainTextEdit_3->setText(QString::number(value));
    fliarrangeRefresh(value);
}




void mainplatformwindow::on_fontComboBox_currentFontChanged(const QFont &f) {
    settings.setValue("Platform/UIFont", f);
    QApplication::setFont(f);
}



void mainplatformwindow::on_tableView_8_clicked(const QModelIndex &index) {
    if(index.isValid()&&index.column()==12) { //Refund
        int row = index.row();
        QAbstractItemModel* model = ui->tableView_8->model();
        QString refund_date = model->data(model->index(row,9)).toString();
        QString seat_id = model->data(model->index(row,11)).toString();
        if(refund_date!="") {
            QMessageBox::critical(this,tr("Error:"),tr("The ticket has already been refunded"));
            return;
        }
        if(seat_id!="") {
            QMessageBox::critical(this,tr("Error:"),tr("The ticket has already been checked in"));
            return;
        }
        QString dep = model->data(model->index(row,3)).toString();
        QStringList list = dep.split("T");
        QString dep_datetime = list[0] + QString(" ")+list[1];
        qDebug()<<dep_datetime;
        QString ticket_ID = model->data(model->index(row,0)).toString();
        QString actual_payment = model->data(model->index(row,6)).toString();
        float money = actual_payment.toFloat();

        float actualRefund = money;
        int TimeDistance = 0;
        QString sql = QString("SELECT UNIX_TIMESTAMP(CAST('%1' AS DATETIME)) - UNIX_TIMESTAMP(NOW())").arg(dep_datetime);
        QSqlQuery query;
        query.exec(sql);
        query.first();
        TimeDistance = query.value(0).toInt();
        if(TimeDistance>=86400) { //距离起飞还有24小时以及更久
            actualRefund *= 0.90;
        } else if(TimeDistance>=7200) { //距离起飞还有2-24小时
            actualRefund *= 0.80;
        } else {
            actualRefund *= 0.65;
        }

        QDate now = QDate::currentDate();
        refund_date = now.toString("yyyy-MM-dd");

        sql = QString("INSERT INTO ticket_refund(`ticket_id`,`refund_date`,`actual refund`) VALUES"
                      "('%1','%2',%3)").arg(ticket_ID).arg(refund_date).arg(actualRefund);


        bool status = query.exec(sql);
        if(status)
            ticketRefresh();
        else
            QMessageBox::critical(this,tr("Refund failed."),tr("Refund failed."));
    } else if(index.isValid()&&index.column()==13) { //Delete
        QMessageBox::StandardButton btn;
        btn = QMessageBox::question(this, tr("himt:"), tr("Are you sure to delete?"), QMessageBox::Yes|QMessageBox::No);
        if (btn == QMessageBox::Yes) {
        int row = index.row();
        QAbstractItemModel* model = ui->tableView_8->model();
        QString refund_date = model->data(model->index(row,9)).toString();
        if(refund_date=="") {
            QMessageBox::critical(this,tr("Error:"),tr("The ticket has not been refunded"));
            return;
        }
        QString ticket_ID = model->data(model->index(row,0)).toString();
        QString sql = QString("DELETE FROM ticket WHERE ticket_id = '%1'").arg(ticket_ID);
        QSqlQuery query;
        bool status = query.exec(sql);
        if(status)
            ticketRefresh();
        else
            QMessageBox::critical(this,tr("Delete failed."),tr("Delete failed."));
        }
    } else if(index.isValid()&&index.column()==14) { //Checkin
        int row = index.row();
        //
        qDebug()<<"你刚刚点击了值机按钮"<<endl; //选择座位等()

        QAbstractItemModel* model = ui->tableView_8->model();
        //        QString seatID =  model->data(model->index(row,7)).toString();
        QString dep_datetime = model->data(model->index(row,3)).toString();
        QString flightID = model->data(model->index(row,2)).toString();
        dep_datetime = dep_datetime.mid(0,10)+" "+dep_datetime.mid(11,8);
        QString classType = model->data(model->index(row,4)).toString();
        QString ticketID = model->data(model->index(row,0)).toString();
        QString order_start = this->ticketOrderStartQuery(ticketID);
        QString order_end = this->ticketOrderEndQuery(ticketID);

        QString seatID = "";
        QString sql_pre = QString("SELECT seat_id FROM ticketVSseatid_view WHERE ticket_id='%1'").arg(ticketID);
        QSqlQuery *query_pre = new QSqlQuery();
        query_pre->exec(sql_pre);
        if(query_pre->next()){
            seatID = query_pre->value(0).toString();
        }

        ticketRefresh();
        if(seatID != "") { //如果完成了值机，则不允许退票
            QMessageBox::information(this,tr("Hint:"),tr("You have checked in. So you cannot Check In AGAIN."));
            return;
        }
        int TimeDistance = 0;
        QString sql = QString("SELECT UNIX_TIMESTAMP(CAST('%1' AS DATETIME)) - UNIX_TIMESTAMP(NOW())").arg(dep_datetime);
        qDebug()<<sql<<endl;
        QSqlQuery *query = new QSqlQuery();
        query->exec(sql);
        query->first();
        TimeDistance = query->value(0).toInt();
        if(TimeDistance <= 900) { //起飞时间距离起飞还有15分钟
            QMessageBox::information(this,tr("Hint:"),tr("The flight is going to take off soon. Check In service have been closed."));
            return;
        }
        //满足值机条件，进入值机界面
        qDebug()<<"满足条件，您即将进入值机页面"<<endl;




        seat_selection *checkIn_interface = new seat_selection(nullptr,model->data(model->index(row,2)).toString(),classType
                                                               ,dep_datetime.mid(0,10)
                                                               ,model->data(model->index(row,1)).toString(),order_start,order_end);
        checkIn_interface->show();
    }
}
QString mainplatformwindow::ticketOrderStartQuery(QString ticketID) {
    QString depID = "0";
    QString sql = QString("SELECT departure_id FROM ticket WHERE ticket_id='%1'").arg(ticketID);
    QSqlQuery *query = new QSqlQuery();
    query->exec(sql);
    if(query->next()) {
        depID = query->value(0).toString();
    }
    return depID;
}

QString mainplatformwindow::ticketOrderEndQuery(QString ticketID) {
    QString arvID = "-1";
    QString sql = QString("SELECT arrival_id FROM ticket WHERE ticket_id='%1'").arg(ticketID);
    QSqlQuery *query = new QSqlQuery();
    query->exec(sql);
    if(query->next()) {
        arvID = query->value(0).toString();
    }
    return arvID;
}

void mainplatformwindow::on_listWidget_8_itemClicked(QListWidgetItem *item) {
    if(item->text() == tr("Add") ) {
        add_ticket = new addticket;
        add_ticket->show();
    }else if(item->text() == tr("Refresh")) {
       ui->horizontalSlider_4->setValue(1);
       on_horizontalSlider_4_valueChanged(1);
    }else if(item->text()==tr("Search")){
        QStringList sqllist = {"ticket_id", "user_id", "flight_id","departure_datetime","class","purchase_date","actual_payment",
                              "departure_airport","arrival_airport","refund_date","actual refund","seat_id"};
        QStringList indexlist = {"Ticket ID", "User ID", "Flight ID","Departure Datetime","Class","Purchase Date",
                                 "Actual Payment","Departure Airport","Arrival Airport","Refund Date","Actual Refund",
                                "Seat ID"};
        QString table = "ticket_all_view";
        q = new QueryDialog(table, sqllist, indexlist);
        q->show();

    }else if(item->text()==tr("Delete")){
        QStringList sqllist = {"ticket_id", "user_id", "flight_id","departure_datetime","class","purchase_date","actual_payment",
                              "departure_airport","arrival_airport","refund_date","actual refund","seat_id"};
        QStringList indexlist = {"Ticket ID", "User ID", "Flight ID","Departure Datetime","Class","Purchase Date",
                                 "Actual Payment","Departure Airport","Arrival Airport","Refund Date","Actual Refund",
                                "Seat ID"};
        QString table = "ticket_all_view";
        d = new deletedialog(table, sqllist, indexlist);
        d->show();
    }else if(item->text()==tr("Modify")){
        QStringList sqllist = {"ticket_id", "user_id", "flight_id","departure_datetime","class","purchase_date","actual_payment",
                              "departure_airport","arrival_airport","refund_date","actual refund","seat_id"};
        QStringList indexlist = {"Ticket ID", "User ID", "Flight ID","Departure Datetime","Class","Purchase Date",
                                 "Actual Payment","Departure Airport","Arrival Airport","Refund Date","Actual Refund",
                                "Seat ID"};
        QString table = "ticket_all_view";

        m = new modifydialog(table, sqllist, indexlist);
        m->show();
    }



}

void mainplatformwindow::on_deliver_clicked()
{
    QString input_data = ui->delivery->toPlainText();
    if(input_data==""){
        QMessageBox::critical(this,tr("Delivery failed."),tr("No input"));
    }
    else{
        QString sql;
        sql = QString("INSERT INTO announcement VALUES('000000000000000000',NOW(),'%1')").arg(input_data);
        QSqlQuery query;
        bool ok = query.exec(sql);
        if(ok){
            QMessageBox::information(this,tr("hint:"),tr("delivery succeed"));
        }
        else{
            QMessageBox::information(this,tr("hint:"),tr("delivery failure"));
        }
    }
}

void mainplatformwindow::on_listWidget_9_itemClicked(QListWidgetItem *item)
{
    if(item->text() == tr("Add") ) {
        add_ann = new addannouncement;
        add_ann->show();
    }else if(item->text() == tr("Refresh")) {
        ui->horizontalSlider_5->setValue(1);
        on_horizontalSlider_5_valueChanged(1);
    }
    else if(item->text()==tr("Search")){
        QStringList sqllist = {"userID", "time", "text"};
        QStringList indexlist = {"User ID", "Delivery Time", "Message"};
        QString table = "announcement";
        q = new QueryDialog(table, sqllist, indexlist);
        q->show();

    }else if(item->text()==tr("Delete")){
        QStringList sqllist = {"userID", "time", "text"};
        QStringList indexlist = {"User ID", "Delivery Time", "Message"};
        QString table = "announcement";

        d = new deletedialog(table, sqllist, indexlist);
        d->show();
    }else if(item->text()==tr("Modify")){
        QStringList sqllist = {"userID", "time", "text"};
        QStringList indexlist = {"User ID", "Delivery Time", "Message"};
        QString table = "announcement";

        m = new modifydialog(table, sqllist, indexlist);
        m->show();
    }


}

void mainplatformwindow::on_tableView_9_clicked(const QModelIndex &index)
{
    if(index.isValid()&&index.column()==3){
        int row = index.row();
        QAbstractItemModel* model = ui->tableView_9->model();
        QString userID = model->data(model->index(row,0)).toString();
        QString time = model->data(model->index(row,1)).toString();
        QStringList list = time.split("T");
        QString datetime = list[0] + QString(" ")+list[1];
        if(userID=="ALL users"){
            userID=QString("000000000000000000");
        }
        mod_ann = new modifyann(userID,datetime);
        mod_ann->show();

    }
    else if(index.isValid()&&index.column()==2){
        int row = index.row();
        QAbstractItemModel* model = ui->tableView_9->model();
        QString userID = model->data(model->index(row,0)).toString();
        QString time = model->data(model->index(row,1)).toString();
        QStringList list = time.split("T");
        QString datetime = list[0] + QString(" ")+list[1];
        if(userID=="ALL users"){
            userID=QString("000000000000000000");
        }
        show_message = new showMessage(userID,datetime);
        show_message->show();
    }
    else if(index.isValid()&&index.column()==4){
        QMessageBox::StandardButton btn;
        btn = QMessageBox::question(this, tr("himt:"), tr("Are you sure to delete?"), QMessageBox::Yes|QMessageBox::No);
        if (btn == QMessageBox::Yes) {
        int row = index.row();
        QAbstractItemModel* model = ui->tableView_9->model();
        QString userID = model->data(model->index(row,0)).toString();
        QString time = model->data(model->index(row,1)).toString();
        QStringList list = time.split("T");
        QString datetime = list[0] + QString(" ")+list[1];
        if(userID=="ALL users"){
            userID=QString("000000000000000000");
        }
        QString sql = QString("DELETE FROM announcement WHERE userID='%1' AND time='%2'").arg(userID).arg(datetime);
        QSqlQuery query;
        bool status = query.exec(sql);
        if(status){
            QMessageBox::information(this,tr("Success"),tr("Delete successfully"));
            annoucementRefresh();
        }
        else
            QMessageBox::critical(this,tr("Delete failed."),tr("Delete failed."));
        }

    }
}
void mainplatformwindow::on_pushButton_9_clicked()
{
    ui->horizontalSlider_4->setValue(ui->horizontalSlider_4->value() - 1);
}

void mainplatformwindow::on_pushButton_10_clicked()
{
    ui->horizontalSlider_4->setValue(ui->horizontalSlider_4->value() + 1);
}

void mainplatformwindow::on_plainTextEdit_4_returnPressed()
{
    ui->horizontalSlider_4->setValue(ui->plainTextEdit_4->text().toInt());
}

void mainplatformwindow::on_horizontalSlider_4_valueChanged(int value)
{
    ui->statusBar->showMessage(tr("Querying..."));
    ui->plainTextEdit_4->setText(QString::number(value));
    ticketRefresh(value);
}

void mainplatformwindow::on_horizontalSlider_5_valueChanged(int value)
{
    ui->statusBar->showMessage(tr("Querying..."));
    ui->plainTextEdit_5->setText(QString::number(value));
    annoucementRefresh(value);
}

void mainplatformwindow::on_pushButton_11_clicked()
{
    ui->horizontalSlider_5->setValue(ui->horizontalSlider_5->value() - 1);
}

void mainplatformwindow::on_plainTextEdit_5_returnPressed()
{
     ui->horizontalSlider_5->setValue(ui->plainTextEdit_5->text().toInt());
}

void mainplatformwindow::on_pushButton_12_clicked()
{
    ui->horizontalSlider_5->setValue(ui->horizontalSlider_5->value() + 1);
}
QMainWindow *sqlmain;
Browser *browser;
void mainplatformwindow::on_pushButton_13_clicked()
{

    sqlmain=new QMainWindow();
    sqlmain->setWindowTitle(QObject::tr("Advanced SQL Management Tool"));

    browser=new Browser(sqlmain);
    sqlmain->setCentralWidget(browser);

    QMenu *fileMenu = sqlmain->menuBar()->addMenu(QObject::tr("&File"));
    fileMenu->addAction(QObject::tr("&Quit"), []() { qApp->quit(); });
    QMenu *helpMenu = sqlmain->menuBar()->addMenu(QObject::tr("&Help"));
    helpMenu->addAction(QObject::tr("About"), [&]() { browser->about(); });
    helpMenu->addAction(QObject::tr("About Qt"), []() { qApp->aboutQt(); });

    QObject::connect(browser, &Browser::statusMessage, [this](const QString &text) {
        sqlmain->statusBar()->showMessage(text);
    });

    sqlmain->show();

}
void mainplatformwindow::on_actionA_bout_triggered(){
    QMessageBox::information(this,tr("About"),tr("This is our project work of Database by 4021,SDS,FDU."));
}

void mainplatformwindow::on_pushButton_14_clicked()
{
    settings.clear();
}
