#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEventLoop>
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QPrintDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QErrorMessage>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QSet>
#include <QCloseEvent>

#include <QSettings>

#include "logger.h"
#include "odginsertview.h"



#include<QDebug>
#include "fileentry.h"
#include "sndrepr.h"

#include "getpointslistfromdb.h"

#define xor_key 77


namespace Ui {
    class MainWindow;
}

/**
 * @brief This class was made to work with all interfaces such as ui, database and mobile device.
 *
 * @class MainWindow mainwindow.h "mainwindow.h"
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void onButtonClick(bool test);

public slots:

    void pushTest();
    void pushAdd();

public:
    void printDialogExec(void);
    int dbInit(void);
    bool checkExists(int id, int generation,int division, QDate date, QTime time);
    void analyzeFile();
    bool workWithFile(QString string, bool test);
    bool sendReport(int repType, QString description, QString by);
    bool fillByPoint(fileEntry entry);
    int getIndexOfSweep(QString whoTake);
    int addNewSweep(QString,QString);
    void sendEMAILSifNess(QString *alStrHtml);
    void cleanValues();
    void alarmData();
    QString finishTable(QString nameOfworker, QString nameOfMaster);

    QString getNameOfDivision (int id);
    QStringList *getPointsToSweep(int id);
    void removeFromSweep(QString d, int id);
    QString reportSweepsHTML(int id);
    QString prepareDocumentForSign();
    QString signDocument(QString textToSign);
    void closeEvent(QCloseEvent *);

private:
    Ui::MainWindow *ui;
    QSqlDatabase db; /**< Global. Database variable */
    QString operODG; /**< Global. Name of operator */
    int curId;
    QString getLetter(void);
    int numberOfpoints[100];
    QString WORK_DIR;

    QString oneEmailNameOfMaster;
    QString oneEmailNameOfOper;

    int alarmsCount;

    void printDataWithoutDB();
    bool willSign;
    bool isPrintable;
    logger log;

    QString globalLetter;
    ODGInsertView insertView;

    QTextDocument doc2print; /**< Global. Document to storage text to print */
    QErrorMessage errorMessageDialog; /**< Global. Message about wrong checkcode */
    sndRepr x;
    QString *ValuesToPrint[100];
    QStringList *ValuesToSweep[100];
    void outListInit();
    void addToString(QString str, int division);
    void finishLines(QString nameOfworker, QString nameOfMaster);
    void printData(QPrinter *);

    QString XORkey(QString data);

    QString pathToWalkData;

    QString dataListToString(QStringList *slst);


};

#endif // MAINWINDOW_H
