#ifndef ODGSELECTREPORT_H
#define ODGSELECTREPORT_H

#include <QDialog>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QFile>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include "odgorptgw.h"
namespace Ui {
class ODGSelectReport;
}

class ODGSelectReport : public QDialog
{
    Q_OBJECT
    
public:
    explicit ODGSelectReport(QWidget *parent = 0);

    ~ODGSelectReport();
public slots:
    void showRep(int categoryId, QString dbAddr, QString dbName, int port,  QString name, QString password);
    void pressOk();
private:
    Ui::ODGSelectReport *ui;
    int id;
    QString dbAddr;
    QString dbName;
    int port;
    QString name;
    QString password;
};

#endif // ODGSELECTREPORT_H
