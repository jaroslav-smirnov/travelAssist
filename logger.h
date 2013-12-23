#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDateTime>

class logger
{
public:
    logger();
    ~logger();
        void addEvent(QString event);
private:
    QFile logFile;
    QTextStream *txtStream;

};

#endif // LOGGER_H

