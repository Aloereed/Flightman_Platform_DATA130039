#include "add_price.h"
#include "ui_add_price.h"

add_price::add_price(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_price)
{
    ui->setupUi(this);
}

add_price::add_price(std::vector<std::vector<QString>> whole_price, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_price)
{
    ui->setupUi(this);
    ui->label->setText(tr("Set the price"));
    price = new QStandardItemModel(0, 4, this);
    theselection = new QItemSelectionModel(price);
    //connect(theselection,SIGNAL(currentChanged(QModelIndex,QModelIndex)),this,SLOT(on_currentChanged(QModelIndex,QModelIndex)));
    ui->tableView->setModel(price);
    ui->tableView->setSelectionModel(theselection);
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    QStringList headerList = {tr("start airport"), tr("arrival airport"), tr("class"), tr("price")};
    price->setHorizontalHeaderLabels(headerList);
    ui->tableView->resizeColumnsToContents();

    //price_vector=whole_price;//Another way of emit the price vector, but i am concerned that it will be polluted by the user;
    for(int i = 0; i < whole_price.size(); i++)
    {
        int count = ui->tableView->model()->rowCount();
        ui->tableView->model()->insertRow(count);
        ui->tableView->model()->setData(ui->tableView->model()->index(count, 0), whole_price[i][0]);
        ui->tableView->model()->setData(ui->tableView->model()->index(count, 1), whole_price[i][1]);
        ui->tableView->model()->setData(ui->tableView->model()->index(count, 2), whole_price[i][2]);

        if(whole_price[i][3] != "NULL")
        {
            ui->tableView->model()->setData(ui->tableView->model()->index(count, 2), whole_price[i][3]);
        }
    }

}
add_price::add_price(QStringList airportlist, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_price)
{
    ui->setupUi(this);
    ui->label->setText(tr("Set the price"));
    price = new QStandardItemModel(0, 4, this);
    theselection = new QItemSelectionModel(price);
    //connect(theselection,SIGNAL(currentChanged(QModelIndex,QModelIndex)),this,SLOT(on_currentChanged(QModelIndex,QModelIndex)));
    ui->tableView->setModel(price);
    ui->tableView->setSelectionModel(theselection);
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    QStringList headerList = {tr("start_airport"), tr("arrival_airport"), tr("class"), tr("price")};
    price->setHorizontalHeaderLabels(headerList);
    ui->tableView->resizeColumnsToContents();

    for(int i = 0; i < airportlist.count(); i++)
    {
        for(int j = i + 1; j < airportlist.count(); j++)
        {
            int count = ui->tableView->model()->rowCount();
            ui->tableView->model()->insertRow(count);
            ui->tableView->model()->setData(ui->tableView->model()->index(count, 0), airportlist[i]);
            ui->tableView->model()->setData(ui->tableView->model()->index(count, 1), airportlist[j]);
            ui->tableView->model()->setData(ui->tableView->model()->index(count, 2), "business");
            count++;
            ui->tableView->model()->insertRow(count);
            ui->tableView->model()->setData(ui->tableView->model()->index(count, 0), airportlist[i]);
            ui->tableView->model()->setData(ui->tableView->model()->index(count, 1), airportlist[j]);
            ui->tableView->model()->setData(ui->tableView->model()->index(count, 2), "economy");
        }
    }

}
add_price::~add_price()
{
    delete ui;
}

void add_price::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->button(QDialogButtonBox::Ok) == button)
    {
        std::vector<std::vector<QString>> whole_price;
        std::vector<QString> row;
        int count = ui->tableView->model()->rowCount();

        for(int i = 0; i < count; i++)
        {
            if(i == 0)
            {
                for(int k = 0; k < 4; k++)
                {
                    QModelIndex index = ui->tableView->model()->index(i, k);
                    QString temp = ui->tableView->model()->data(index).toString();
                    row.push_back(temp);

                    if(k == 3)
                    {
                        bool ok;
                        temp.toInt(&ok);

                        if(!ok)
                        {
                            QMessageBox::information(this, tr("Failure"), tr("The price hasn't be set up properly"));
                            return;
                        }
                    }
                }
            }
            else
            {
                for(int k = 0; k < 4; k++)
                {
                    QModelIndex index = ui->tableView->model()->index(i, k);
                    QString temp = ui->tableView->model()->data(index).toString();
                    row[k] = temp;

                    if(k == 3)
                    {
                        bool ok;
                        temp.toInt(&ok);

                        if(!ok)
                        {
                            QMessageBox::information(this, tr("Failure"), tr("The price hasn't be set up properly"));
                            return;
                        }
                    }
                }
            }

            whole_price.push_back(row);
        }

        emit sendprice(whole_price);
        this->close();

    }
    else if(ui->buttonBox->button(QDialogButtonBox::Cancel) == button)
    {
        QProgressDialog dialog(tr("Returning to the previous window"), tr("cancel"), 0, 3000, this);
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
