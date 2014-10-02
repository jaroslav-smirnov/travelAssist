#include "odgsweeppoint.h"

ODGSweepPoint::ODGSweepPoint(QObject *parent) :
    QObject(parent)
{
}

void ODGSweepPoint::setParams(QString id, QString division,
                              QString secCode,QString isAlarmed,QString isOff,
                              QString value, QString time_date, QString description,
                              QString defect,QString code){
    this->id = id;
    this->division = division;
    this->secCode = secCode;
    this->isAlarmed = isAlarmed;
    this->isOff = isOff;
    this->value = value;
    this->time_date = time_date;
    this->description = description;
    this->defect = defect;
    this->code = code;

    QStringList flSListValue = value.split(" ");
    if (flSListValue.size()>0){
        floatValue = flSListValue.at(0).toFloat();
    }
    QStringList tdl = time_date.split(",");
    if (tdl.length() == 2){
        time = tdl.at(0).trimmed();
        date = tdl.at(1).trimmed();
    }
    QString queryStr = "SELECT human_names, \"Limits\", \"VALUES\"  FROM \"eBoxes\" WHERE (\"NAME\" = %1) AND (\"DIVISION\" = %2);";
    queryStr = queryStr.arg(id).arg(division);
    QSqlQuery query;
    if (!query.exec(queryStr)){
        qDebug()<<query.lastError();
    }
    QStringList ranges;
    isAlarmedBool = false;
    isWarnedBool = false;
    while (query.next()){
        this->name = query.value(0).toString().trimmed();
        this->dimensions = query.value(2).toString().trimmed();
        QString buffstr = query.value(1).toString().trimmed();
        buffstr.replace("*", "ggfg");
        qDebug()<<buffstr;
        ranges = buffstr.split("ggfg");
        qDebug()<<ranges;
        qDebug()<<ranges.length();
        if (ranges.length() != 4 ){
         isAnalog = false;
            continue;
        }
        else isAnalog = true;
        qDebug()<<"check ranges";
        this->LAL = ranges.at(0).toFloat();
        this->LWL = ranges.at(1).toFloat();
        this->HAL = ranges.at(3).toFloat();
        this->HWL = ranges.at(2).toFloat();
        if ((floatValue > HWL) || (floatValue < LWL)) this->isWarnedBool = true;
        if ((floatValue > HAL) || (floatValue < LAL)) this->isAlarmedBool = true;
    }
    alreadyExist = this->isExists();
}
bool ODGSweepPoint::isExists(){
    QString queryStr = "SELECT count(id)  FROM points WHERE (date = '%1') AND (time = '%2')"
            " AND (number = %3) AND (division = %4);";
    queryStr = queryStr.arg(time).arg(date).arg(id).arg(division);
    QSqlQuery query;
    if (!query.exec(queryStr)){
        qDebug()<<query.lastError();
        return false;
    }
    if (query.next())
    {
        if (query.value(0).toInt()==0) return false;
        else return true;
    }
    return true;
}

QString ODGSweepPoint::printPoint(){
    return QString("Name : %11, Division id: %1 division: %2, "
                   "secCode: %3, isAlarmed %4, isOff "
                   " %5, value: %6, time_date %7, defect %8 "
                   " description %9, code %10").arg(id).arg(division).arg(secCode).arg(isAlarmed).
        arg(isOff).arg(value).arg(time_date).arg(defect).arg(description).arg(code).arg(name);
}
