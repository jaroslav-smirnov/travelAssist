#ifndef ODGBROADCASTASKER_H
#define ODGBROADCASTASKER_H

#include <QObject>
#include <QtNetwork/QUdpSocket>
#include <QTimer>
#include <QDateTime>
#include <QList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QStringList>
#include <QNetworkAddressEntry>
#include <QSettings>
#include <QTimer>
class ODGBroadCastAsker : public QObject
{
    Q_OBJECT
public:
    explicit ODGBroadCastAsker(QObject *parent = 0);
    QList<QStringList> getDevices();
private:
    QList<QStringList> *devices;
    QTimer *timer;
    QUdpSocket *socket;
    QTcpServer *server;
    QList<QTcpSocket *> connectedSockets;
    QList<QString> names;
    bool listStart;
    QTimer waitForData;
    QString sendOutBuffer;
signals:
    void nameChanged(int i, QString newName);
    void nameAdded(int i, QString name, QString ip);
    void nameDeleted(int i);
    void dataGot(QString outTxt);
    
public slots:
    void onTimer(void);
    int getClientCount();
    void newConnection();
    void readyReadClient();
    void socDisconnect();
    void getData(int i);
    void getList(int i);
    void getBigList();
    void getConcreteData(int i, QString name);
    void collectionReady();
};

#endif // ODGBROADCASTASKER_H
