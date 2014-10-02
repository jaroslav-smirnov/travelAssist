#include "odgorptgw.h"

OdgORPTGw::OdgORPTGw(QObject *parent) :
    QObject(parent)
{
}

int OdgORPTGw::startRPTRender(QString databaseURL, QString username, QString passwd, QString loadfromdb, QString params){
    QString batLine = "orpt\\RPTrender.exe -databaseURL=%1 -username=\"%2\" -passwd=\"%3\""
            " -pdf -outpdf=\"%7\""
            " -loadfromdb=\"%4\" %6";

    QString filepath;
    QFile file("temp.pdf");
    filepath="temp.pdf";
    if (file.exists()){

        if (!file.remove()){
            filepath = "temp"+QString::number(qrand());
        }
    }
    QString line = batLine.arg(databaseURL).arg(username).arg(passwd).arg(loadfromdb).arg(params).arg(filepath);
    QProcess process;
    QProcess pdfProc;
         process.startDetached(QString(line));
         qDebug()<<line;
         QFile outputFile(filepath);
         int loopwait = 0;
         while (!outputFile.exists()){
             Sleep(500);
             loopwait ++;
             qDebug()<<loopwait;
             if (loopwait > 10)
                 break;
         }
         if (outputFile.exists()){

             QFile RNPdf("runpdf.bat");
             if (RNPdf.exists())
             {
                 RNPdf.remove();
             }

             if (!RNPdf.open(QIODevice::WriteOnly | QIODevice::Text))
                              return -1;

                          QTextStream out(&RNPdf);

                          out << QString("START \"\" \"%1\"").arg(QFileInfo(outputFile).absoluteFilePath());
                          //out.flush();
                          RNPdf.close();
                          pdfProc.startDetached("runpdf.bat");


         }
         else
         {
             return -1;
         }

         return 0;

}
