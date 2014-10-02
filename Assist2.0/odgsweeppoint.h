#ifndef ODGSWEEPPOINT_H
#define ODGSWEEPPOINT_H

#include <QObject>
#include <QSqlQuery>
#include <QVariant>
#include <QStringList>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

class ODGSweepPoint : public QObject
{
    Q_OBJECT
private:

public:
    explicit ODGSweepPoint(QObject *parent = 0);
    void setParams(QString id, QString division,
                   QString secCode,QString isAlarmed,QString isOff,
                   QString value, QString time_date, QString description,
                   QString defect,QString code);
    QString printPoint();
    QString division;
    QString secCode;
    QString isAlarmed;
    QString isOff;
    QString value;
    QString time_date;
    QString description;
    QString defect;
    QString code;
    QString name;
    QString time;
    QString date;
    bool alreadyExist;
    bool isAlarmedBool;
    bool isWarnedBool;
    float floatValue;
QString id;
    float LAL;
    float LWL;
    float HWL;
    float HAL;
    QString dimensions;
    bool isAnalog;
    bool exists;
    bool isExists();
signals:
    
public slots:
    
};

#endif // ODGSWEEPPOINT_H
