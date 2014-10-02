#include "odgselectreport.h"
#include "ui_odgselectreport.h"

ODGSelectReport::ODGSelectReport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ODGSelectReport)
{
    ui->setupUi(this);
}

ODGSelectReport::~ODGSelectReport()
{
    delete ui;
}


void ODGSelectReport::showRep(int categoryId, QString dbAddr, QString dbName, int port,  QString name, QString password)
{
    this->id = categoryId;
    this->dbAddr = dbAddr;
    this->dbName = dbName;
    this->port =  port;
    this->name = name;
    this->password = password;
    QStandardItemModel * model = new QStandardItemModel(this);
    QSqlQuery query;
    connect (ui->pushButton,SIGNAL(clicked()),this,SLOT(pressOk()));
    this->show();

    bool ok = query.exec("SELECT report_id, report_descrip, report_name FROM report ORDER BY 2;");
    if (!ok) {
     this->close();
        qDebug()<<query.lastError();
        return;
    }
    qDebug()<<"get report list";
    QList<QStandardItem *> items;
    model->setHorizontalHeaderItem(0, new QStandardItem("id"));
    model->setHorizontalHeaderItem(1,new QStandardItem(QString::fromLocal8Bit("Название отчета")));
     model->setHorizontalHeaderItem(2,new QStandardItem(QString::fromLocal8Bit("Внутреннее имя отчета")));

     while(query.next()){
         items.clear();
        items.append(new QStandardItem(QString::number(query.value(0).toInt())));
        items.append(new QStandardItem(query.value(1).toString()));
        items.append(new QStandardItem(query.value(2).toString()));
        items.at(1)->setEditable(false);
        model->appendRow(items);
    }
    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(1,400);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(2);
    qDebug()<<"column width";
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);


    connect (ui->tableView, SIGNAL(doubleClicked(QModelIndex)),this, SLOT(pressOk()));
}
void ODGSelectReport::pressOk(){

    QItemSelectionModel * mdl = ui->tableView->selectionModel();
    qDebug()<<"Push has model";
    QModelIndexList selList = mdl->selectedRows();
    if (selList.size() > 0){
           qDebug()<<"Sel size greater then null";
       QStandardItemModel *models = (QStandardItemModel *)ui->tableView->model();
        QString filename = models->item(selList.at(0).row(),2)->text();

        if (OdgORPTGw::startRPTRender("psql://"+dbAddr+"/"+dbName+":"+QString::number(port),
                                  name,password,filename,
                                      "-param=categ_id:integer="+QString::number(id)) == -1){
            QMessageBox::warning(this, QString::fromLocal8Bit("Не удается вывести отчет"),
                                 QString::fromLocal8Bit("Не удается вывести отчет. Повторите попытку позже. Если повторные попытки будут неудачными, обратитесь к администратору."));
        }
    }
    this->close();
}
