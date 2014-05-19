#ifndef ODGINSERTVIEW_H
#define ODGINSERTVIEW_H

#include <QDialog>
#include <QTextDocument>
#include <QCloseEvent>
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
namespace Ui {
class ODGInsertView;
}

class ODGInsertView : public QDialog
{
    Q_OBJECT
    
public:
    explicit ODGInsertView(QWidget *parent = 0);
    ~ODGInsertView();
    void setView(QTextDocument *doc);
    bool waitFunction();
    void closeEvent(QCloseEvent *);
    bool isAccepted();
public slots:
    void okClick();
    void cancelClick();
signals:
    void readyOk();
    void readyNo();
    
private:
    Ui::ODGInsertView *ui;
    QTextDocument doc;
    short type;
    bool reallyClose;
    bool accepted;
};

#endif // ODGINSERTVIEW_H
