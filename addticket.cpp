#include "addticket.h"
#include "ui_addticket.h"

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

        QString sql = QString("select `airline_arrangement_view`.`flight_id`,`departure_date`,`departure_airport_id`,`departure_airport`,`departure_city`,`departure_time`,"
                              "`arrival_airport_id`,`arrival_airport`,`arrival_city`,`arrival_time` from `airline_arrangement_view` INNER JOIN `flight_arrangement` ON "
                              "`airline_arrangement_view`.flight_id=`flight_arrangement`.flight_id WHERE `departure_date` Like \'");
        if(year!=""){
            sql+=year+QString("-");
        }
        else{
            sql+=QString("%-");
        }
        if(month!=""){
            sql+=month+QString("-");
        }
        else{
            sql+=QString("%-");
        }
        if(day!=""){
            sql+=day+QString("' ");
        }
        else{
            sql+=QString("%' ");
        }

        if(flight_id!=""){
            sql += QString ("AND `airline_arrangement_view`.`flight_id` = '%1' ").arg(flight_id);
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
        add_ticket->insertColumn(10);
        add_ticket->setHeaderData(10, Qt::Horizontal, QString::fromUtf8(tr("Economy").toUtf8()));
        add_ticket->insertColumn(11);
        add_ticket->setHeaderData(11, Qt::Horizontal, QString::fromUtf8(tr("Business").toUtf8()));

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



}

QVariant addticketmodel::data(const QModelIndex &item, int role) const {
    QVariant value = QSqlQueryModel::data(item, role);
    if (role == Qt::BackgroundColorRole){
        if(item.column()==10||item.column()==11)
            return QVariant::fromValue(QColor(225,225,225));
    }
    if (role == Qt::DisplayRole){
        if(item.column()==10)
            return QVariant::fromValue(tr("Economy"));
        if(item.column()==11)
            return QVariant::fromValue(tr("Business"));
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

void addticket::on_tableView_clicked(const QModelIndex &index)
{
    if(index.isValid()&&index.column()==8){//Refund
        int row = index.row();
        QAbstractItemModel* model = ui->tableView->model();
        QString refund_date = model->data(model->index(row,9)).toString();


        QString ticket_ID = model->data(model->index(row,0)).toString();
        QString actual_payment = model->data(model->index(row,6)).toString();


//        QString sql = QString("INSERT INTO ticket_refund(`ticket_id`,`refund_date`,`actual refund`) VALUES"
//                              "('%1','%2',%3)").arg(ticket_ID).arg(refund_date).arg(money);

//        QSqlQuery query;
//        bool status = query.exec(sql);
//        if(status)
//            ticketRefresh();
//        else
//            QMessageBox::critical(this,tr("Refund failed."),tr("Refund failed."));
    }
}
