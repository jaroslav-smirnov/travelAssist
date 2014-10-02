#include "selectitems.h"
#include "ui_selectitems.h"

SelectItems::SelectItems(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectItems)
{
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(pushOk()));
}

SelectItems::~SelectItems()
{
    delete ui;
}

void SelectItems::setItemsFromString(QString str){
    QStringList lst = str.split("\r\n");
    QStandardItemModel * model = new QStandardItemModel(this);
    model->setHorizontalHeaderItem(0,new QStandardItem(QString::fromLocal8Bit("Дата и время")));
    model->setHorizontalHeaderItem(1,new QStandardItem(QString::fromLocal8Bit("Имя")));
    model->setHorizontalHeaderItem(2,new QStandardItem(QString::fromLocal8Bit("Имя файла")));
      model->setHorizontalHeaderItem(3,new QStandardItem(QString::fromLocal8Bit("Номер устройства")));
    QList<QStandardItem *> row;
    for (int i = 0; i < lst.length(); i++){
        row.clear();
        QStringList lst2 = lst.at(i).split(";");
        for (int j = 0; j < 3; j++){
            if (lst2.size() < 4) continue;
            QStandardItem *itm = new QStandardItem(lst2.at(j+1));
            itm->setEditable( false);
            row.append(itm);
}
        model->appendRow(row);
    }

    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->hideColumn(2);
    ui->tableView->hideColumn(3);
    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(pushOk()));
}
void SelectItems::pushOk(){
    qDebug()<<"Push ok";
    QString filename;
    QItemSelectionModel * mdl = ui->tableView->selectionModel();
    qDebug()<<"Push has model";
    QModelIndexList selList = mdl->selectedRows();
    if (selList.size() > 0){
       QStandardItemModel *models = (QStandardItemModel *)ui->tableView->model();
        filename = models->item(selList.at(0).row(),2)->text();
        qDebug()<<"Emit signal";
        emit(haveDataName(filename));
    }

    else if (selList.size() > 1){
        QStringList names;
        QStandardItemModel *models = (QStandardItemModel *)ui->tableView->model();
        for (int i = 0; i < selList.count(); i++){
            names.append(models->item(selList.at(0).row(),2)->text());
        }
        emit haveSeveralNames(names);
    }

    this->close();
}
