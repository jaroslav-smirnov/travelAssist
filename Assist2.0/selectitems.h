#ifndef SELECTITEMS_H
#define SELECTITEMS_H

#include <QDialog>
#include <QDebug>
#include <QStandardItemModel>

namespace Ui {
class SelectItems;
}

class SelectItems : public QDialog
{
    Q_OBJECT
  public slots:
    void pushOk();
public:
    explicit SelectItems(QWidget *parent = 0);
    void setItemsFromString(QString str);
    ~SelectItems();

signals:
    void haveDataName(QString path);
    void haveSeveralNames(QStringList names);
private:
    Ui::SelectItems *ui;
};

#endif // SELECTITEMS_H
