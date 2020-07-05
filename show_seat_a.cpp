#include "show_seat_a.h"
#include "ui_show_seat_a.h"

show_seat_a::show_seat_a(QString tablename_input, QString departure_date_input, QString flight_id_input, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::show_seat_a)
{
    ui->setupUi(this);
    tablename=tablename_input;
    departure_date=departure_date_input;
    flight_id=flight_id_input;

    seat=new QSqlQueryModel(this);
    QString sql = QString("SELECT * FROM ")+tablename+QString(" WHERE flight_id=\'")+flight_id +
            QString("\' AND departure_date=\'")+departure_date+QString("\'");
    seat->setQuery(sql);

    QString title;
    if(tablename=="seat_amount"){
        title="Seat Amount of ";
        seat->setHeaderData(0,Qt::Horizontal,QString::fromUtf8(tr("Flight ID").toUtf8()));
        seat->setHeaderData(1,Qt::Horizontal,QString::fromUtf8(tr("Order").toUtf8()));
        seat->setHeaderData(2,Qt::Horizontal,QString::fromUtf8(tr("Departure Date").toUtf8()));
        seat->setHeaderData(3,Qt::Horizontal,QString::fromUtf8(tr("Type").toUtf8()));
        seat->setHeaderData(4,Qt::Horizontal,QString::fromUtf8(tr("Seats left").toUtf8()));

    }
    else{
        title="Seat Arrangement of ";
        seat->setHeaderData(0,Qt::Horizontal,QString::fromUtf8(tr("Flight ID").toUtf8()));
        seat->setHeaderData(1,Qt::Horizontal,QString::fromUtf8(tr("Order").toUtf8()));
        seat->setHeaderData(2,Qt::Horizontal,QString::fromUtf8(tr("Departure Date").toUtf8()));
        seat->setHeaderData(3,Qt::Horizontal,QString::fromUtf8(tr("Seat ID").toUtf8()));
        seat->setHeaderData(4,Qt::Horizontal,QString::fromUtf8(tr("User Who Bought It").toUtf8()));
    }
    title+= flight_id +" on " + departure_date;
    ui->label->setText(title);

    ui->tableView->setModel(seat);
    ui->tableView->show();
}

show_seat_a::~show_seat_a()
{
    delete ui;
}
