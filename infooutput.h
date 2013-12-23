#ifndef INFOOUTPUT_H
#define INFOOUTPUT_H

#include <QDialog>


namespace Ui {
class InfoOutput;
}

class InfoOutput : public QDialog
{
    Q_OBJECT
    
public:
    void setOutput(QString *data);
    explicit InfoOutput(QWidget *parent = 0);
    ~InfoOutput();
    
private:
    Ui::InfoOutput *ui;
};

#endif // INFOOUTPUT_H
