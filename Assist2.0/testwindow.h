#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QTextDocument>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDate>
#include <QTime>
#include <QAbstractItemView>

#include "odgxmlparser.h"
#include"odgconfdialog.h"
#include "odgaddpartition.h"
#include "odgaddcategory.h"
#include "selectitems.h"
#include "odgselectreport.h"
#include "odgdataaddmaster.h"
#include <QSettings>
#include "../broadcastSender/odgbroadcastasker.h"

namespace Ui {
class TestWindow;
}

class TestWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit TestWindow(QWidget *parent = 0);
    void initUserView();
    QString getLetter(void);

    ~TestWindow();

public slots:
    void startBatchAdding();
    void clickDelete();
    void itemDataChanged(QStandardItem *);
    void askConcreteData(QString data);
    void askListOfData(QStringList datalist);
    void addPartition();
    void addCategorie();
    void addData();
    void confirmlyAdd(QString * who);
    void viewPoints(int catId);
    void selectRowFromPart ( QModelIndex );
    void selectRowFromCat ( QModelIndex );
    void showReport();
    void viewPartitions();
    void viewCategories(int id);
void customMenuRequested(QPoint pos);
    void askerNewUser(int i,QString name, QString ip);
    void askerChngUser(int i, QString name);
    void askerDelUser(int i);
    void askerNewData(QString data);
    void findData();
    void askerGetDataToUs();
    void askerGetListToUs();

    void fromUSB(void);
    void acceptChanges();
    void changesAccepted(QString *whoAccept);

    private:

    void removePoint(int id);
    void updateValue(QString str, int id);
    void updateDefects(QString str, int id);

    ODGBroadCastAsker *asker;
    void workData(OdgXmlParser *parser);

    Ui::TestWindow *ui;
    QFileDialog *dlg;
    QSqlDatabase db; /**< Global. Database variable */
int current_cat_id;
int current_part_id;
QModelIndex lastIndexPoints;
QString wName;
QString bufferData;
QMenu *menu;
QAction *action;

};

#endif // TESTWINDOW_H
