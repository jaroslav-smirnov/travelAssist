#include "odgdataaddmaster.h"
#include "ui_odgdataaddmaster.h"

ODGDataAddMaster::ODGDataAddMaster(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ODGDataAddMaster)
{
    ui->setupUi(this);
}

ODGDataAddMaster::~ODGDataAddMaster()
{
    delete ui;
}

void ODGDataAddMaster::setODGBroadcaster(ODGBroadCastAsker * asker, int curCat){
    current_cat_id = curCat;
    ui->toolBox->setCurrentIndex(0);
    this->asker = asker;
    count = asker->getClientCount();
    percent = 100/(count+1);
    currentI = 0;
    if (count==0){
        QMessageBox::information(this,QString::fromLocal8Bit("Отсутствуют устройства"),QString::fromLocal8Bit("В зоне действия сети отсутствуют подключенные устройства. Мастер добавления не может продолжать работу"));
        this->close();
    }
    connect(asker, SIGNAL(dataGot(QString)), this, SLOT(gotData(QString)));
    asker->getList(currentI);
    ui->progressBar->setValue(0);
    connect (ui->toolBox,SIGNAL(currentChanged(int)),this, SLOT(activeViewChanged(int)));
    connect (ui->pushButton_2,SIGNAL(clicked()),this,SLOT(close()));
    connect (ui->pushButton,SIGNAL(clicked()),this,SLOT(acceptData()));
    ui->toolBox->setItemEnabled(2,false);
}

void ODGDataAddMaster::activeViewChanged(int view){
    if (view == 1){
        ui->toolBox->setItemEnabled(2,true);
        ui->toolBox->setItemEnabled(0,false);
        setItems();
    }
    else if (view == 2){
        ui->toolBox->setItemEnabled(1,false);
        initLastPane();
    }
}

void ODGDataAddMaster::setItems(){
    ui->progressBar_2->setValue(0);
    QItemSelectionModel *selModel = ui->tableView_2->selectionModel();
    for(int i = 0; i < selModel->selectedRows().size(); i++){
        selectedItems.append(phoneFiles.at(selModel->selectedRows().at(i).row()));
    }
    selectedItemsSize = 1;
    if (selectedItems.size()==0)return;
    selectedItemsSize = selectedItems.size();
    int i = selectedItems.at(0)->getDevNumber();
    QString filepath = selectedItems.at(0)->getPath();
    selectedItems.removeAt(0);
    asker->getConcreteData(i,filepath);
    QStandardItemModel *model = new QStandardItemModel(this);
    ui->tableView->setModel(model);

}
void ODGDataAddMaster::acceptData(){
    qDebug()<<"Set items";
    QSqlQuery query;
    QString queryStr;

    QSettings settings("config.ini",QSettings::IniFormat);
    QString who;
    int curPos = ui->comboBox->currentIndex();
    settings.setValue("confirm_pos", QVariant(curPos));
    settings.sync();


    queryStr="SELECT \"NAME\" FROM users WHERE \"NAME\"= '%1' AND \"CODE\" = %2;";
    bool ok = query.exec(queryStr.arg(ui->comboBox->currentText())
                         .arg(QString::number(ui->lineEdit->text().toInt())));
    if(!ok) {
        qDebug()<<query.lastError();
        return;
    }
    qDebug()<<query.lastQuery();
    qDebug()<<query.size();
    if (query.size()<1){
        QMessageBox::warning(this,QString::fromLocal8Bit("Неверный пароль"),QString::fromLocal8Bit("Пожалуйста проверьте введенный Вами пароль."));
        return;
    }
    else
    {
        who = ui->comboBox->currentText();
    }
    QStandardItemModel *model = (QStandardItemModel *)ui->tableView->model();
    QStringList wNames;
    for (int i = 0; i < model->rowCount(); i++){
        if (!wNames.contains(model->item(i,10)->text()))
            wNames.append(model->item(i,10)->text());
    }
    for (int i = 0; i < wNames.size(); i++){
        addPointsFromTable(wNames.at(i),who);
    }
    this->close();
}

void ODGDataAddMaster::addPointsFromTable(QString wName, QString whoAdd){
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

        if (!(model->item(i,10)->text().trimmed() == wName.trimmed())){
            continue;
        }

        dateAndTime = ((QStandardItem *)model->item(i,0))->text();

        tempQueryTemplate = queryTemplate;
        QString temporaryBool;
        if (((QStandardItem *)model->item(i,8))->checkState()==Qt::Checked)
            temporaryBool = "true";
        else temporaryBool = "false";

        QString tempStr = ((QStandardItem *)model->item(i,5))->text().replace("  "," ");
        QStringList items = tempStr.split(" ");
        double number;
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
                .arg(temporaryBool)    .arg(QString::number(number))
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
        }

    }
    if (dublicate) QMessageBox::information(this,QString::fromLocal8Bit("Повторяющиеся строки"), QString::fromLocal8Bit("Одно или несколько значение не были добавлены, так как уже есть в базе данных"));

    emit viewPoints(this->current_cat_id);
}



void ODGDataAddMaster::gotData(QString data){
    if (data.contains("<<listdata>>") && data.contains("<<endlistdata>>")){
        data =  data.remove("<<listdata>>");
        data = data.remove("<<endlistdata>>");
        QStringList items = data.split("\r\n");

        for (int j = 0; j < items.size(); j ++){
            if (items.at(j).isEmpty())continue;
            PhoneFileItem *item = new PhoneFileItem(this);
            item->fromString(items.at(j), currentI);
            phoneFiles.append(item);
        }

        currentI++;
        ui->progressBar->setValue(currentI*100/count);
        if (currentI == count){
            ui->progressBar->hide();
            QStandardItemModel *mdl = new QStandardItemModel(this);
            this->sortPhoneFiles();
            for (int i = 0; i < phoneFiles.size(); i++){
                mdl->appendRow(phoneFiles.at(i)->getItemsForTable());
                qDebug()<<phoneFiles.at(i)->toString();
            }
            ui->tableView_2->setModel(mdl);
            ui->tableView_2->resizeColumnsToContents();
            ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
        }
        else
        {
            asker->getList(currentI);

        }
    }
    else if (data.contains("<<odgdata>>") && data.contains("<<stopodgdata>>")){
        data.remove("<<odgdata>>");
        data.remove("<<stopodgdata>>");
        OdgXmlParser *parser = new OdgXmlParser(this);
        parser->xml2DOM(data);
        addFileDataToTable(parser);
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
        addFileDataToTable(parser);
    }

}

void ODGDataAddMaster::addFileDataToTable(OdgXmlParser *parser){
    ui->progressBar_2->setValue(100*(selectedItemsSize - selectedItems.size())/selectedItemsSize);
    QStandardItemModel *model = (QStandardItemModel *)ui->tableView->model();


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




    if (selectedItems.size()==0)
    {
        ui->progressBar_2->hide();
        return;
    }
    int i = selectedItems.at(0)->getDevNumber();
    QString filepath = selectedItems.at(0)->getPath();
    selectedItems.removeAt(0);
    asker->getConcreteData(i,filepath);
}

bool ODGDataAddMaster::comparePhoneFiles(PhoneFileItem *a, PhoneFileItem *b){
    qDebug()<<a->getDateTime().toMSecsSinceEpoch() - b->getDateTime().toMSecsSinceEpoch();
    return (a->getDateTime().toMSecsSinceEpoch()<b->getDateTime().toMSecsSinceEpoch());
}

void ODGDataAddMaster::initLastPane(){
    QSqlQuery query;
    QString queryStr;
    queryStr="SELECT \"NAME\" FROM users ORDER BY \"NAME\";";
    bool ok = query.exec(queryStr);
    if(!ok) {
        qDebug()<<query.lastError();
        return;
    }
    QStringList names;
    while (query.next()){
        names.append(query.value(0).toString());
    }
    ui->comboBox->addItems(names);
    connect(ui->pushButton,SIGNAL(clicked()), this, SLOT(pushAdd()));

    QSettings settings("config.ini",QSettings::IniFormat);
    int curPos = settings.value("confirm_pos", QVariant(0)).toInt();
    ui->comboBox->setCurrentIndex(curPos);
    ui->lineEdit->setFocus();
}

void ODGDataAddMaster::sortPhoneFiles(){
    QList<PhoneFileItem *> buffer = phoneFiles;
    phoneFiles.clear();
    PhoneFileItem *curi;
    qDebug()<<buffer.length();
    int iterations = buffer.size();
    for (int j = 0; j < iterations; j++){
        curi = buffer.at(0);
        for (int i =0; i < buffer.size(); i++){
            if (comparePhoneFiles(curi,buffer.at(i)))
                curi = buffer.at(i);

        }
        phoneFiles.append(curi);
        buffer.removeOne(curi);
    }
    qDebug()<<curi->toString();

}
