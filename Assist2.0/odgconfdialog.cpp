#include "odgconfdialog.h"
#include "ui_odgconfdialog.h"

ODGConfDialog::ODGConfDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ODGConfDialog)
{
    ui->setupUi(this);
    init();
}
void ODGConfDialog::setTypeTwo(){
    ui->pushButton->setText(QString::fromLocal8Bit("�����������"));
    this->setWindowTitle(QString::fromLocal8Bit("����������� ���������"));
    ui->label->setText(QString::fromLocal8Bit("��� ������������� ��������� �������� ���� �� ������ � ������� ������."));
}

ODGConfDialog::~ODGConfDialog()
{
    delete ui;
}
void ODGConfDialog::init(){
    QSqlQuery query;
    QString queryStr;
    queryStr="SELECT \"NAME\" FROM users ORDER BY \"NAME\";";
    bool ok = query.exec(queryStr);
    if(!ok) {
        qDebug()<<query.lastError();
        return;
    }
    QStringList names;
    while (query.next()){
        names.append(query.value(0).toString());
    }
    ui->comboBox->addItems(names);
    connect(ui->pushButton,SIGNAL(clicked()), this, SLOT(pushAdd()));

    QSettings settings("config.ini",QSettings::IniFormat);
    int curPos = settings.value("confirm_pos", QVariant(0)).toInt();
    ui->comboBox->setCurrentIndex(curPos);
    ui->lineEdit->setFocus();
}
void ODGConfDialog::pushAdd(){

    QSqlQuery query;
    QString queryStr;

    QSettings settings("config.ini",QSettings::IniFormat);

    int curPos = ui->comboBox->currentIndex();
    settings.setValue("confirm_pos", QVariant(curPos));
    settings.sync();


    queryStr="SELECT \"NAME\" FROM users WHERE \"NAME\"= '%1' AND \"CODE\" = %2;";
    bool ok = query.exec(queryStr.arg(ui->comboBox->currentText())
                         .arg(QString::number(ui->lineEdit->text().toInt())));
    if(!ok) {
        qDebug()<<query.lastError();
        return;
    }
    qDebug()<<query.lastQuery();
    qDebug()<<query.size();
    if (query.size()<1){
        QMessageBox::warning(this,QString::fromLocal8Bit("�������� ������"),QString::fromLocal8Bit("���������� ��������� ��������� ���� ������."));
        return;
    }
    else
    {
        QString *who = new QString();
        *who = ui->comboBox->currentText();

        emit confirmed(who);
        this->close();
    }

}
