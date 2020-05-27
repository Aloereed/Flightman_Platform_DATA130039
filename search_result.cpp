#include "search_result.h"
#include "ui_search_result.h"

search_result::search_result(QString sql,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::search_result)
{
    ui->setupUi(this);
    QSqlQueryModel* search = new QSqlQueryModel;
    search->setQuery(sql);
    ui->tableView->setModel(search);
}

search_result::~search_result()
{
    delete ui;
}

void search_result::on_pushButton_clicked()
{
    QProgressDialog dialog(tr("Returning to the search window"),tr("cancel"), 0, 3000, this);
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
