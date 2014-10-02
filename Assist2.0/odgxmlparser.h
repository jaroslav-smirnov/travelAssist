#ifndef ODGXMLPARSER_H
#define ODGXMLPARSER_H

#include <QObject>
#include <QVector>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include "odgdivision.h"
#include <QTextCodec>
#include "odgorptgw.h"

class OdgXmlParser : public QObject
{
    Q_OBJECT
public:
    explicit OdgXmlParser(QObject *parent = 0);
    bool setFile(QString path);

    bool xml2DOM();
    bool xml2DOM(QString outerString);
    QVector <OdgDivision *> divisions;
    QString workerName;
    QString XMLFile;
    bool parsePoint(QDomElement);

signals:
    
public slots:
private:

};

#endif // ODGXMLPARSER_H
