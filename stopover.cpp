#include "stopover.h"
#include "ui_stopover.h"

stopover::stopover(QWidget *parent,QString flight_id_input) :
    QWidget(parent),
    ui(new Ui::stopover)
{
    ui->setupUi(this);
    flight_id = flight_id_input;
    ui->showflight->setText("STOP OVER OF "+flight_id);
    stop_over = new QSqlTableModel(this);
    stop_over->setTable("airline");
    stop_over->setEditStrategy(QSqlTableModel::OnManualSubmit);
    stop_over->setFilter(QObject::tr("flight_id='%1'").arg(flight_id));
    stop_over->setSort(4,Qt::AscendingOrder);
    stop_over->select();
    stop_over->setHeaderData(0,Qt::Horizontal,QString::fromUtf8(tr("Flight ID").toUtf8()));
    stop_over->setHeaderData(1,Qt::Horizontal,QString::fromUtf8(tr("Airport Code").toUtf8()));
    stop_over->setHeaderData(2,Qt::Horizontal,QString::fromUtf8(tr("Arrival Time").toUtf8()));
    stop_over->setHeaderData(3,Qt::Horizontal,QString::fromUtf8(tr("Depature Time").toUtf8()));
    stop_over->setHeaderData(4,Qt::Horizontal,QString::fromUtf8(tr("Order").toUtf8()));
    ui->tableView->setModel(stop_over);



}

stopover::~stopover()
{
    delete ui;
}

void stopover::on_back_clicked()
{
    this->close();
}

void stopover::on_submit_clicked()
{
    stop_over->database().transaction();
    if(stop_over->submitAll()){
        stop_over->database().commit();
    }
    else{
        stop_over->database().rollback();
        QMessageBox::warning(this,tr("submission failed"),tr("error:%1").arg(stop_over->lastError().text()));
    }
}

void stopover::on_retract_clicked()
{
    stop_over->revertAll();
}

void stopover::on_delete_2_clicked()
{
    int curRow = ui->tableView->currentIndex().row();
    stop_over->removeRow(curRow);
    int ok = QMessageBox::warning(this,tr("Delete the current row!"),tr("Sure?"),
                            QMessageBox::Yes,QMessageBox::No);
       if(ok == QMessageBox::No)
       {
          stop_over->revertAll();
       }
       else stop_over->submitAll();
}

void stopover::on_add_clicked()
{
    int rowNum = stop_over->rowCount();
    stop_over->insertRow(rowNum);
    stop_over->setData(stop_over->index(rowNum,0),flight_id);

}
