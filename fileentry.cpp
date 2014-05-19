#include "fileentry.h"

/**
 * @brief Constructor of class. It is empty.
 *
 * @fn fileEntry::fileEntry
 */
fileEntry::fileEntry()
{
}

bool fileEntry::fromODGSweepPoint(){

    wString = params.value(0);/**< Get the id of current point from string list */
    vers = wString.mid(0,1).toInt();/**< Get the version of barcode from id of current point */
    devision = wString.mid(1,2).toInt();/**< Get the division of code*/
    idPoint = wString.mid(3,4).toInt();/**< Get the number of point from ID */
    cDate = QDate::fromString(params.value(1).trimmed(),"d.M.yyyy");/**< Get the date of point scanning */
    cTime = QTime::fromString(params.value(2).trimmed(),"h:m:s");/**< Get the time of point scanning */
    cPoint = params.value(3).trimmed();/**< Get the "human" point name from string list */
    value = params.value(4).trimmed();/**< Get value of point in string format*/

    if (params.value(5).trimmed().length() < 8) /**< Check length of entry in string list which are connected with failure */
            itsOk = false;/**< If it is shorter than 8 failure */
        else
            itsOk = true; /**< Else it's Ok */

    cFailure = params.value(6).trimmed();/**< Get the description of failure from list and trimm it to avoid empty chars at the end and start */
    description = params.value(7).trimmed();/**< Get the additional description from list and trimm it to avoid empty chars at the end and start */

    isItWarned = isWarned();

    Str.chop(Str.length() - Str.lastIndexOf(",")); /**< Delete the checksum from initial string for result checking */
    int chksm = calcCode(Str); /**< Create new int to store the checksum of string */
    CHKSUM = params.value(8);/**< Get the checksum value from list */
    if (CHKSUM.toInt() == chksm) /**< Compare calculated value and stored value */
        codeCorr = true;
    else
        codeCorr = false;
    return true;/**< All is good return true */

}

/**
 * @brief Used to get data from string to fileEntry class
 *
 * @fn fileEntry::fromString
 * @param Str raw string with csv format
 * @return bool if execution was correct
 */
bool fileEntry::fromString(QString Str)
{
    QStringList params = Str.split(","); /**< Create stringlist params by splitting initial string by ',' */
    QString wString; /**< Create string for id of current point */

    if (params.isEmpty()) return false; /**< If we haven't data - stop function with false*/
    wString = params.value(0);/**< Get the id of current point from string list */
    vers = wString.mid(0,1).toInt();/**< Get the version of barcode from id of current point */
    devision = wString.mid(1,2).toInt();/**< Get the division of code*/
    idPoint = wString.mid(3,4).toInt();/**< Get the number of point from ID */
    cDate = QDate::fromString(params.value(1).trimmed(),"d.M.yyyy");/**< Get the date of point scanning */
    cTime = QTime::fromString(params.value(2).trimmed(),"h:m:s");/**< Get the time of point scanning */
    cPoint = params.value(3).trimmed();/**< Get the "human" point name from string list */
    value = params.value(4).trimmed();/**< Get value of point in string format*/

    if (params.value(5).trimmed().length() < 8) /**< Check length of entry in string list which are connected with failure */
            itsOk = false;/**< If it is shorter than 8 failure */
        else
            itsOk = true; /**< Else it's Ok */

    cFailure = params.value(6).trimmed();/**< Get the description of failure from list and trimm it to avoid empty chars at the end and start */
    description = params.value(7).trimmed();/**< Get the additional description from list and trimm it to avoid empty chars at the end and start */

    isItWarned = isWarned();

    Str.chop(Str.length() - Str.lastIndexOf(",")); /**< Delete the checksum from initial string for result checking */
    int chksm = calcCode(Str); /**< Create new int to store the checksum of string */
    CHKSUM = params.value(8);/**< Get the checksum value from list */
    if (CHKSUM.toInt() == chksm) /**< Compare calculated value and stored value */
        codeCorr = true;
    else
        codeCorr = false;
    return true;/**< All is good return true */
}
/**
 * @brief Return private integer id
 *
 * @fn fileEntry::getId
 * @return int number of point
 */
int fileEntry::getId(){
    return idPoint;
}

bool fileEntry::isWarned (){
    QSqlQuery query;
    bool ok = query.exec("SELECT \"Limits\" FROM \"eBoxes\" WHERE \"NAME\"="+
                         QString::number(idPoint)+" AND \"DIVISION\"="+QString::number(devision)+";");
    if (!ok) {
        //QMessageBox::warning(this,QString::fromLocal8Bit("Ошибка запроса к базе данных при получении информации об аварийных параметрах"),query.lastError().text());
    }
    if (!query.next()) return true;


    QStringList limits = query.value(0).toString().trimmed().split("*");

    qDebug()<<limits;
    if (limits.size()!=4) return false;
    float outStart = limits.at(0).toFloat();
    float start = limits.at(1).toFloat();
    float stop = limits.at(2).toFloat();
    float outStop = limits.at(3).toFloat();
    float val = getValueNumber();

    qDebug()<<"Values: "<<outStart<<" "<<outStop<<" "<<start<<" "<<" "<<stop<<" "<<val<<" is it ok? "<<itsOk;

    if ((val > outStart)&&(val<outStop))
            this->itsOk = true;

    qDebug()<<"Values: "<<outStart<<" "<<outStop<<" "<<start<<" "<<" "<<stop<<" "<<val<<" is it ok? "<<itsOk;

    if (val>stop) return true;
    if (val<start) return true;
    return false;
}


int fileEntry::getDivision()
{
    return devision;
}

/**
 * @brief Returns private integer version
 *
 * @fn fileEntry::getVersion
 * @return int version of code
 */
int fileEntry::getVersion()
{
    return vers;
}
/**
 * @brief Returns private QDate date
 *
 * @fn fileEntry::getDate
 * @return QDate date of check
 */
QDate fileEntry::getDate()
{
    return cDate;
}
/**
 * @brief Returns private QTime time
 *
 * @fn fileEntry::getTime
 * @return QTime time of check
 */
QTime fileEntry::getTime()
{
   return cTime;
}
/**
 * @brief Returns private String "Human" name of point
 *
 * @fn fileEntry::getPointName
 * @return QString "human" name of point
 */
QString fileEntry::getPointName()
{
    return cPoint;
}
/**
 * @brief Returns private String value
 *
 * @fn fileEntry::getValue
 * @return QString value of point
 */
QString fileEntry::getValue()
{
    return value;
}
/**
 * @brief Returns non-failure status
 *
 * @fn fileEntry::isOk
 * @return bool no failure
 */
bool fileEntry::isOk()
{
   return itsOk;
}
/**
 * @brief Returns failure status in form of string
 *
 * @fn fileEntry::isntOkStr
 * @return QString is point in failure
 */
QString fileEntry::isntOkStr()
{
    if (itsOk)
        return QString::fromAscii("false");
    return QString::fromAscii("true");
}
/**
 * @brief Returns string with type of failure
 *
 * @fn fileEntry::getFailure
 * @return QString failure of point
 */
QString fileEntry::getFailure()
{
   return cFailure;
}
/**
 * @brief Returns string with additional information about point
 *
 * @fn fileEntry::getDescription
 * @return QString description of point
 */
QString fileEntry::getDescription()
{
    return description;
}
/**
 * @brief Returns result of checksum operation
 *
 * @fn fileEntry::checkSUM
 * @return bool check sum
 */
bool fileEntry::checkSUM()
{
    return codeCorr;
}
/**
 * @brief Creates header for output file using pseudographics
 *
 * @fn fileEntry::addHeader
 * @return QString header in pseudographics
 */
QString fileEntry::addHeader()
{
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));
    QString bufferStr; /**< Create new buffer string for accumulator*/
    QChar sep = ' '; /**< Create new separator to make code more clean */
    /**< Add data to string */
    bufferStr.append(QString::fromLocal8Bit("Номер").rightJustified(6,sep) +' ');
    bufferStr.append(QString::fromLocal8Bit("Дата").leftJustified(10,sep) + ' ' +
                     QString::fromLocal8Bit("Время").leftJustified(8,sep) + ' ' +
                     QString::fromLocal8Bit("Название").leftJustified(20,sep)+' ' +
                     QString::fromLocal8Bit("Значение").leftJustified(8,sep,true)+' ' +
                     QString::fromLocal8Bit("Дефект").leftJustified(15,sep)+ ' ' +
                     QString::fromLocal8Bit("Примечание").leftJustified(30,sep)+"\n");

    return bufferStr; /**< Return this string as result */
}
/**
 * @brief Creates row for output file using pseudographics
 *
 * @fn fileEntry::getString2Write
 * @return QString in pseudographics
 */
QString fileEntry::getString2Write()
{
    QString bufferStr; /**< Create new string for accumulator */
    QChar sep = ' '; /**< Create new separator to simplify code */
/**< Add first part of data to buffer string */
    bufferStr.append(QString::number(idPoint).rightJustified(6,QChar::fromAscii('0')) +  ' ');
    bufferStr.append(cDate.toString("dd.MM.yyyy").leftJustified(10,sep,true) + ' ' +
                     cTime.toString().leftJustified(8,sep,true) + ' ' +
                     cPoint.leftJustified(20,sep,true) + ' ' + value.leftJustified(8,sep,true)
                     + ' ');
    /**< Check no-failure value. If it's ok then put no-failure string, else put failure */
    if (itsOk) bufferStr.append(QString::fromLocal8Bit("Нет дефекта").leftJustified(15,sep,true)+ ' ');
    else bufferStr.append(cFailure.leftJustified(15,sep,true) + ' ');
    bufferStr.append(description.leftJustified(30,sep,true)+"\n"); /**< Append the last parameter of string */

    return bufferStr; /**< Return the result string */
}
/**
 * @brief Creates header for output file using html tags
 *
 * @fn fileEntry::addHeaderHTML
 * @return QString with header of file
 */


QString fileEntry::addHeaderHTML()
{
    QString bufferStr; /**< Create new string for accumulator */
    /**< Add header data to this string */
    bufferStr.append("<tr><td>" + QString::fromLocal8Bit("Номер") + "</td><td>" +
                     QString::fromLocal8Bit("Дата") + "</td><td>" +
                     QString::fromLocal8Bit("Время") + "</td><td>" +
                     QString::fromLocal8Bit("Название") + "</td><td>" +
                     QString::fromLocal8Bit("Значение") + "</td><td>"+
                     QString::fromLocal8Bit("Дефект")+ "</td><td>"+
                     QString::fromLocal8Bit("Примечание") + "</td></tr>");

    return bufferStr; /**< Return this string */
}
/**
 * @brief Creates row for output file using html tags
 *
 * @fn fileEntry::getString2WriteHTML
 * @return QString with tags row
 */

QString fileEntry::getString2WriteHTML(){
  return  getString2WriteHTML("");
}

QString fileEntry::getString2WriteHTML(QString dName)
{
    QString bufferStr; /**< Create new accumulator for buffer string */

    /**< Append first part of data with HTML tags */
    QString outName = dName.trimmed() + " " + cPoint;
    //outName.trimmed();

    if (itsOk){
        if (this->isWarned())bufferStr.append("<tr bgcolor=#FFFF66>");
        else bufferStr.append("<tr bgcolor=#FFFFFF>");
    }
    else bufferStr.append("<tr bgcolor=#FF6347>");
    bufferStr.append("<td>" + QString::number(devision).rightJustified(2,QChar::fromAscii('0'))+
                                                  QString::number(idPoint).rightJustified(4,QChar::fromAscii('0')) + "</td><td>" +
                     cDate.toString("dd.MM.yyyy") + "</td><td>" +
                     cTime.toString() + "</td><td>" +
                     outName + "</td><td>" + value + "</td><td>");
    /**< Check for no-failure value and add result in string form */
    if (itsOk) bufferStr.append(QString::fromLocal8Bit("Нет дефекта")+ "</td><td>");
    else bufferStr.append(cFailure + "</td><td>");
    bufferStr.append(description + "</td></tr>");/**< Append the last part of row */

    return bufferStr; /**< Return this string */
}
/**
 * @brief Used to calculate checksum
 *
 * @fn fileEntry::calcCode
 * @param str
 * @return int checksum
 */
int fileEntry::calcCode(QString str)
{
    int result = 0; /**< Create result accumulator */

    for (int i = 0; i < str.length(); i++) /**< Start for statement to check all elements of string */
            result+=str.at(i).unicode(); /**< Calculate the sum of all symbols code in unicode */

    result = qAbs(result); /**< Get the result by module, because we cannot use unsigned variable in java */

    return result; /**< Return the result */
}
/**
 * @brief Used to get value in form of float
 *
 * @fn fileEntry::getValueNumber
 * @return float value
 */
float fileEntry::getValueNumber()
{
    return value.section(" ",0,0).toFloat(); /**< Return the first part of string converted to float */
}

/**
 * @brief Used to get dimension of value in string form
 *
 * @fn fileEntry::getDimension
 * @return QString dimension of value
 */
QString fileEntry::getDimension()
{
    return value.section(" ",1,1); /**< Return the second part of string */
}

/**
 * @brief Creates query to send to database
 *
 * @fn fileEntry::getQueryToSend
 * @return QString query
 */
QString fileEntry::getQueryToSend(int curId)
{
    /**< Return query with special string and private values */
    isItWarned = isWarned();
    return "INSERT INTO public.\"points\" "
                         "("
                         "\"number\","
                         "date,"
                         "\"time\","
                         "\"values\","
                         "defect,"
                         "description,"
                         "is_defect,"
                         "values_number,"
                         "dimensions,"
                         "generation,"
                         "sweep_id,"
                         "division,"
                         "\"humanNames\","
                         "coordinates"
                         ")"
                         "VALUES ( "
                         +QString::number(idPoint)+",'"
                         + cDate.toString("dd.MM.yyyy") + "','"
                         + cTime.toString("hh:mm:ss")+ "','"
                         + value + "','"
                         + cFailure + "','"
                         + description + "',"
                         + this->isntOkStr() + ","
                         + QString::number(this->getValueNumber()) + ",'"
                         + this->getDimension() + "',"
                         + QString::number(vers)+ ","
                         + QString::number(curId)+ ","
                         + QString::number(devision)+ ",'"
            + cPoint + "','" + QString::number(this->isWarned())+"');";


}

