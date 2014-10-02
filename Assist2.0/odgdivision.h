#ifndef ODGDIVISION_H
#define ODGDIVISION_H

#include <QObject>
#include <QVector>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlError>
#include "odgsweeppoint.h"

class OdgDivision : public QObject
{
    Q_OBJECT
public:
    explicit OdgDivision(QObject *parent = 0);
    void setDivision(int div);
    int getDiv();
    QString getName();
    QVector<ODGSweepPoint*> sweepVector;
    bool fullFillFromDB();
signals:
    
public slots:
private:
    int division;
    QString name;

};

#endif // ODGDIVISION_H
