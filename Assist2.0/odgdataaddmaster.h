#ifndef ODGDATAADDMASTER_H
#define ODGDATAADDMASTER_H

#include <QDialog>
#include <QList>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include "../broadcastSender/odgbroadcastasker.h"
#include "phonefileitem.h"
#include <QStandardItem>
#include <QStandardItemModel>
#include "odgxmlparser.h"
#include "odgsweeppoint.h"
#include "odgdivision.h"

namespace Ui {
class ODGDataAddMaster;
}

class ODGDataAddMaster : public QDialog
{
    Q_OBJECT
signals:
    void viewPoints(int);
    
public:
    explicit ODGDataAddMaster(QWidget *parent = 0);
    ~ODGDataAddMaster();
    void setODGBroadcaster(ODGBroadCastAsker * asker, int curCat);
public slots:
    void gotData(QString);
    void activeViewChanged(int view);
    void acceptData();
private:
    Ui::ODGDataAddMaster *ui;
    float percent;
    int currentI;
    QString dataList;
    int count;
    ODGBroadCastAsker * asker;
    QList<PhoneFileItem *> phoneFiles;
    QList<PhoneFileItem *> selectedItems;
    bool comparePhoneFiles(PhoneFileItem * a, PhoneFileItem * b);
    void sortPhoneFiles();
    void setItems();
    QString bufferData;
    int selectedItemsSize;
    void addFileDataToTable(OdgXmlParser *parser);
    void initLastPane();
    void addPointsFromTable(QString makerName, QString getterName);
    int current_cat_id;

};

#endif // ODGDATAADDMASTER_H
