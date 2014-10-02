#ifndef ODGORPTGW_H
#define ODGORPTGW_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QDebug>
#include <qmath.h>
#include <QMessageBox>
#include "windows.h"
#include <QFileInfo>

class OdgORPTGw : public QObject
{
    Q_OBJECT
public:
    explicit OdgORPTGw(QObject *parent = 0);
    static int startRPTRender(QString databaseURL, QString username, QString passwd, QString loadfromdb, QString params);
    
signals:
    //void couldntCreate();
    
public slots:
    
};

#endif // ODGORPTGW_H
