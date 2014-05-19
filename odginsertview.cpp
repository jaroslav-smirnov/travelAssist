#include "odginsertview.h"
#include "ui_odginsertview.h"

ODGInsertView::ODGInsertView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ODGInsertView)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);

    type = 0;
    reallyClose = false;
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(okClick()));
    connect(ui->pushButton_2,SIGNAL(clicked()), this, SLOT(cancelClick()));
    ui->label->hide();
    ui->lineEdit->hide();
    this->setWindowTitle(QString::fromLocal8Bit("Предварительный просмотр вносимых данных"));
}

ODGInsertView::~ODGInsertView()
{
    delete ui;
}
void ODGInsertView::setView(QTextDocument *doc){
    accepted = false;
    ui->textEdit->setDocument(doc);
}

void ODGInsertView::closeEvent(QCloseEvent *event){
    if (this->reallyClose)
    {
        this->reallyClose = false;
        event->accept();
    }
    else
    {
        QMessageBox::information(this,QString::fromLocal8Bit("Необходимо выбрать ответ"),
                                 QString::fromLocal8Bit("Пожалуйста выберите ответ с помощью клавиш расположенных внизу."));
        event->ignore();
    }
}
void ODGInsertView::okClick(){
    //QSqlDatabase::database().commit();

    this->reallyClose = true;
    accepted = true;
    emit (readyOk());
    this->close();


}

void ODGInsertView::cancelClick()
{
   // QSqlDatabase::database().rollback();
    this->reallyClose = true;
    accepted = false;
    emit(readyNo());
    this->close();
}
bool ODGInsertView::isAccepted()
{
    return accepted;
}
