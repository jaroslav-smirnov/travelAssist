#include "testwindow.h"
#include "ui_testwindow.h"
#include "odgbroadcastasker.h"

testWindow::testWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::testWindow)
{
    ui->setupUi(this);
    ODGBroadCastAsker *asker;
    asker= new ODGBroadCastAsker(this);
    connect(ui->pushButton_getData,SIGNAL(clicked()),asker,SLOT(getData()));
}

testWindow::~testWindow()
{
    delete ui;
}
