#include "odgaddcategory.h"
#include "ui_odgaddcategory.h"

ODGAddCategory::ODGAddCategory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ODGAddCategory)
{
    ui->setupUi(this);
}

void ODGAddCategory::setPartition(int partId)
{
    this->partId = partId;

    QStandardItemModel * model = new QStandardItemModel(this);
    model->setHorizontalHeaderItem(0, new QStandardItem(QString::fromLocal8Bit("Опрерация/категория")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString::fromLocal8Bit("id")));

    QSqlQuery query;

    bool ok = query.exec("SELECT c_name, id FROM cat_types ORDER BY c_name;");
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
    connect(ui->pushButton,SIGNAL(clicked()), this, SLOT(addCat()));
    ui->pushButton->setEnabled(false);

}

ODGAddCategory::~ODGAddCategory()
{
    delete ui;
}

void ODGAddCategory::selectRow(QModelIndex index){
    ui->pushButton->setEnabled(true);
    id = ((QStandardItemModel *)ui->tableView->model())->item(index.row(),1)->text().toInt();
}
void ODGAddCategory::addCat(){
    qDebug()<<"Add cat";
    QSqlQuery query;
    bool ok = query.
            exec(QString("INSERT INTO categories"
                         " (add_date, cat_type, cat_desc,parent_part) "
                         " VALUES (current_timestamp, %1, '%2',%3);")
                 .arg(QString::number(id))
            .arg(ui->lineEdit->text())
            .arg(QString::number(this->partId)));

    if (!ok) qDebug()<<query.lastError();
    emit(newCatAdd(partId));
    this->close();
}
