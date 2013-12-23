#ifndef OUTPUTTEMPLATE_H
#define OUTPUTTEMPLATE_H

#include <QObject>
#include <QSqlQuery>
#include <QString>
#include <QVariant>

class OutputTemplate : public QObject
{
    Q_OBJECT
public:
    explicit OutputTemplate(QObject *parent = 0);
    void getTemplate (QSqlQuery *query, int division, int type);
    QString getHeader();
    
private:
    QString header;
signals:
    
public slots:
    
};

#endif // OUTPUTTEMPLATE_H
