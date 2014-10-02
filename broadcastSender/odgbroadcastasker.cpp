#include "odgbroadcastasker.h"

ODGBroadCastAsker::ODGBroadCastAsker(QObject *parent) :
    QObject(parent)
{
    timer = new QTimer(this);
    timer->setInterval(12000);

    socket = new QUdpSocket(this);
    server = new QTcpServer(this);
    server->listen(QHostAddress::Any, 3261);
    connect (server,SIGNAL(newConnection()),this, SLOT(newConnection()));
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    onTimer();
    timer->start();
}
void ODGBroadCastAsker::newConnection(){
    qDebug()<<"new socket";
    QTcpSocket * clientSocket = server->nextPendingConnection();
    connectedSockets.append(clientSocket);
    names.append("");
    connect(clientSocket,SIGNAL(readyRead()),this,SLOT(readyReadClient()));
    connect(clientSocket,SIGNAL(disconnected()),this,SLOT(socDisconnect()));

    clientSocket->write(QByteArray("What is your name?\r\n"));
    emit (nameAdded(connectedSockets.indexOf(clientSocket),QString(""),clientSocket->peerAddress().toString()));
}

void ODGBroadCastAsker::socDisconnect(){
    qDebug()<<names;
    QTcpSocket *clientSocket = (QTcpSocket *)sender();
    int i = connectedSockets.indexOf(clientSocket);
    clientSocket->close();
    connectedSockets.removeAt(i);
    names.removeAt(i);
    qDebug()<<names;
    emit(nameDeleted(i));
}
void ODGBroadCastAsker::getData(int i){
   // int i = 0;

    if (connectedSockets.count()> i)
    connectedSockets.at(i)->write("Give data!\r\n");


}
void ODGBroadCastAsker::getConcreteData(int i,QString name){
   // int i = 0;

    if (connectedSockets.count()> i)
    connectedSockets.at(i)->write("#$#DIRECTLY#$#::"+name.toLocal8Bit()+"\r\n");


}
void ODGBroadCastAsker::getList(int i){
    if (connectedSockets.count()> i){
    qDebug()<<"Write to sock";
        connectedSockets.at(i)->write("Give me list!$!\r\n");
}
}

void ODGBroadCastAsker::getBigList(){
      listStart = true;
      sendOutBuffer.clear();
      waitForData.setInterval(5000);
      connect (&waitForData,SIGNAL(timeout()),this,SLOT(collectionReady()));
    for (int i = 0; i < connectedSockets.count(); i++){
        connectedSockets.at(i)->write("Give me list!$!\r\n");
    }
}
void ODGBroadCastAsker::collectionReady(){
    listStart = false;
    emit (dataGot(sendOutBuffer));
}

void ODGBroadCastAsker::readyReadClient(){
    QTcpSocket *clientSocket = (QTcpSocket *)sender();
    QByteArray data = clientSocket->readAll();
    QString txt = QString(data);
    int i = connectedSockets.indexOf(clientSocket);
    QString out = QString::fromUtf8(data.data(),data.count());

    qDebug()<<out;
    if (txt.contains("name:")){
        qDebug()<<data.toHex();
        //ushort  convArray[999999];

        qDebug()<<out;
        //qDebug()<<QString::fromUtf16(convArray,data.size()/2);
        QString newName = out.split(":").at(1).trimmed();
        if (names.at(i) != newName){
            emit (nameChanged(i, newName));
        }
        names.replace(i,newName);

    } else
        //if(txt.contains("<<odgdata>>"))
        {
        qDebug()<<"new odg data";
        qDebug()<<out;

        //if (!listStart)
        emit(dataGot(out));
        //else
          //  sendOutBuffer.append(out);
    }
    if (names.at(i).isEmpty()) clientSocket->write("What is your name?");
    qDebug()<<names;
}

int ODGBroadCastAsker::getClientCount(){
    return connectedSockets.size();
}

void ODGBroadCastAsker::onTimer(){

    QSettings settings("config.ini",QSettings::IniFormat);
    QString broadcastIp = settings.value("broadcast_ip", QVariant("192.168.43.255")).toString();

    for (int i =0; i< connectedSockets.length(); i++){
        connectedSockets.at(i)->write(QByteArray("What is your name?\r\n"));
        qDebug()<<QDateTime::currentDateTime().time().toString()<<connectedSockets.at(i)->peerAddress().toString()<<names.at(i);
    }
    //qDebug()<<"Broadcast start"<<QTime::currentTime();
    QByteArray aarr;
    aarr.append(QDateTime::currentDateTime().time().toString());
    QByteArray datagram = "ODG?"+ aarr;

    socket->writeDatagram(datagram.data(), datagram.size(), QHostAddress(broadcastIp), 4969);
    qDebug()<<"Broadcast " << datagram.data();
}

