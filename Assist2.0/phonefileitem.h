#ifndef PHONEFILEITEM_H
#define PHONEFILEITEM_H

#include <QObject>
#include <QDateTime>
#include <QStringList>
#include <QList>
#include <QStandardItem>

class PhoneFileItem : public QObject
{
    Q_OBJECT
public:
    explicit PhoneFileItem(QObject *parent = 0);
    QString getName();
    QString getPath();
    QDateTime getDateTime();
    bool fromString(QString value, int devNumber);
    QString toString();
    QList<QStandardItem *> getItemsForTable();
    int getDevNumber();

signals:
    
public slots:
private:
    QString makerName;
    QString path;
    QDateTime dateTime;
    int devNumber;
};

#endif // PHONEFILEITEM_H
