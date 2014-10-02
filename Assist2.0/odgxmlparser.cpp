#include "odgxmlparser.h"

OdgXmlParser::OdgXmlParser(QObject *parent) :
    QObject(parent)
{
}

bool OdgXmlParser::setFile(QString path)
{
    XMLFile.clear();
    QFile file(path);
         if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
             return false;


         QTextStream in(&file);
         in.setCodec(QTextCodec::codecForName("UTF-8"));
         while (!in.atEnd()) {
             XMLFile.append(in.readLine());
         }
         return true;
}
bool OdgXmlParser::xml2DOM(){

    this->xml2DOM(XMLFile);
}

bool OdgXmlParser::xml2DOM(QString outerString){
    XMLFile = outerString;
    int column;
    int line;
    QString errorString;
    QString workFile = outerString + "</sweep>";
    QDomDocument doc;
    if (!doc.setContent(workFile, &errorString, &line, &column)){
        qWarning()<<QString("Line %1, column %2: %3").arg(line).arg(column).arg(errorString);
        return false;
    }
    QDomElement root = doc.documentElement();
    if (root.tagName()!= "sweep"){
        qWarning("File is not sweep file");
        return false;
    }
    qDebug ()<< doc.toString(2);

    QDomNode node = root.firstChild();

    while (!node.isNull()){
        //qDebug()<<"newSibling";
        if (node.toElement().tagName() == "worker"){
            //Process worker name
            workerName = node.toElement().text();
            qDebug()<<"Name of worker "<< workerName;
        }
        if (node.toElement().tagName() == "point"){
            parsePoint(node.toElement());
        }
        //qDebug() << "nextSibling";
        node = node.nextSibling();
    }
    return true;
}

bool OdgXmlParser::parsePoint(QDomElement element){
    QString elementStr;
    QString id = element.attribute("id");
    QString division = element.attribute("division");
    QString secCode = element.attribute("secCode");
    QString isAlarmed = element.attribute("isAlarmed","false");
    QString isOff = element.attribute("isOff","false");
    QString value = " ";
    QString time_date = " ";
    QString description = " ";
    QString defect = " ";
    QString code = " ";

    QDomNode node = element.firstChild();
    while (!node.isNull()){
        if (node.toElement().tagName() == "value")
            value = node.toElement().text();
        else if (node.toElement().tagName() == "code")
            code = node.toElement().text();
        else if (node.toElement().tagName() == "time_date")
            time_date = node.toElement().text();
        else if (node.toElement().tagName() == "defect")
            defect = node.toElement().text();
        else if (node.toElement().tagName() == "description")
            description = node.toElement().text();

        node = node.nextSibling();
    }

    elementStr = QString("Division id: %1 division: %2, "
                       "secCode: %3, isAlarmed %4, isOff "
                       " %5, value: %6, time_date %7, defect %8 "
                       " description %9, code %10").arg(id).arg(division).arg(secCode).arg(isAlarmed).
            arg(isOff).arg(value).arg(time_date).arg(defect).arg(description).arg(code);
    qDebug()<<elementStr;
    ODGSweepPoint * point = new ODGSweepPoint(this);
    point->setParams(id,division,secCode,isAlarmed,isOff,value,time_date,description,defect,code);
    bool find = false;
    foreach (OdgDivision * div, divisions){
        if (div->getDiv() == division.toInt()){
            div->sweepVector.append(point);
            find = true;
        }
    }
    if (!find){
        OdgDivision *oddivision = new OdgDivision(this);
        oddivision->setDivision(division.toInt());
        oddivision->sweepVector.append(point);
        divisions.append(oddivision);
    }

}
