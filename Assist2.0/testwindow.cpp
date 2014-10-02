#include "testwindow.h"
#include "ui_testwindow.h"

TestWindow::TestWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TestWindow)
{
    ui->setupUi(this);
current_cat_id = 0;
current_part_id = 0;
ui->pushButton_2->setEnabled(false);
ui->action_7->setEnabled(false);
ui->action_2->setEnabled(false);
connect(ui->action_4, SIGNAL(triggered()), this, SLOT(close()));
connect(ui->action_7, SIGNAL(triggered()), this, SLOT(showReport()));
connect (ui->action,SIGNAL(triggered()), this, SLOT(addPartition()));
connect (ui->action_2, SIGNAL(triggered()), this, SLOT(addCategorie()));
connect(ui->action_3,SIGNAL(triggered()),this,SLOT(startBatchAdding()));
connect (ui->pushButton_3, SIGNAL(clicked()), this, SLOT(fromUSB()));
connect (ui->pushButton_net, SIGNAL(clicked()),this,SLOT(askerGetDataToUs()));
connect (ui->pushButton_sweepList,SIGNAL(clicked()),this,SLOT(askerGetListToUs()));
connect (ui->pushButton_4,SIGNAL(clicked()),this,SLOT(acceptChanges()));

connect(ui->tableView_2, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customMenuRequested(QPoint)));
connect(ui->tableView_3, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(askerGetListToUs()));
connect(ui->tableView_part, SIGNAL(clicked(QModelIndex)), this, SLOT(selectRowFromPart(QModelIndex)));
connect(ui->tableView_cat, SIGNAL(clicked(QModelIndex)), this, SLOT(selectRowFromCat(QModelIndex)));


this->showMaximized();

    db = QSqlDatabase::addDatabase("QPSQL");


    QSettings settings("config.ini",QSettings::IniFormat);

ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    db = QSqlDatabase::addDatabase("QPSQL"); /**< Set database parameter to PostgreSQL */
    db.setHostName(settings.value("host",QVariant("192.168.0.238")).toString());
    db.setPort(settings.value("port",QVariant(5432)).toInt());
    db.setDatabaseName(settings.value("db",QVariant("factory")).toString());
//db.setDatabaseName(settings.value("db",QVariant("odgControl")).toString());
    db.setUserName(settings.value("user_name",QVariant("head")).toString());
    db.setPassword(settings.value("password_opened",QVariant("0000")).toString()); /**< Set password to *** */



    bool ok = db.open();
    if (!ok)
    {
        qDebug()<<"We have a problem "<<db.lastError();
    }

    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(addData()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(findData()));
    viewPartitions();

    asker= new ODGBroadCastAsker(this);
   initUserView();
    connect(asker, SIGNAL(nameAdded(int,QString,QString)), this, SLOT(askerNewUser(int,QString,QString)));
    connect(asker,SIGNAL(nameChanged(int,QString)), this, SLOT(askerChngUser(int,QString)));
    connect(asker, SIGNAL(nameDeleted(int)), this, SLOT(askerDelUser(int)));
    connect(asker,SIGNAL(dataGot(QString)), this,SLOT(askerNewData(QString)));

}

void TestWindow::askerNewData(QString data){
    qDebug()<<"Here is asker with new data";
   if (!this->isActiveWindow()) return;
    if (data.contains("<<odgdata>>") && data.contains("<<stopodgdata>>")){
        data.remove("<<odgdata>>");
        data.remove("<<stopodgdata>>");
        OdgXmlParser *parser = new OdgXmlParser(this);
        parser->xml2DOM(data);
        workData(parser);
}
    else if (data.contains("<<odgdata>>")){
        data.remove("<<odgdata>>");
        bufferData.clear();
        bufferData.append(data);
    }
    else if (data.contains("<<stopodgdata>>")){
        data.remove("<<stopodgdata>>");
        OdgXmlParser *parser = new OdgXmlParser(this);
        parser->xml2DOM(bufferData.append(data));
        workData(parser);
    }
    else if (data.contains("<<listdata>>") && data.contains("<<endlistdata>>")){
       data =  data.remove("<<listdata>>");
       data = data.remove("<<endlistdata>>");
        SelectItems *selItems = new SelectItems(this);
        selItems->setItemsFromString(data);


        connect(selItems,SIGNAL(haveDataName(QString)),this,SLOT(askConcreteData(QString)));
        connect(selItems,SIGNAL(haveSeveralNames(QStringList)),this,SLOT(askListOfData(QStringList)));

                                     //If windows is opened
        selItems->show();


    }
    else{
     bufferData.append(data);
    }
}

void TestWindow::askListOfData(QStringList datalist){
    qDebug()<<datalist;
}

void TestWindow::askConcreteData(QString data){
    qDebug()<<"Get signal";
    QItemSelectionModel * selModel = ui->tableView_3->selectionModel();
    if (selModel->selection().count()>0){
        int i = selModel->currentIndex().row();
      asker->getConcreteData(i,data);
    }
}

void TestWindow::askerGetDataToUs(){
    //QStandardItemModel * model = (QStandardItemModel *)ui->tableView_3->model();
    QItemSelectionModel * selModel = ui->tableView_3->selectionModel();
    if (selModel->selection().count()>0){
        int i = selModel->currentIndex().row();
      asker->getData(i);
    }
}




void TestWindow::askerGetListToUs(){
    //QStandardItemModel * model = (QStandardItemModel *)ui->tableView_3->model();
    QItemSelectionModel * selModel = ui->tableView_3->selectionModel();
    if (selModel->selection().count()>0){
        int i = selModel->currentIndex().row();
        qDebug()<<"Get list";
        asker->getList(i);
    }
}



void TestWindow::initUserView(){
    QStandardItemModel * model = new QStandardItemModel(this);
    model->setHorizontalHeaderItem(0, new QStandardItem(QString::fromLocal8Bit("Имя")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString::fromLocal8Bit("Ip")));

    ui->tableView_3->setModel(model);
    ui->tableView_3->resizeRowsToContents();
    ui->tableView_3->hideColumn(0);
ui->tableView_3->setSelectionBehavior(QAbstractItemView::SelectRows);
ui->tableView_3->setSelectionMode(QAbstractItemView::SingleSelection);

}
void TestWindow::askerNewUser(int i, QString name, QString ip){

    QStandardItemModel * model;

    model = (QStandardItemModel *) ui->tableView_3->model();
    QString ourName = name;
    QString ourIp = ip;
QStandardItem *itm = new QStandardItem(ourName);
model->setItem(i,0,itm);
itm = new QStandardItem(ourIp);
itm->setEditable(false);
        model->setItem(i,1,itm);
        }

void TestWindow::askerChngUser(int i, QString name){
    QStandardItemModel * model;
    model = (QStandardItemModel *) ui->tableView_3->model();
    model->setItem(i,0,new QStandardItem(name));

}

void TestWindow::askerDelUser(int i){
    QStandardItemModel * model;
    model = (QStandardItemModel *) ui->tableView_3->model();
    model->removeRow(i);
}

TestWindow::~TestWindow()
{
    delete ui;
}
\
void TestWindow::addPartition()
{
    odgAddPartition *partition = new odgAddPartition(this);
    partition->show();
    connect(partition,SIGNAL(newPartAdd()), this, SLOT(viewPartitions()));
}
void TestWindow::addCategorie()
{
    ODGAddCategory *category = new ODGAddCategory(this);
    category->setPartition(this->current_part_id);
    category->show();

    connect(category,SIGNAL(newCatAdd(int)), this, SLOT(viewCategories(int)));

}

void TestWindow::viewPartitions(){
    QSqlQuery query;
    QString queryStr;
    queryStr = "SELECT partitions.part_name, part_types.type_name, partitions.add_date, partitions.id "
            "FROM "
            "partitions "
            "INNER JOIN "
            "part_types "
            "ON "
            "partitions.part_type = part_types.id "
            "ORDER BY partitions.add_date DESC;";
    bool ok = query.exec(queryStr);
    if(!ok) {
        qDebug()<<query.lastError();
        return;
    }
    QStandardItemModel * model = new QStandardItemModel(this);
    model->setHorizontalHeaderItem(0, new QStandardItem(QString::fromLocal8Bit("Идентификатор")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString::fromLocal8Bit("Дата добавления")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString::fromLocal8Bit("Тип")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString::fromLocal8Bit("id")));

    QList<QStandardItem *> itemList;
    while (query.next()){
        itemList.clear();
        itemList.append(new QStandardItem(query.value(0).toString()));
        itemList.append(new QStandardItem(query.value(2).toString().replace("T", " ")));
        itemList.append(new QStandardItem(query.value(1).toString()));

        itemList.append(new QStandardItem(query.value(3).toString()));
        model->appendRow(itemList);
    }

    ui->tableView_part->setModel(model);
    ui->tableView_part->resizeColumnToContents(0);
    ui->tableView_part->resizeColumnToContents(1);
    ui->tableView_part->hideColumn(3);
    ui->tableView_part->resizeRowsToContents();
ui->tableView_part->setSelectionBehavior(QAbstractItemView::SelectRows);
ui->tableView_part->setSelectionMode(QAbstractItemView::SingleSelection);
}

void TestWindow::viewCategories(int id)
{
    QSqlQuery query;
    QString queryStr;
    queryStr = "SELECT categories.add_date, cat_types.c_name, cat_desc, categories.id "
            "FROM "
            "categories "
            "INNER JOIN "
           " cat_types "
            "ON "
            "categories.cat_type = cat_types.id "
           " WHERE categories.parent_part = %1 "
            "ORDER BY categories.add_date DESC;";

    bool ok = query.exec(queryStr.arg(QString::number(id)));
    if(!ok) {
        qDebug()<<query.lastError();
        return;
    }
    QStandardItemModel * model = new QStandardItemModel(this);
    model->setHorizontalHeaderItem(1, new QStandardItem(QString::fromLocal8Bit("Тип операции")));
    model->setHorizontalHeaderItem(0, new QStandardItem(QString::fromLocal8Bit("Дата добавления")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString::fromLocal8Bit("Описание")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString::fromLocal8Bit("id")));

    QList<QStandardItem *> itemList;
    while (query.next()){
        itemList.clear();
        itemList.append(new QStandardItem(query.value(0).toString().replace("T", " ")));
        itemList.append(new QStandardItem(query.value(1).toString()));
        itemList.append(new QStandardItem(query.value(2).toString()));
        itemList.append(new QStandardItem(query.value(3).toString()));
        model->appendRow(itemList);
    }

    ui->tableView_cat->setModel(model);
    ui->tableView_cat->resizeColumnToContents(0);
    ui->tableView_cat->resizeRowsToContents();
    ui->tableView_cat->hideColumn(3);
    ui->tableView_cat->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_cat->setSelectionMode(QAbstractItemView::SingleSelection);


}
void TestWindow::addData(){
    ODGConfDialog *dlg = new ODGConfDialog(this);
    dlg->show();
    connect (dlg,SIGNAL(confirmed(QString *)), this, SLOT(confirmlyAdd(QString *)));
}

void TestWindow::confirmlyAdd(QString * who){
    //qDebug()<<*who->trimmed();
    QString whoAdd = *who;
    delete who;
    QStandardItemModel *model = (QStandardItemModel *)ui->tableView->model();
bool dublicate = false;
    /*Create new sweep*/
    QSqlQuery query;
    QString queryStr = "INSERT INTO \"SWEEPS\" "
            "(\"date\",\"time\",who_take,who_make, category_id) "
            "VALUES ('%1','%2','%3','%4',%5) "
            "RETURNING id; ";
    bool ok = query.exec(queryStr.arg(QDate::currentDate().toString("dd.MM.yyyy"))
                         .arg(QTime::currentTime().toString("hh:mm:ss"))
                         .arg(whoAdd)
                         .arg(wName)
                         .arg(this->current_cat_id));
    if (!ok)
    {
        qDebug()<<query.lastError();
        return;
    }
    query.next();
    int currentSweepId = query.value(0).toInt();
    qDebug()<<currentSweepId;
    QString dateAndTime;

    QString queryTemplate = "INSERT INTO points (\"number\",\"date\",\"time\", \"values\", "
                                                "defect,description, is_defect, values_number, dimensions, "
                                                "generation, sweep_id, division,\"humanNames\") "
                                                "VALUES (%1,'%2','%3','%4','%5', "
                                                "'%6',%7,%8,'%9',%10,%11,%12,'%13'); ";
QString tempQueryTemplate;
    for (int i = 0; i < model->rowCount(); i++){
        dateAndTime = ((QStandardItem *)model->item(i,0))->text();

tempQueryTemplate = queryTemplate;
QString temporaryBool;
if (((QStandardItem *)model->item(i,8))->checkState()==Qt::Checked)
    temporaryBool = "true";
else temporaryBool = "false";

QString tempStr = ((QStandardItem *)model->item(i,5))->text().replace("  "," ");
QStringList items = tempStr.split(" ");
double number = 0;
QString dms;
if (items.size() == 2){
    number = items.at(0).toDouble();
    dms = items.at(1);
}

tempQueryTemplate = tempQueryTemplate
                  .arg(((QStandardItem *)model->item(i,3))->text())
                  .arg(dateAndTime.split(",").at(0))
                  .arg(dateAndTime.split(",").at(1))
                  .arg(((QStandardItem *)model->item(i,5))->text())
                  .arg(((QStandardItem *)model->item(i,6))->text())
                  .arg(((QStandardItem *)model->item(i,7))->text())
                  .arg(temporaryBool)   ///Thi s is flag is alarmed
                  //.arg(((QStandardItem *)model->item(i,6))->text())
                  .arg(QString::number(number))
                  .arg(dms)
                  .arg(((QStandardItem *)model->item(i,9))->text())
                  .arg(QString::number(currentSweepId))
                  .arg(((QStandardItem *)model->item(i,1))->text())
                  .arg(((QStandardItem *)model->item(i,4))->text());

ok = query.exec(tempQueryTemplate);
    if (!ok)
    {
        qDebug()<<query.lastError();
        if (query.lastError().text().contains("duplicate key value violates"))
            dublicate = true;

        //return;
    }
      //qDebug()<< tempQueryTemplate;

    }
    if (dublicate) QMessageBox::information(this,QString::fromLocal8Bit("Повторяющиеся строки"), QString::fromLocal8Bit("Одно или несколько значение не были добавлены, так как уже есть в базе данных"));

viewPoints(this->current_cat_id);
}
void TestWindow::viewPoints(int id){
    QStandardItemModel * model = new QStandardItemModel(this);

    model->setHorizontalHeaderItem(0,new QStandardItem(QString::fromLocal8Bit("Дата")));
    model->setHorizontalHeaderItem(1,new QStandardItem(QString::fromLocal8Bit("Время")));

    model->setHorizontalHeaderItem(2,new QStandardItem(QString::fromLocal8Bit("Группа")));
    model->setHorizontalHeaderItem(3,new QStandardItem(QString::fromLocal8Bit("Название")));

    model->setHorizontalHeaderItem(4,new QStandardItem(QString::fromLocal8Bit("Описание")));
    model->setHorizontalHeaderItem(5,new QStandardItem(QString::fromLocal8Bit("Величина")));
    model->setHorizontalHeaderItem(7,new QStandardItem(QString::fromLocal8Bit("Кто записал")));
    model->setHorizontalHeaderItem(6,new QStandardItem(QString::fromLocal8Bit("Кто принял")));

    QSqlQuery query;
    QString queryTemplate="SELECT trim(points.\"date\"), trim(points.\"time\"), trim(points.\"humanNames\"), trim(points.\"values\"), "
            "trim(\"SWEEPS\".who_take), trim(\"SWEEPS\".who_make), trim(\"DIVISIONS\".\"NAME\"), trim(points.defect), points.id FROM points INNER JOIN \"SWEEPS\" "
            "ON \"SWEEPS\".id = points.sweep_id "
            "INNER JOIN \"DIVISIONS\" "
            " ON \"DIVISIONS\".\"DIVISION\" = points.division"
            " WHERE \"SWEEPS\".category_id =  %1 "
            "ORDER BY 7;";

    bool ok = query.exec(queryTemplate.arg(id));

    qDebug()<<query.lastQuery();
    if(!ok){
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();
    return;
    }

    QList<QStandardItem *> list;
//QStandardItem * buf;
    while (query.next())
    {
        list.clear();

        list.append(new QStandardItem(query.value(0).toString()));
        list.append(new QStandardItem(query.value(1).toString()));
        list.append(new QStandardItem(query.value(6).toString()));
        list.append(new QStandardItem(query.value(2).toString()));
        list.append(new QStandardItem(query.value(7).toString()));
        list.append(new QStandardItem(query.value(3).toString()));
        list.append(new QStandardItem(query.value(4).toString()));
        list.append(new QStandardItem(query.value(5).toString()));
        list.append(new QStandardItem(QString::number(query.value(8).toInt())));
        for (int i =0; i < list.length(); i++){
            if ((i!=4)&&(i!=4)) list.at(i)->setEditable(false);
        }

        model->appendRow(list);
    }
    connect(model, SIGNAL(itemChanged(QStandardItem*)),this,SLOT(itemDataChanged(QStandardItem *)));

    ui->tableView_2->setModel(model);
    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_2->resizeRowsToContents();
ui->tableView_2->hideColumn(8);
    ui->tableView_2->setContextMenuPolicy(Qt::CustomContextMenu);
    disconnect(this, SLOT(customMenuRequested(QPoint)));


}

void TestWindow::customMenuRequested(QPoint pos){

    lastIndexPoints=ui->tableView_2->indexAt(pos);
    QMenu *menu=new QMenu(ui->tableView_2);
    QAction *action = new QAction(QString::fromLocal8Bit("Удалить"),menu);
    connect(action,SIGNAL(triggered()),this,SLOT(clickDelete()));
    menu->addAction(action);
    qDebug()<<"Menu exec";
    //menu->exec();
    menu->popup(ui->tableView_2->viewport()->mapToGlobal(pos));
}

void TestWindow::clickDelete(){
int row = lastIndexPoints.row();
qDebug()<<"delete clicked" << row;
QStandardItemModel *mdl = (QStandardItemModel *)ui->tableView_2->model();
for (int i = 0; i < mdl->columnCount(); i ++){
  mdl->item(row,i)->setBackground(Qt::red);
}
qDebug()<<"delete clicked finished" << row;

}
void TestWindow::acceptChanges(){
    QStandardItemModel * model = (QStandardItemModel *)ui->tableView_2->model();
    if (model == 0) return;
    for (int i = 0; i < model->rowCount(); i++){
        for(int j = 0; j <model->columnCount(); j++){
            if (model->item(i,j)->background()==Qt::yellow){
            ODGConfDialog *dlg = new ODGConfDialog();
            dlg->setTypeTwo();
            connect(dlg,SIGNAL(confirmed(QString*)),this,SLOT(changesAccepted(QString *)));
        dlg->show();
        return;
            }
        }
    }
    QMessageBox::information(this,QString::fromLocal8Bit("Информация для изменения"), QString::fromLocal8Bit("Отсутствует информация для изменения."));

}

void TestWindow::changesAccepted(QString *whoAccept){
    qDebug()<<"who accept ?" << *whoAccept;
    QStandardItemModel * model = (QStandardItemModel *)ui->tableView_2->model();
    if (model == 0) return;
QList<int> removeRows;
QList<int> defectRows;
QList<int> valueRows;
QStringList newDefects;
QStringList newValues;
    for (int i = 0; i < model->rowCount(); i++){
        for(int j = 0; j <model->columnCount(); j++){
            if (model->item(i,j)->background()==Qt::yellow){
                if ((j == 4)){
                  defectRows.append(model->item(i,8)->text().toInt());
                  newDefects.append(model->item(i,j)->text().trimmed());
                    }
                else if (j == 5){
                    valueRows.append(model->item(i,8)->text().toInt());
                    newValues.append(model->item(i,j)->text().trimmed());

                }
                else if (j==0){
                    removeRows.append(model->item(i,8)->text().toInt());
                }
            }
        }
    }

    qDebug()<<defectRows << defectRows.count()<<defectRows.size();
    qDebug()<<valueRows;
    qDebug()<<removeRows;
    for (int i = 0; i < removeRows.count(); i++){
        qDebug()<<"value at i" << i;
        removePoint(removeRows.at(i));
    }
    for (int i = 0; i < valueRows.count();i++){
        qDebug()<<"value at i" << i;
        //updateValue(newValues.at(i),valueRows.at(i));
    }
    for (int i = 0; i < defectRows.count();i++){
        qDebug()<<"value at i" << i;
       updateDefects(newDefects.at(i),defectRows.at(i));
    }

    this->viewPoints(this->current_cat_id);
}

void TestWindow::removePoint(int id){
    qDebug()<<"Remove point "<<id;
    QSqlQuery query;
    QString sqlTemplate = "DELETE FROM points WHERE id = %1;";
    bool ok = query.exec(sqlTemplate.arg(id));
    if (!ok) {
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();
    }
}


void TestWindow::updateValue(QString str, int id){
    qDebug()<<"Remove point "<<id;
    QSqlQuery query;
    QString sqlTemplate = "DELETE FROM points WHERE id = %1;";
    //bool ok
            //= query.exec(sqlTemplate.arg(id));
    /*if (!ok) {
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();
    }*/

}

void TestWindow::updateDefects(QString str, int id){
    qDebug()<<"Update defect "<<id;
    QSqlQuery query;
    QString sqlTemplate = "UPDATE points SET defect='%1' WHERE id=%2;";
    bool ok = query.exec(sqlTemplate.arg(str).arg(id));
    if (!ok) {
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();
    }

}


void TestWindow::itemDataChanged(QStandardItem *itm){
    itm->setBackground(Qt::yellow);
}

void TestWindow::selectRowFromCat(QModelIndex row){

    QStandardItemModel * model = (QStandardItemModel *) ui->tableView_cat->model();
    int id = model->item(row.row(),3)->text().toInt();
    this->current_cat_id = id;
    viewPoints(id);
    ui->pushButton_2->setEnabled(true);
    ui->action_7->setEnabled(true);

}
//
/*G:\openrpt-3.3.0>RPTrender.exe -databaseURL=psql://192.168.0.238/factory:5432 -u
sername="head" -passwd="0000" -loadfromdb="TechKard.CLF.6.2" -pdf -outpdf="dd.pd
f"*/




void TestWindow::selectRowFromPart(QModelIndex row){
    QStandardItemModel * model = (QStandardItemModel *) ui->tableView_part->model();
    int id = model->item(row.row(),3)->text().toInt();
    this->current_part_id = id;
    viewCategories(id);
    ui->action_2->setEnabled(true);

}

void TestWindow::showReport(){

    int curr_category = this->current_cat_id;
    QSqlQuery query;

    /*bool ok = query.exec(QString("SELECT report.report_name "
                        " FROM "
                         " public.categories, "
                         " public.report "
                       " WHERE "
                        " categories.cat_type = report.report_cat_id AND"
                        " categories.id = %1;").arg(curr_category));
    if (!ok)
    {
        qDebug()<<query.lastError();
        return;
    }

    qDebug()<<query.size();
    if (!query.next()){
        QMessageBox::information(this, "No reports to show", "There is no reports to this category");
        qDebug()<<query.lastQuery();
        return;
    }

    OdgORPTGw::startRPTRender("psql://"+db.hostName()+"/"+db.databaseName()+":"+QString::number(db.port()),
                              db.userName(),db.password(),query.value(0).toString(),
                              "-param=categ_id:integer="+QString::number(curr_category));*/

    ODGSelectReport * selRep = new ODGSelectReport(this);
    selRep->showRep(curr_category,db.hostName(),db.databaseName(),db.port(),db.userName(),db.password());
}

void TestWindow::findData(){

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open xml file"), "/home/", tr("XML Files (*.xml)"));
    OdgXmlParser *parser = new OdgXmlParser();
    parser->setFile(fileName);
    parser->xml2DOM();
    workData(parser);
}

void TestWindow::workData(OdgXmlParser *parser){

    wName = parser->workerName;

    foreach(OdgDivision *dvs, parser->divisions){
        qDebug()<<dvs->getDiv() << dvs->getName();
        foreach(ODGSweepPoint *swp, dvs->sweepVector){
            qDebug()<<swp->printPoint();
        }
    }

    QString full_list;

QStandardItemModel *model = new QStandardItemModel(this);
model->setHorizontalHeaderItem(0,new QStandardItem(QString::fromLocal8Bit("Дата и время")));
model->setHorizontalHeaderItem(1,new QStandardItem(QString::fromLocal8Bit("Номер группы")));
model->setHorizontalHeaderItem(2,new QStandardItem(QString::fromLocal8Bit("Имя группы")));
model->setHorizontalHeaderItem(3,new QStandardItem(QString::fromLocal8Bit("Номер параметра")));
model->setHorizontalHeaderItem(4,new QStandardItem(QString::fromLocal8Bit("Имя параметра")));
model->setHorizontalHeaderItem(5,new QStandardItem(QString::fromLocal8Bit("Значение")));
model->setHorizontalHeaderItem(6,new QStandardItem(QString::fromLocal8Bit("Пояснение")));
model->setHorizontalHeaderItem(7,new QStandardItem(QString::fromLocal8Bit("Примечание")));
model->setHorizontalHeaderItem(8,new QStandardItem(QString::fromLocal8Bit("Флаг дефекта")));
model->setHorizontalHeaderItem(9,new QStandardItem(QString::fromLocal8Bit("Поколение")));
model->setHorizontalHeaderItem(10,new QStandardItem(QString::fromLocal8Bit("Выполнил")));
ui->tableView->setModel(model);
ui->tableView->resizeRowsToContents();
QString line_template = "<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td><td>%5</td><td>%6</td></tr>";
full_list.append("<table border=\"1\" width=\"100%\">");
full_list.append(line_template.arg(QString::fromLocal8Bit("Дата и время"))
                    .arg(QString::fromLocal8Bit("Имя подразделения"))
                  .arg(QString::fromLocal8Bit("Имя точки"))
                  .arg(QString::fromLocal8Bit("Значение"))
                  .arg(QString::fromLocal8Bit("Дефект"))
                  .arg(QString::fromLocal8Bit("Примечание")));
QList<QStandardItem *> *itemList;
foreach(OdgDivision *dvs, parser->divisions){
    foreach(ODGSweepPoint *swp, dvs->sweepVector){
        itemList = new QList<QStandardItem *>();
        itemList->clear();

        QStandardItem *defectCell = new QStandardItem ();
        defectCell->setCheckable(true);
        if (swp->isAlarmedBool)
            defectCell->setCheckState(Qt::Checked);


        itemList->append(new QStandardItem(swp->time_date));
        itemList->append(new QStandardItem(QString::number(dvs->getDiv())));
        itemList->append(new QStandardItem(dvs->getName()));
        itemList->append(new QStandardItem(swp->id));
        itemList->append(new QStandardItem(swp->name));
        itemList->append(new QStandardItem(swp->value));
        itemList->append(new QStandardItem(swp->defect));
        itemList->append(new QStandardItem(swp->description));
        itemList->append(defectCell);
        itemList->append(new QStandardItem(swp->secCode));
        itemList->append(new QStandardItem(parser->workerName));
        for (int i = 0; i < itemList->length(); i++){
            if ((i!=4) && (i!=6)) itemList->at(i)->setEditable(false);
        }

        model->appendRow(*itemList);

    }
   ui->tableView->hideColumn(1);
   ui->tableView->hideColumn(3);
   ui->tableView->hideColumn(8);
   ui->tableView->hideColumn(9);
   ui->tableView->resizeColumnsToContents();
   ui->tableView->setColumnWidth(2,150);
     ui->tableView->setColumnWidth(4,150);
   ui->tableView->resizeRowsToContents();

}
full_list.append("</table>");


QString fullTextForOut;
fullTextForOut
        //.append("<h2>Divisions list</h2>")
        //.append(division_list)
        //.append("<h2>Analog values</h2>")
        //.append(numeric_list)
        //.append("<h2>Alarm list</h2>")
        //.append(alarm_list)
        //.append("<h2>Not maked list</h2>")
        //.append(unmaked_list)
        .append("<h2>Full list</h2>").append(full_list);
QTextDocument *qtDoc = new QTextDocument(this);
qtDoc->setHtml(fullTextForOut);
//ui->textEdit->setDocument(qtDoc);

//viewCategories(0);

}

void TestWindow::fromUSB(void){

    OdgXmlParser *parser = new OdgXmlParser();
    QString filePath =  getLetter();
    parser->setFile(filePath);
    parser->xml2DOM();
    workData(parser);
    if (filePath!="false"){

    QFile renFile(filePath);
    if (!renFile.open(QIODevice::ReadWrite | QIODevice::Text)){
        QMessageBox::warning(this,QString::fromLocal8Bit("Проблема с доступом к файлу"), QString::fromLocal8Bit("Обратитесь к администратору"));

        return;
    }
    renFile.rename(renFile.fileName()+".used");
    }
}

QString TestWindow::getLetter(void)
{
    QString WORK_DIR = "/ODGFiles/xml/";
    QStringList filters; /**< Create new string list to storage filter value */
    filters << "*.xml"; /**< Add filter "clean*" to avoid raw files using */
    QDir ourDir; /**< Create new QDir variable to storage dir value of mobile device*/
    ourDir.setNameFilters(filters);/**< Set filter "clean*" to this dir to use only clean files */


    QFileInfoList list = QDir::drives(); /**< Create new list to storage drives on this computer. */
    /**< Get this drived */
    QString letter; /**< Create QString to storage this letter */
    while (!list.isEmpty()) /**< Go to loop to check the all drives on computer */
    {
        letter = list.takeLast().absolutePath()+WORK_DIR; /**< Create full path to system directory */
        ourDir.setPath(letter);/**< Set this path */
        if (ourDir.exists()) /**< If this path exists  */
        {
            if (!ourDir.entryList().isEmpty())/**<      If thre are files */
            {
                return letter + ourDir.entryList(QDir::NoFilter,QDir::Time).first();/**<            Return the filepath */
            }

            return "false";/**< In other cases return "false" */
        }
    }
    return "false";
}
void TestWindow::startBatchAdding(){
    ODGDataAddMaster * master = new ODGDataAddMaster(this);
    master->show();
    master->setODGBroadcaster(asker, current_cat_id);
    connect(master,SIGNAL(viewPoints(int)),this,SLOT(viewPoints(int)));
    //master->exec();
}

