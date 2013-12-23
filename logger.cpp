#include "logger.h"

logger::logger()
{

    logFile.setFileName("errors.log");
         if (!logFile.open(QIODevice::Append | QIODevice::Text))
             return;
         if (logFile.size() > 500000)
         {
             logFile.close();
             if (!logFile.open(QIODevice::ReadWrite | QIODevice::Text))
                 return;
             QFile tempFile("errors.old");
             if (!tempFile.open(QIODevice::ReadWrite | QIODevice::Text))
                 return;
             tempFile.write(logFile.readAll());
             tempFile.close();
             logFile.close();
             if(!QFile::remove("errors.log"))
                 return;
             if (!logFile.open(QIODevice::Append | QIODevice::Text))
                 return;
         }
    txtStream = new QTextStream(&logFile);
    txtStream->setCodec("UTF-8");
    *txtStream<<QDateTime::currentDateTimeUtc().toString()<<QString::fromLocal8Bit("  Новый сеанс ")<<"\r\n";
}

logger::~logger()
{
    delete txtStream;
}

void logger::addEvent(QString event)
{
    *txtStream<<QDateTime::currentDateTimeUtc().toString()<<":"<<event<<"\r\n";
}
