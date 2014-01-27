#include "mainwindow.h"
#include "ui_mainwindow.h"

//#define WORK_DIR  "/ODGFiles/"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->alarmsCount = 0;
    ui->setupUi(this); /**< Connect user interface with class */
    //setlocale(LC_CTYPE,"rus");
    outListInit();
    //qDebug()<<XORkey("0000")<<XORkey(XORkey("0000"));

    //ui->label_6->setVisible(false);
    qDebug()<<QApplication::applicationDirPath();
    qDebug()<<QApplication::libraryPaths();
    x.log = &log;
    if (!dbInit()){
        ui->pushButton->setEnabled(false);
        return; /**< If we couldn't initialise database stop */
    }
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(pushAdd())); /**< COnnect button from ui to slot of this class */
    connect (ui->pushButton_2,SIGNAL(clicked()), this, SLOT(pushTest()));
ui->lineEdit->setEchoMode(QLineEdit::Password);
    ///****************************************************

    // x.send_data(QString::fromLocal8Bit("У нас есть неплохой способ отправлять отчеты через e-mail на як, теперь с поддержкой очереди"),"jaroslav.smirnov.k@gmail.com");
}

/**
 * @brief Gets where mobile device is connected and what is the last file
 *
 * @fn MainWindow::getLetter
 * @return QString letter of drive where mobile device is
 */
QString MainWindow::getLetter(void)
{
    QStringList filters; /**< Create new string list to storage filter value */
    filters << "clean*"; /**< Add filter "clean*" to avoid raw files using */
    QDir ourDir; /**< Create new QDir variable to storage dir value of mobile device*/
    ourDir.setNameFilters(filters);/**< Set filter "clean*" to this dir to use only clean files */


    QFileInfoList list = QDir::drives(); /**< Create new list to storage drives on this computer. */
    /**< Get this drived */
    QString letter; /**< Create QString to storage this letter */
    while (!list.isEmpty()) /**< Go to loop to check the all drives on computer */
    {
        letter = list.takeLast().absolutePath()+WORK_DIR; /**< Create full path to system directory */
        ourDir.setPath(letter);/**< Set this path */
        if (ourDir.exists()) /**< If this path exists  */
        {
            if (!ourDir.entryList().isEmpty())/**<      If thre are files */
            {
                globalLetter = letter;

                pathToWalkData = letter + QDate::currentDate().toString("dd.MM.yyyy.csv");
                return letter + ourDir.entryList(QDir::NoFilter,QDir::Time).first();/**<            Return the filepath */

            }

            return "false";/**< In other cases return "false" */
        }
    }
    return "false";
}
/**
 * @brief It checks existation of value with given id
 *
 * @fn MainWindow::checkExists
 * @param id number of point
 * @param generation version of code
 * @param date Date of check
 * @param time Time of check
 * @return bool Was execution correct
 */
bool MainWindow::checkExists(int id, int generation, int division,QDate date, QTime time)
{
    QSqlQuery query; /**< Create new query to contain sql query */
    QString dateTime = date.toString("dd.MM.yyyy")+"."+time.toString("hh:mm:ss");
    /**< Create new string to contain good formate of date */

    /**< Create query and exucute it */
    bool ok = query.exec(" SELECT \"eBoxes\".\"NAME\",\"eBoxes\".\"GENERATION\",\"eBoxes\".\"LAST_CHECKED\"FROM public.\"eBoxes\" WHERE \"eBoxes\".\"NAME\" = "
                         + QString::number(id) + "AND \"eBoxes\".\"GENERATION\" = "
                         + QString::number(generation) + "AND \"eBoxes\".\"DIVISION\" = "
                         +QString::number(division)+";");
    if (!ok) /**< If error return false */
    {
        qDebug()<<"Error "<<query.lastError();
        QMessageBox::warning(this,QString::fromLocal8Bit("Ошибка запроса к базе данных"),query.lastError().text());

        qDebug()<<query.result();

        return false;
    }
    if ((query.size() == 1)||id==0)/**< If one record from query, analyze it else return false */
    {
        ok = query.exec("UPDATE public.\"eBoxes\" SET \"LAST_CHECKED\" = '" + dateTime + "', \"DATECHANGE\" = '"
                        + QDateTime::currentDateTime().toString("dd.MM.yyyy.hh:mm:ss") +
                        "' WHERE \"NAME\" = " + QString::number(id) + ";");
        if (!ok)
        {
            qDebug()<<"Error "<<query.lastError();
            QMessageBox::warning(this,QString::fromLocal8Bit("Ошибка запроса к базе данных"),query.lastError().text());
            qDebug()<<query.result();
            return false;
        }
        return true;
    }
    else
    {
        sendReport(1, QString::number(generation)+","+QString::number(division)+","+QString::number(id), operODG);
        /**< If there are another number of records from query
  send report to database*/

        return false;
    }

}
/**
 * @brief Initialize database
 *
 * @fn MainWindow::dbInit
 * @return int Was execution correct?
 */
int MainWindow::dbInit()
{
    QSettings settings("config.ini",QSettings::IniFormat);


    db = QSqlDatabase::addDatabase("QPSQL"); /**< Set database parameter to PostgreSQL */
    db.setHostName(settings.value("host",QVariant("localhost")).toString());
    db.setPort(settings.value("port",QVariant(5432)).toInt());
    db.setDatabaseName(settings.value("db",QVariant("odgControl")).toString());
    db.setUserName(settings.value("user_name",QVariant("postgres")).toString());
    db.setPassword(XORkey(settings.value("password",QVariant(XORkey("0000"))).toString())); /**< Set password to *** */
    isPrintable = settings.value("is_printable",QVariant(true)).toBool();

    WORK_DIR = settings.value("scaner_dir", QVariant("/ODGFiles/")).toString();

    willSign = settings.value("EDS",QVariant(false)).toBool();

    settings.setValue("host", QVariant(db.hostName()));
    settings.setValue("port", QVariant(db.port()));
    settings.setValue("db", QVariant(db.databaseName()));
    settings.setValue("user_name", QVariant(db.userName()));
    settings.setValue("EDS",QVariant(willSign));
    settings.setValue("password",QVariant(XORkey(db.password())));

    settings.setValue("scaner_dir",QVariant(WORK_DIR));
    settings.setValue("is_printable",QVariant(isPrintable));


    //settings.sync();

    bool ok = db.open(); /**< Create new boolean to contain result of opening andopen db */
    if (!ok)
    {
        qDebug()<<"We have a problem "<<db.lastError();

        QMessageBox::warning(this,QString::fromLocal8Bit("Ошибка базы данных"),QString::fromLocal8Bit("Ошибка при подключении к базе данных, пробуем подключиться к резервной базе данных!"));

        db.setHostName(settings.value("redundand_host",QVariant("localhost")).toString());
        /*bool ok = db.open(); Create new boolean to contain result of opening andopen db
        if (!ok)
        {
        QMessageBox::warning(this,QString::fromLocal8Bit("Ошибка резервной базы данных"),QString::fromLocal8Bit("Ошибка при подключении к резервной базе данных, отчет будет выведен на печать через выбранный принтер! СООБЩИТЕ ОБ ИНЦЕДЕНТЕ АДМИНИСТРАТОРУ СИСТЕМЫ!"));
        }
        else{
            QMessageBox::information(this,QString::fromLocal8Bit("Система подключена к резервной базе данных"),QString::fromLocal8Bit("Вы работаете с резервной базой данных! СООБЩИТЕ ОБ ИНЦЕДЕНТЕ АДМИНИСТРАТОРУ СИСТЕМЫ!"));
            return 1;
        }*/
        printDataWithoutDB();
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->lineEdit->setEnabled(false);

        return 0;
    }
    return 1;
}

void MainWindow::printDataWithoutDB()
{
    QString string = getLetter();

    if (string == "false") return;/**< Check string */

    QFile file(string);/**< Create QFile file to handle file from mobile device */

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))/**< Open file and check result */
        return;
    QTextStream out(&file); /**< Create new stream to work with file on mobile device */
    out.setCodec("UTF-8");  /**< Set codec to UTF-8 */
    QString line;  /**< Create new string to contain the lines */
    while (!out.atEnd()) /**< Read the all file */
    {
        line.append(out.readLine()); /**< Set Buffer string to text line from mobile device */
        line.append("<br/>");
    }

    line.append(QString::fromLocal8Bit("ФИО принявшего лица:______________   Дата:___________ Подпись:___________"));

    QTextDocument doc2print;
    doc2print.setHtml(line);

    QPrinter printer; /**< Create new variable to use printer */
    QPrintDialog dialog(&printer,this); /**< Start new printer dialog and create it */
    dialog.exec();
    //if (dialog.exec() != QDialog::Accepted) return;  /**< In case of cancel button stop */

    doc2print.print(&printer);


    file.close();  /**< Close the file */


}

/**
 * @brief Working with file of given path
 *
 * @fn MainWindow::workWithFile
 * @param string path of file
 * @return bool result of execution
 */
bool MainWindow::workWithFile(QString string, bool test)
{
    if (string == "false") return false;/**< Check string */

    QFile file(string);/**< Create QFile file to handle file from mobile device */

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))/**< Open file and check result */
        return false;
    fileEntry buf;/**< Create buffer value to contain one string from our class fileEntry */
    QString bufferStrHTML; /**< Create string to contain HTML code */
    QString alarmStrHTML;
    QString nameOfworker; /**< Create string to containt name of worker */
    QString nameOfMaster = operODG; /**< Create string with name of master and set this name to operODG */

    bufferStrHTML.append("<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\">"); /**< Append HTML code to string */
    bufferStrHTML.append(buf.addHeaderHTML()); /**< Append header function from buf variable */
    alarmStrHTML.append("<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\">");
    alarmStrHTML.append(buf.addHeaderHTML());

    QTextStream out(&file); /**< Create new stream to work with file on mobile device */
    out.setCodec("UTF-8");  /**< Set codec to UTF-8 */
    //out.readLine(); /**< Read line from stream */
    QString line;  /**< Create new string to contain the lines */
    while (!out.atEnd()) /**< Read the all file */
    {
        line  = out.readLine(); /**< Set Buffer string to text line from mobile device */

        qDebug()<<"our line is "<< line;
        if (!buf.fromString(line)) return false;  /**< Get data to buf from string if it is false stop function with false */

        qDebug()<< "Is it name of worker?";
        if (buf.getId() == 0)  /**< If the id 0, this string contains name of worker */
        {nameOfworker = buf.getPointName();
            qDebug()<< "yes it is " <<nameOfworker;
        }
        else
        {
            bufferStrHTML.append(buf.getString2WriteHTML());  /**< Add new row to table */


            //****************************************************************
            addToString(buf.getString2WriteHTML(), buf.getDivision());



            if (!buf.isOk()) {
                alarmStrHTML.append(buf.getString2WriteHTML(getNameOfDivision(buf.getDivision())+"."));
             alarmsCount++;
            }

        }

        ui->label_5->setText(nameOfworker);  /**< Set the label in ui to the name of worker */

        checkExists(buf.getId(), buf.getVersion(),buf.getDivision(), buf.getDate(), buf.getTime());  /**< Check exist of this value in database */
        //qDebug()<<"fill by new point";

        if (!test) fillByPoint(buf);  /**< Fill the database by this point */

    }

    bufferStrHTML.append(finishTable(nameOfworker, nameOfMaster));  /**< Finish HTML code */

    if (this->alarmsCount == 0){
        alarmStrHTML.append(QString::fromLocal8Bit("<h1>Нет новых дефектов</h1>"));
    }
    alarmStrHTML.append(finishTable(nameOfworker, nameOfMaster));  /**< Finish HTML code */


    finishLines(nameOfworker,nameOfMaster);


    if (!test)     sendEMAILSifNess(&alarmStrHTML);
    file.close();  /**< Close the file */
    return true;  /**< Return true */


}


QString MainWindow::finishTable(QString nameOfworker, QString nameOfMaster)
{
    return         "</table>"  /**< Finish HTML code */
            "<br><table border=\"0\" cellspacing=\"0\" cellpadding=\"5\">"
            "<tr><td>"+ QString::fromLocal8Bit("Обход выполнил")+"</td><td>" +nameOfworker +"</td><td>" +
            QDate::currentDate().toString("dd.MM.yyyy") + "</td><td>___________________</td></tr>"
            "<tr><td>"+ QString::fromLocal8Bit("Обход принял")+"</td><td>"+ nameOfMaster +"</td><td>" +
            QDate::currentDate().toString("dd.MM.yyyy") + "</td><td>___________________</td></tr>"
            "</table>";
}

/**
 * @brief Send report about bad situation
 *
 * @fn MainWindow::sendReport
 * @param repType type of report
 * @param description of report
 * @param by who was using system when report was sent
 * @return bool was execution correct
 */
bool MainWindow::sendReport(int repType, QString description, QString by)
{
    QSqlQuery query; /**< Query to make new record to database */
    bool ok = query.exec( "INSERT INTO public.\"reports\" (\"TYPE_OF_REPORT\",\""
                          "DESCRIPTION\",\"DATE\",\"BY\") VALUES ( "
                          + QString::number(repType)+", '" + description +
                          "', '"+QDateTime::currentDateTime().toString()+"','"+by+"');");
    if (!ok)
    {

        qDebug()<<query.lastError();
        QMessageBox::warning(this,QString::fromLocal8Bit("Ошибка запроса к базе данных"),query.lastError().text());
        return false;
    }
    return true;
}



/**
 * @brief Destructor
 *
 * @fn MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    cleanValues();
    delete ui;
}
/**
 * @brief Execution of print dialog to setup printer properties
 *
 * @fn MainWindow::printDialogExec
 */
void MainWindow::printDialogExec(void)
{
    if (!isPrintable) return;

    QPrinter printer; /**< Create new variable to use printer */
    QPrintDialog dialog(&printer,this); /**< Start new printer dialog and create it */
    if (dialog.exec() != QDialog::Accepted) return;  /**< In case of cancel button stop */
    //printer.setOrientation(QPrinter::Landscape);  /**< Set orientation to landscape */
    //doc2print.print(&printer);  /**< print document */

    printData(&printer);
}

void MainWindow::pushAdd(){
    this->onButtonClick(false);
//ui->pushButton->setEnabled(false);
//ui->pushButton_2->setEnabled(false);
}

void MainWindow::pushTest(){    
    this->onButtonClick(true);
    //ui->pushButton->setEnabled(false);
    //ui->pushButton_2->setEnabled(false);
}

/**
 * @brief public slot to handle button click
 *
 * @fn MainWindow::onButtonClick
 */
void MainWindow::onButtonClick(bool test)
{

    //if (test) QMessageBox::warning(this, QString::fromLocal8Bit("Тест"),QString::fromLocal8Bit("Режим теста"));
    //this->cleanValues();


    QSqlQuery query; /**< Create uery to check code */
    bool ok = query.exec("SELECT users.\"NAME\" FROM public.users WHERE users.\"CODE\" = " +ui->lineEdit->text() + ";"); /**< Execute this query*/
    //qDebug()<<ok;
    if (!ok)
    {
        log.addEvent(query.lastError().text());
        qDebug()<<query.lastError();

        QMessageBox::warning(this,QString::fromLocal8Bit("Ошибка запроса к базе данных"),query.lastError().text());
        return;  /**< If there are error - stop execution */
    }


    if (query.size()!=1)
    {

        QMessageBox::warning(this,QString::fromLocal8Bit("Неверное имя пользователя"), QString::fromLocal8Bit("Пожалуйста проверьте введенный код"));
        return;  /**< If result size is not equal to one stop execution. Show message */
    }
    if (!test)    {/*ui->pushButton->setEnabled(false);*/}

    query.first();  /**< Get value of operator name */
    operODG = query.value(0).toString();
    ui->label_4->setText(operODG);  /**< Set text of operator name */
    //qDebug()<<operODG;
    //qDebug()<<"Let`s get index of sweep";
    if (!test) getIndexOfSweep(operODG);
    QString letter = getLetter();
    if (letter == "false")
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("Проблемы с соединением"),QString::fromLocal8Bit("Мобильное устройство не подключено или проблемы со связью. Проверьте соединительный провод."));
        log.addEvent("Device isn't connected");
        return;
    }
    //QSqlQuery newQuery;
    if (!test) GetPointsListFromDB::initScanner(&query,this->globalLetter);
    workWithFile(letter,test);  /**< Start file working function */

    if (test) this->alarmData();
    if (test) {
        /**********************************************************************************/
     this->cleanValues();
        /************************************************************************************/
        return;
    }
    addNewSweep(operODG, ui->label_5->text());
    printDialogExec();  /**< Execute printer function to show output*/
    if (willSign)
    {
        QString textToSign;
        textToSign = prepareDocumentForSign();
        QString author = signDocument(textToSign);
        QString boss = signDocument(textToSign);

        if ((author!=QString("no sign"))&&(boss!=QString("no sign")))
        {

            bool ok = query.exec( "INSERT INTO public.signed_docs (text, signature_make, signature_take) VALUES ( ' "+textToSign+" ','"
                                  +author +
                                  "', '" + boss +"');");
            if (!ok)
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,QString::fromLocal8Bit("Ошибка запроса к базе данных"),query.lastError().text());
            }
        }

        else  QMessageBox::warning(this,QString::fromLocal8Bit("Проблема с подписью"),QString::fromLocal8Bit("Цифровая подпись не получена"));


    }
    /*********************************************************************/
    QMessageBox::information(this, QString::fromLocal8Bit("Данные добавлены"),QString::fromLocal8Bit("Данные успешно внесены в базу данных"));

    QProcess::startDetached("odgView.exe");

    this->cleanValues();
    /********************************************************************/
}
/**
 * @brief Fill database by new entry
 *
 * @fn MainWindow::fillByPoint
 * @param entry to add to database
 * @return bool result of execution
 */
bool MainWindow::fillByPoint(fileEntry entry)
{
    QSqlQuery query; /**< Create query to add new record */
    //qDebug()<<entry.getQueryToSend();
    bool ok = query.exec(entry.getQueryToSend(curId)); /**< Execute statement by getQueryToSend() function */

    if (!entry.checkSUM()) QMessageBox::warning(this,QString::fromLocal8Bit("Данные повреждены"),
                                                QString::fromLocal8Bit("Цифровая подпись строки нарушена"));
    if (ok)return true;
    qDebug()<<query.lastError();
    qDebug()<<" Error in filling ";
    QMessageBox::warning(this,QString::fromLocal8Bit("Ошибка запроса к базе данных"),query.lastError().text());
    log.addEvent(query.lastError().text());
    return false;
}

int MainWindow::getIndexOfSweep(QString whoTake)
{
    QSqlQuery query; /**< Create query to add new record */
    //qDebug()<<query.value(0).toString();
    bool ok = query.exec( "SELECT nextval('sweeps_id_seq');");
    if (!ok)
    {
        qDebug()<<query.lastError();
        QMessageBox::warning(this,QString::fromLocal8Bit("Ошибка запроса к базе данных"),query.lastError().text());
        log.addEvent(query.lastError().text());
        return -1;
    }

    query.first();
    curId = query.value(0).toInt();

    return curId;
}

int MainWindow::addNewSweep(QString whoTake, QString whoMake)
{
    QSqlQuery query; /**< Create query to add new record */
    bool ok = query.exec( "INSERT INTO public.\"SWEEPS\" VALUES ( " + QString::number(curId) + ",'"
                          + QDate::currentDate().toString("dd.MM.yyyy")+
                          "', '" + QTime::currentTime().toString("hh:mm:ss") +
                          "','"+whoTake.trimmed()+"','" + whoMake.trimmed() +"');");
    if (!ok)
    {
        qDebug()<<query.lastError();
        QMessageBox::warning(this,QString::fromLocal8Bit("Ошибка запроса к базе данных"),query.lastError().text());
        return -1;
    }

    ok = query.exec( "SELECT last_value from \"sweeps_id_seq\";");
    if (!ok)
    {
        qDebug()<<query.lastError();
        QMessageBox::warning(this,QString::fromLocal8Bit("Ошибка запроса к базе данных"),query.lastError().text());
        log.addEvent(query.lastError().text());
        return -1;
    }

    query.first();
    curId = query.value(0).toInt();

    return curId;
}
void MainWindow::outListInit()
{
    for (int i=0; i<100; i++)
    {
        ValuesToPrint[i] = 0x0;
        numberOfpoints[i] = 0;
        ValuesToSweep[i] = 0x0;
    }
}

void MainWindow::addToString(QString str, int division)
{
    fileEntry buf;
    if (ValuesToPrint[division]==0x0)
    {
        ValuesToPrint[division] = new QString;
        ValuesToPrint[division]->append("<h2>" + QString::fromLocal8Bit("Результат обхода системы. ")+
                                        getNameOfDivision (division)+ "</h2><br>");
        ValuesToPrint[division]->append("<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\">"); /**< Append HTML code to string */
        ValuesToPrint[division]->append(buf.addHeaderHTML()); /**< Append header function from buf variable */
        ValuesToSweep[division] = getPointsToSweep(division);
    }
    ValuesToPrint[division]->append(str);
    qDebug()<<"This is number for remove" << str.mid(24,6);
    removeFromSweep(str.mid(24,6), division);

    numberOfpoints[division]++;
}

void MainWindow::finishLines(QString nameOfworker, QString nameOfMaster)
{
    for (int i=0; i<100;i++)
    {
        if (ValuesToPrint[i]!=0x0)
        {
            ValuesToPrint[i]->append("</table>");  /**< Finish HTML code */

            ValuesToPrint[i]->append("<br><table border=\"0\" cellspacing=\"0\" cellpadding=\"5\">");
            ValuesToPrint[i]->append("<tr><td>"+ QString::fromLocal8Bit("Обход выполнил")+"</td><td>" +nameOfworker +"</td><td>" +
                                     QDate::currentDate().toString("dd.MM.yyyy") + "</td><td>___________________</td></tr>");
            ValuesToPrint[i]->append("<tr><td>"+ QString::fromLocal8Bit("Обход принял")+"</td><td>"+ nameOfMaster +"</td><td>" +
                                     QDate::currentDate().toString("dd.MM.yyyy") + "</td><td>___________________</td></tr>");
            ValuesToPrint[i]->append("</table>");
            ValuesToPrint[i]->append(reportSweepsHTML(i));
        }
    }
}

void MainWindow::printData(QPrinter *printer)
{
    QTextDocument doc;
    for (int i=0; i<100; i++)
    {
        if (ValuesToPrint[i]!=0x0)
        {
            doc.clear();
            doc.setHtml(*ValuesToPrint[i]);
            doc.print(printer);
        }
    }
}

void MainWindow::alarmData ()
{
    QString buffer;
    int pos1, pos2;
    for (int i=0; i<100; i++)
    {
        if (ValuesToPrint[i]!=0x0)
        {

            buffer = *ValuesToPrint[i];
            pos1 = buffer.indexOf("<table");
            pos2 = buffer.indexOf("</table>");
            buffer.remove(pos1, pos2-pos1 + QString("</table>").length());
            pos1 = buffer.indexOf("<table");
            pos2 = buffer.indexOf("</table>");
            buffer.remove(pos1, pos2-pos1 + QString("</table>").length());
            buffer.remove("<br><br>");
            buffer.remove("</h2><h2>");
            buffer.remove(QString::fromLocal8Bit("Результат обхода системы."));
            qDebug()<<buffer;
            QMessageBox::warning(this,QString::fromLocal8Bit("Предварительный просмотр отчета"),buffer);
            }
    }
}

QString MainWindow::dataListToString(QStringList *slst)
{
    QString result;

    QString buf;
    result.append("<br><table>");
    result.append(QString::fromLocal8Bit("<tr><td>Время</td><td>Кол-во шагов</td><td>Темп</td><td>Текущий уровень сигнала</td>"
                                         "<td>Макс</td><td>Сред</td><td>Мин</td></tr>"));
    for (int i = 0; i < slst->length(); i++){
        result.append("<tr><td>");
        buf = slst->at(i);
        result.append(buf.replace(";","</td><td>"));
        result.append("</td></tr>");
    }
    result.append("</table>");
    qDebug()<<result;
    return result;
}

void MainWindow::sendEMAILSifNess(QString *alStrHtml)
{
    QSqlQuery query;
    bool ok;
    QString email;

    /**************************************************************/




    QFile file(pathToWalkData);/**< Create QFile file to handle file from mobile device */

    QStringList dataList;
    QString line;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))/**< Open file and check result */
    {
        QTextStream out(&file); /**< Create new stream to work with file on mobile device */
        out.setCodec("UTF-8");  /**< Set codec to UTF-8 */
        while (!out.atEnd()) /**< Read the all file */
        {
            line  = out.readLine(); /**< Set Buffer string to text line from mobile device */
            dataList << line;
        }
        line ="";
    }





    /*****************************************************************/

    for (int i=0; i<100;i++)
    {
        if (ValuesToPrint[i]!=0x0)
        {
            /*qDebug()<<"SELECT \"bosesForReport\".\"To\" FROM public.\"bosesForReport\""
                                        "WHERE \"bosesForReport\".reports LIKE '%"
                                        + QString::number(i).rightJustified(2,QChar('0'))
                                        + "%';";*/
            ok = query.exec("SELECT \"bosesForReport\".\"To\" FROM public.\"bosesForReport\""
                            "WHERE \"bosesForReport\".reports LIKE '%"
                            + QString::number(i).rightJustified(2,QChar('0'))
                            + "%';");
            if (!ok)
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,QString::fromLocal8Bit("Ошибка запроса к базе данных"),query.lastError().text());
                log.addEvent(query.lastError().text());
            }
            if (query.size()>0)
            {
                while (query.next()) {
                    email = query.value(0).toString().trimmed();
                    // qDebug()<<email;
                    x.send_data(*ValuesToPrint[i] + line, email);
                }

            }
            else
                qDebug()<<"Nobody is interested in "<<i;
        }
    }

    ok = query.exec("SELECT \"bosesForReport\".\"To\" FROM public.\"bosesForReport\""
                    "WHERE \"bosesForReport\".alarms LIKE '%1%';");

    if (!ok)
    {
        qDebug()<<query.lastError();
        QMessageBox::warning(this,QString::fromLocal8Bit("Ошибка запроса к базе данных"),query.lastError().text());
        log.addEvent(query.lastError().text());
    }


    qDebug()<<"Email "<<*alStrHtml;
    if (query.size()>0)
    {
        while (query.next()) {
            email = query.value(0).toString().trimmed();
            //qDebug()<<email;
            x.send_data(*alStrHtml, email.trimmed());

        }

    }
    else
        qDebug()<<"Nobody is interested in ";

}


void MainWindow::cleanValues()
{
    for (int i=0; i<100;i++)
    {
        if (ValuesToPrint[i]!=0x0)
        {
            delete ValuesToPrint[i];
            delete ValuesToSweep[i];
            ValuesToPrint [i] = 0x00;
            ValuesToSweep [i] = 0x00;
        }
    }
    ui->lineEdit->clear();
    alarmsCount = 0;
}

QString MainWindow::getNameOfDivision (int id)
{
    QSqlQuery query;
    bool ok;
    ok = query.exec("SELECT \"DIVISIONS\".\"NAME\""
                    "FROM public.\"DIVISIONS\""
                    "WHERE \"DIVISIONS\".\"DIVISION\" = " + QString::number(id) + ";");

    if(!ok)
    {
        QMessageBox::warning(this,QString::fromLocal8Bit("Ошибка запроса к базе данных"),query.lastError().text());
        log.addEvent(query.lastError().text());
    }
    if (query.size()==0) return QString::fromLocal8Bit("Нет подразделения");

    while (query.next())
    {
        return query.value(0).toString();
    }
    return QString("none");

}

QStringList  *MainWindow::getPointsToSweep(int id)
{
    QStringList *strList;

    QSqlQuery query;
    bool ok;
    ok = query.exec("SELECT"
                    "\"eBoxes\".human_names,"
                    "\"eBoxes\".\"NAME\","
                    "\"eBoxes\".\"DIVISION\""
                    "FROM public.\"eBoxes\""
                    "WHERE \"eBoxes\".\"DIVISION\" = " + QString::number(id) + ";");

    if(!ok)
    {
        QMessageBox::warning(this,QString::fromLocal8Bit("Ошибка запроса к базе данных"),query.lastError().text());
        log.addEvent(query.lastError().text());
    }

    strList = new QStringList();

    while (query.next())
    {
        strList->append(QString::number(query.value(2).toInt()).rightJustified(2,QChar::fromAscii('0'))+
                        QString::number(query.value(1).toInt()).rightJustified(4,QChar::fromAscii('0'))+":"+ query.value(0).toString());
    }
    return strList;

}

void MainWindow::removeFromSweep(QString d, int id)
{
    QStringList *a;
    a = ValuesToSweep[id];
    for (int i = 0; i < a->length(); i++)
    {
        if (a->at(i).contains(d))
            a->removeAt(i);
    }
}

QString MainWindow::reportSweepsHTML(int id)
{
    QString accumulator;
    if (ValuesToSweep[id]->length() == 0 ) return QString::fromLocal8Bit("<h2>Все точки пройдены!<h2>");
    else accumulator.append(QString::fromLocal8Bit("<h2>Не пройдено ")
                            + QString::number(ValuesToSweep[id]->length())
                            + QString::fromLocal8Bit(" точек!</h2> <table border=\"0\" cellspacing=\"0\" cellpadding=\"5\">"));

    for (int i = 0; i < ValuesToSweep[id]->length(); i++)
    {
        accumulator.append("<tr><td>");
        accumulator.append(ValuesToSweep[id]->at(i));
        accumulator.append("</td></tr>");
    }
    accumulator.append("</table>");

    return accumulator;
}

QString MainWindow::prepareDocumentForSign()
{
    QString accumulator;
    for (int i=0; i<100;i++)
    {

        if (ValuesToPrint[i]!=0x0)
        {
            accumulator.append(*ValuesToPrint[i]+"\r\n ---------------- \r\n");
        }
    } return accumulator;
}

QString MainWindow::signDocument(QString textToSign)
{
    //QFileDialog dialog;
    QString fileName;

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly,true);
    if(!dialog.exec())
    {
        return "no sign";
    }

    fileName = dialog.selectedFiles().at(0);

    QFile signFile;
    QTextStream *signFileStream;

    signFile.setFileName(fileName + "/signText.txt");
    signFile.remove();

    if (!signFile.open(QIODevice::ReadWrite | QIODevice::Text))
        return "no sign";

    signFileStream = new QTextStream(&signFile);
    signFileStream->setCodec("Windows-1251");

    *signFileStream << textToSign;

    delete signFileStream;

    signFile.close();

    QDir::setCurrent(fileName);

    QFile sigFile("sig");
    sigFile.remove();

    QProcess process;
    process.execute(QString("cmd.exe"),QStringList()<<"/C"<<fileName + "/sign.bat");
    process.waitForFinished();


    if (!sigFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return "no sign";
    QByteArray line;
    if (!sigFile.atEnd()) {
        line = sigFile.readLine();
    }
    return QString(line.toBase64());
}
QString MainWindow::XORkey(QString data)
{
    QString outString;
    for (int i = 0; i < data.length(); i++)
    {
        outString.append(data.at(i).unicode()^xor_key);
    }

    return outString;
}
