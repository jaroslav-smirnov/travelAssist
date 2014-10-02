#ifndef ODGADDPARTITION_H
#define ODGADDPARTITION_H

#include <QDialog>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
namespace Ui {
class odgAddPartition;
}

class odgAddPartition : public QDialog
{
    Q_OBJECT
    
public:
    explicit odgAddPartition(QWidget *parent = 0);
    ~odgAddPartition();
public slots:
           void selectRow(QModelIndex);
           void addPartition();
private:
    Ui::odgAddPartition *ui;
    int id;
signals:
    void newPartAdd();
};

#endif // ODGADDPARTITION_H
