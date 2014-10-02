#ifndef ODGCONFDIALOG_H
#define ODGCONFDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>
namespace Ui {
class ODGConfDialog;
}

class ODGConfDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ODGConfDialog(QWidget *parent = 0);
void setTypeTwo();
    ~ODGConfDialog();
public slots:
    void pushAdd();
signals:
    void confirmed(QString * whoGet);
private:
    Ui::ODGConfDialog *ui;
    void init(void);
};

#endif // ODGCONFDIALOG_H
