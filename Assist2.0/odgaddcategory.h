#ifndef ODGADDCATEGORY_H
#define ODGADDCATEGORY_H

#include <QDialog>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

namespace Ui {
class ODGAddCategory;
}

class ODGAddCategory : public QDialog
{
    Q_OBJECT
    
public:
    explicit ODGAddCategory(QWidget *parent = 0);
    void setPartition(int partId);
    ~ODGAddCategory();
public slots:
    void selectRow(QModelIndex);
    void addCat();
signals:
    void newCatAdd(int);
private:
    Ui::ODGAddCategory *ui;
    int partId;
    int id;
};

#endif // ODGADDCATEGORY_H
