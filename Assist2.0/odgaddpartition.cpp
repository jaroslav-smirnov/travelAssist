#include "odgaddpartition.h"
#include "ui_odgaddpartition.h"

odgAddPartition::odgAddPartition(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::odgAddPartition)
{
    ui->setupUi(this);

    QStandardItemModel * model = new QStandardItemModel(this);
    model->setHorizontalHeaderItem(0, new QStandardItem(QString::fromLocal8Bit("Тип раздела")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString::fromLocal8Bit("id")));

    QSqlQuery query;

    bool ok = query.exec("SELECT type_name, id FROM part_types ORDER BY type_name;");
    if (!ok){
        qDebug()<<query.lastError();
        return;
    }
    QList<QStandardItem *> items;
    while (query.next()){
        items.clear();
        items.append(new QStandardItem(query.value(0).toString()));
        items.append(new QStandardItem(query.value(1).toString()));
        for (int i=0; i < items.size(); i++){
            items.at(i)->setEditable(false);
        }
        model->appendRow(items);
    }
    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->hideColumn(1);
    ui->tableView->resizeColumnsToContents();
    connect(ui->tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(selectRow(QModelIndex)));
    connect(ui->pushButton,SIGNAL(clicked()), this, SLOT(addPartition()));
    ui->pushButton->setEnabled(false);
}

odgAddPartition::~odgAddPartition()
{
    delete ui;
}
void odgAddPartition::selectRow(QModelIndex index){

    ui->pushButton->setEnabled(true);
    id = ((QStandardItemModel *)ui->tableView->model())->item(index.row(),1)->text().toInt();

}
void odgAddPartition::addPartition(){

    QSqlQuery query;
    bool ok = query.
            exec(QString("INSERT INTO partitions (add_date, part_name, part_type) VALUES (current_timestamp, '%1', %2);")
                 .arg(ui->lineEdit->text()).arg(QString::number(id)));

    emit(newPartAdd());
    this->close();
}
