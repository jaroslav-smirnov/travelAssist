#ifndef SNDREPR_H
#define SNDREPR_H

#include <QDialog>
#include <QTcpSocket>
#include <QMessageBox>
#include "logger.h"

namespace Ui {
class sndRepr;
}

class sndRepr : public QDialog
{
    Q_OBJECT
    
public:
    explicit sndRepr(QWidget *parent = 0);
    ~sndRepr();
    void send_data(QString data, QString to_email);
    bool isFree();
    logger *log;
public slots:
    void onConnected(void);
    void onDataGet(void);
    void newError(QAbstractSocket::SocketError error);

    
private:
    Ui::sndRepr *ui;
    QTcpSocket tcpSocket;
    QStringList lst;
    int i;
    bool freec;
};

#endif // SNDREPR_H
