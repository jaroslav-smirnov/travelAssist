#ifndef GETPOINTSLISTFROMDB_H
#define GETPOINTSLISTFROMDB_H

#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QStringList>
#include <QSqlRecord>
#include <QVariant>
#include <QFile>
#include <QTextStream>
#include <QIODevice>


class GetPointsListFromDB
{
public:
    GetPointsListFromDB();
    static QStringList getAllPoints(QSqlQuery *query, int division);
    static void writeToFile(QString pref, QString post, int division, QStringList lst);
    static QStringList getAllOperats(QSqlQuery *query);
    static QStringList getAllDefects(QSqlQuery *query, int division);
    static void initScanner(QSqlQuery *query, QString path);
    static QString getDivNames(QSqlQuery *query, int divId);
    //static QString filePath;
};

#endif // GETPOINTSLISTFROMDB_H
