#include "addticket.h"
#include "ui_addticket.h"
usercheck *user_check;

addticket::addticket(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addticket)
{
    ui->setupUi(this);
    ticket = ui->tableView;
    add_ticket=new addticketmodel;
}

addticket::~addticket()
{
    delete ui;
}

void addticket::on_pushButton_clicked()
{
    QString flight_id = ui->flight_id->text();
    QString year = ui->year->text();
    QString month = ui->month->text();
    QString day = ui->day->text();
    QString departure_airport = ui->departure_airport->text();
    QString arrival_airport = ui->arrival_airport->text();

        QString sql = QString("select * from `addticket_all_view` WHERE `departure_date` Like \'");
        if(year!=""){
            sql+=year+QString("-");
        }
        else{
            sql+=QString("%-");
        }
        if(month!=""){
            if(month.length()<2){
                sql+=QString("0")+month+QString("-");
            }
            else
                sql+=month+QString("-");
        }
        else{
            sql+=QString("%-");
        }
        if(day!=""){
            if(day.length()<2){
                sql+=QString("0")+day+QString("' ");
            }
            else
                sql+=day+QString("' ");
        }
        else{
            sql+=QString("%' ");
        }

        if(flight_id!=""){
            sql += QString ("AND `flight_id` LIKE '%%1%' ").arg(flight_id);
        }
        if(departure_airport!=""){
            sql += QString ("AND `departure_city` = '%1' ").arg(departure_airport);
        }
        if(arrival_airport!=""){
            sql += QString ("AND `arrival_city` = '%1' ").arg(arrival_airport);
        }
        qDebug()<<sql;


        add_ticket->setQuery(sql);
        ticket->setModel(add_ticket);
        add_ticket->insertColumn(14);
        add_ticket->setHeaderData(14, Qt::Horizontal, QString::fromUtf8(tr("Book").toUtf8()));


        add_ticket->setHeaderData(0, Qt::Horizontal, QString::fromUtf8(tr("Flight ID").toUtf8()));
        add_ticket->setHeaderData(1, Qt::Horizontal, QString::fromUtf8(tr("Departure Date").toUtf8()));
        add_ticket->setHeaderData(2, Qt::Horizontal, QString::fromUtf8(tr("Departure Airport ID").toUtf8()));
        add_ticket->setHeaderData(3, Qt::Horizontal, QString::fromUtf8(tr("Departure Airport").toUtf8()));
        add_ticket->setHeaderData(4, Qt::Horizontal, QString::fromUtf8(tr("Departure City").toUtf8()));
        add_ticket->setHeaderData(5, Qt::Horizontal, QString::fromUtf8(tr("Departure Time").toUtf8()));
        add_ticket->setHeaderData(6, Qt::Horizontal, QString::fromUtf8(tr("Arrival Airport ID").toUtf8()));
        add_ticket->setHeaderData(7, Qt::Horizontal, QString::fromUtf8(tr("Arrival Airport").toUtf8()));
        add_ticket->setHeaderData(8, Qt::Horizontal, QString::fromUtf8(tr("Arrival City").toUtf8()));
        add_ticket->setHeaderData(9, Qt::Horizontal, QString::fromUtf8(tr("Arrival Time").toUtf8()));
        add_ticket->setHeaderData(10, Qt::Horizontal, QString::fromUtf8(tr("Company Name").toUtf8()));
        add_ticket->setHeaderData(11, Qt::Horizontal, QString::fromUtf8(tr("Company ID").toUtf8()));
        add_ticket->setHeaderData(12, Qt::Horizontal, QString::fromUtf8(tr("Class").toUtf8()));
        add_ticket->setHeaderData(13, Qt::Horizontal, QString::fromUtf8(tr("Price").toUtf8()));



}

QVariant addticketmodel::data(const QModelIndex &item, int role) const {
    QVariant value = QSqlQueryModel::data(item, role);
    if (role == Qt::BackgroundColorRole){
        if(item.column()==14)
            return QVariant::fromValue(QColor(225,225,225));
    }
    if (role == Qt::DisplayRole){
        if(item.column()==14)
            return QVariant::fromValue(tr("Book"));

    }
    return value;
}

void addticket::on_pushButton_2_clicked()
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

QString addticket::getRandomString(int length)
{
    qsrand(QDateTime::currentMSecsSinceEpoch());//为随机值设定一个seed

    //const char chrs[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const char chrs[] = "0123456789";
    int chrs_size = sizeof(chrs);

    char* ch = new char[length + 1];
    memset(ch, 0, length + 1);
    int randomx = 0;
    for (int i = 0; i < length; ++i)
    {
        randomx= rand() % (chrs_size - 1);
        ch[i] = chrs[randomx];
    }

    QString ret(ch);
    delete[] ch;
    return ret;
}


void addticket::on_tableView_clicked(const QModelIndex &index)
{
    if(index.isValid()&&index.column()==14){//book
        int row = index.row();
        QAbstractItemModel* model = ui->tableView->model();
        QString class_name = model->data(model->index(row,12)).toString();
        QString class_;
        if(class_name=="economy")
            class_ = "1";
        else {
            class_="0";
        }
        QString flightid = model->data(model->index(row,0)).toString();
        QString departure_date = model->data(model->index(row,1)).toString();
        QString departure_air = model->data(model->index(row,2)).toString();
        QString departure_time = model->data(model->index(row,5)).toString();
        QString departure_datetime = departure_date + QString(" ")+ departure_time;
        QString arrival_air = model->data(model->index(row,6)).toString();
        QString companyID = model->data(model->index(row,11)).toString();

        QString ticket_ID = companyID+getRandomString(11);

        user_check = new usercheck(flightid,departure_air,arrival_air,class_,departure_date,ticket_ID,departure_datetime);
        user_check->show();
    }
    else if(index.isValid()&&index.column()==13){//business
        int row = index.row();
        QAbstractItemModel* model = ui->tableView->model();
        QString class_ = "0";
        QString flightid = model->data(model->index(row,0)).toString();
        QString departure_date = model->data(model->index(row,1)).toString();
        QString departure_air = model->data(model->index(row,2)).toString();
        QString departure_time = model->data(model->index(row,5)).toString();
        QString departure_datetime = departure_date + QString(" ")+ departure_time;
        QString arrival_air = model->data(model->index(row,6)).toString();
        QString companyID = model->data(model->index(row,11)).toString();

        QString ticket_ID = companyID+getRandomString(11);

        user_check = new usercheck(flightid,departure_air,arrival_air,class_,departure_date,ticket_ID,departure_datetime);
        user_check->show();
    }
}
