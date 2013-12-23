#include "getpointslistfromdb.h"

GetPointsListFromDB::GetPointsListFromDB()
{
}

QStringList GetPointsListFromDB::getAllPoints(QSqlQuery *query, int division)
{

    QString queryString = "SELECT \"GENERATION\",\"DIVISION\",\"NAME\",\"VALUES\",human_names,\"Limits\""
            "FROM public.\"eBoxes\" WHERE \"eBoxes\".\"DIVISION\" =" + QString::number(division)  + "ORDER BY \"NAME\";";

    QStringList result;


    if (!query->exec(queryString))
    {
        qDebug()<<query->lastError();
        return result;
    }

    QString str;

    while (query->next())
    {
        str = "x;";
        for (int i = 0; i < query->record().count(); i++)
            str+=query->value(i).toString().trimmed()+";";
        str.remove(str.length()-1, 1);
        result << str;
        str.clear();
    }
    qDebug()<<result;
    return result;

}

void GetPointsListFromDB::writeToFile(QString pref, QString post, int division, QStringList lst){
    QStringList result = lst;
    QString str;
    QString middle;
    if (division >= 0) middle = QString::number(division);
    else middle ="";

    QFile file(pref+middle+post);


    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out.setCodec("UTF-8");  /**< Set codec to UTF-8 */

    for (int i = 0; i < result.size(); i++)
    {
        str = result.at(i)+"\n";
        out<<str;
    }

    file.close();
}

QStringList GetPointsListFromDB::getAllDefects(QSqlQuery *query, int division)
{

    QString queryString = "SELECT problems.name FROM public.problems WHERE problems.div_id = "
            + QString::number(division) + ";";

    QStringList result;


    if (!query->exec(queryString))
    {
        qDebug()<<query->lastError();
        return result;
    }

    QString str;

    while (query->next())
    {
        for (int i = 0; i < query->record().count(); i++)
            str+=query->value(i).toString().trimmed()+";";
        str.remove(str.length()-1, 1);
        result << str;
        str.clear();
    }
    qDebug()<<result;
    return result;

}

QStringList GetPointsListFromDB::getAllOperats(QSqlQuery *query)
{

    QString queryString = "SELECT op_users.name FROM public.op_users;";

    QStringList result;


    if (!query->exec(queryString))
    {
        qDebug()<<query->lastError();
        return result;
    }

    QString str;

    while (query->next())
    {
        for (int i = 0; i < query->record().count(); i++)
            str+=query->value(i).toString().trimmed()+";";
        str.remove(str.length()-1, 1);
        result << str;
        str.clear();
    }
    qDebug()<<result;
    return result;

}

QString GetPointsListFromDB::getDivNames(QSqlQuery *query, int divId)
{

    QString queryString = QString("SELECT \"NAME\" FROM \"DIVISIONS\" WHERE \"DIVISION\" = ")+QString::number(divId)+";";

    QString str;


    if (!query->exec(queryString))
    {
        qDebug()<<query->lastError();
        return str;
    }


    while (query->next())
    {
        str = query->value(0).toString();
    }
    return str;

}


void GetPointsListFromDB::initScanner(QSqlQuery *query, QString path)
{

    QStringList buffer = GetPointsListFromDB::getAllOperats(query);
    GetPointsListFromDB::writeToFile(path + "names",".SGM",-1,buffer);
    QStringList divisions;
    int division;
    QString queryString = "SELECT \"DIVISION\" FROM \"DIVISIONS\";";
    if (!query->exec(queryString))
    {
        qDebug()<<query->lastError();
        return;
    }
    while (query->next())
    {
        divisions << QString::number(query->value(0).toInt());
    }
    for (int i = 0; i < divisions.size(); i++)
    {
        division = divisions.at(i).toInt();

        buffer = GetPointsListFromDB::getAllPoints(query,division);
        GetPointsListFromDB::writeToFile(path+"m",".DATA",division,buffer);

        buffer = GetPointsListFromDB::getAllDefects(query,division);
        GetPointsListFromDB::writeToFile(path + "-M",".DATA",division,buffer);
        buffer.clear();
        buffer << QString::number(division) << "80" << GetPointsListFromDB::getDivNames(query,division);
        GetPointsListFromDB::writeToFile(path + "INF",".DATA",division,buffer);
    }


}
