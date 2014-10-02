#include "odgdivision.h"

OdgDivision::OdgDivision(QObject *parent) :
    QObject(parent)
{
}
void OdgDivision::setDivision(int div){
    QString queryStr = "SELECT \"NAME\" FROM \"DIVISIONS\" WHERE \"DIVISION\" = %1;";
    queryStr = queryStr.arg(QString::number(div));
    QSqlQuery query;
    if (!query.exec(queryStr)){
        qDebug()<<query.lastError();
    }
    while (query.next()){
        this->name = query.value(0).toString().trimmed();
    }
    this->division = div;
}
QString OdgDivision::getName(){
    return this->name;
}
int OdgDivision::getDiv(){
    return this->division;
}

bool OdgDivision::fullFillFromDB(){
    QString queryStr = "SELECT \"NAME\" FROM \"eBoxes\" WHERE \"DIVISION\" = %1;";
    queryStr = queryStr.arg(QString::number(division));
    QSqlQuery query;
    if (!query.exec(queryStr)){
        qDebug()<<query.lastError();
        return false;
    }
    this->sweepVector.clear();
    ODGSweepPoint *sweepPoint;
    while (query.next()){
        sweepPoint = new ODGSweepPoint(this);
        sweepPoint->setParams(QString::number(query.value(0).toInt()), QString::number(this->division),
                              QString (""), QString (""),QString (""),
                              QString (""), QString (""), QString (""),
                              QString (""),QString (""));
        sweepVector.append(sweepPoint);

    }
    return true;
}
