#include "infooutput.h"
#include "ui_infooutput.h"

InfoOutput::InfoOutput(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InfoOutput)
{
    ui->setupUi(this);
}


void InfoOutput::setOutput(QString *data)
{
    //ui->webView->setHtml(*data);
}

InfoOutput::~InfoOutput()
{
    delete ui;
}
