#-------------------------------------------------
#
# Project created by QtCreator 2013-05-04T15:18:59
#
#-------------------------------------------------

QT       += core gui sql network

TARGET = odgInsert
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    fileentry.cpp \
    sndrepr.cpp \
    logger.cpp \
    getpointslistfromdb.cpp \
    outputtemplate.cpp \
    odginsertview.cpp

HEADERS  += mainwindow.h \
    fileentry.h \
    sndrepr.h \
    logger.h \
    getpointslistfromdb.h \
    outputtemplate.h \
    odginsertview.h

FORMS    += mainwindow.ui \
    sndrepr.ui \
    odginsertview.ui


