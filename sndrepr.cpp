#include "sndrepr.h"
#include "ui_sndrepr.h"

sndRepr::sndRepr(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sndRepr)
{
    ui->setupUi(this);
    connect(&tcpSocket,SIGNAL(connected()),this,SLOT(onConnected()));
    connect(&tcpSocket,SIGNAL(readyRead()),this,SLOT(onDataGet()));
    connect(&tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(newError(QAbstractSocket::SocketError)));
    i = 0;
    lst.clear();
}

sndRepr::~sndRepr()
{
    delete ui;
}

void sndRepr::send_data(QString data, QString to_email)
{
    freec = false;
    //qDebug()<<"executed";
    lst<<"EHLO yandex.ru\r\n";
    lst<<"AUTH LOGIN\r\n";
    lst<<"amFyb3NsYXYuc21pcm5vdi5r\r\n";
    lst<<"SkExcm8yU2w=\r\n";
    lst<<"mail from:<jaroslav.smirnov.k@yandex.ru>\r\n";
    lst<<"rcpt to:<" + to_email + ">\r\n";
    lst<<"data\r\n";
    lst<<"To: " + to_email + "\r\n"
         "Content-type: text/html; charset=\"utf8\"\r\n"
         "Subject: Information from ODG control \r\n"
         "From: jaroslav.smirnov.k@yandex.ru\r\n\r\n" +
         data +
         "\r\n.\r\n";
    qDebug()<<tcpSocket.state();
    if (tcpSocket.state()==QAbstractSocket::UnconnectedState)
        tcpSocket.connectToHost("smtp.yandex.ru",25);

}
void sndRepr::onConnected()
{
    qDebug()<<"connected";
}
void sndRepr::onDataGet()
{
    qDebug()<<tcpSocket.read(256);


    if (i<lst.length())
    {
        //qDebug()<<lst.at(i).toUtf8();
        tcpSocket.write(lst.at(i).toUtf8());
        i++;
    }
    else
    {
        //qDebug()<<"list is empty";
        i = 0;
        lst.clear();
        tcpSocket.write(QString("quit").toAscii());
        tcpSocket.disconnectFromHost();
    }

}
bool sndRepr::isFree()
{
    return freec;
}

void sndRepr::newError(QAbstractSocket::SocketError error)
{
    qDebug()<<error;
    qDebug()<<tcpSocket.errorString();
    QMessageBox::warning(this,QString::fromLocal8Bit("Ошибка подключения к сокету"), tcpSocket.errorString());
    log->addEvent(tcpSocket.errorString());
}
