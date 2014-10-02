#include "phonefileitem.h"

PhoneFileItem::PhoneFileItem(QObject *parent) :
    QObject(parent)
{
}

QString PhoneFileItem::getName(){
    return makerName;
}

QString PhoneFileItem::getPath(){
    return path;
}

QDateTime PhoneFileItem::getDateTime(){
    return dateTime;
}

bool PhoneFileItem::fromString(QString value, int devNumber){
    this->devNumber = devNumber;
    QStringList items = value.split(";");
    if (items.size() !=4) return false;

    this->makerName = items.at(2);
    this->path = items.at(3);
    this->dateTime = QDateTime::fromString(items.at(1), "dd.MM.yyyy hh:mm:ss");
}

int PhoneFileItem::getDevNumber(){
    return this->devNumber;
}

QString PhoneFileItem::toString(){
    return dateTime.toString("dddd dd.MM.yyyy hh:mm:ss")+" "+makerName + " " + path;
}

QList<QStandardItem *> PhoneFileItem::getItemsForTable(){
    QList<QStandardItem *> list;
    list.append(new QStandardItem(dateTime.toString("dddd dd.MM.yyyy hh:mm:ss")));
    list.append(new QStandardItem(makerName));
    for (int i = 0; i < list.size(); i++){
        list.at(i)->setEditable(false);
    }
    return list;
}
