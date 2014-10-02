#-------------------------------------------------
#
# Project created by QtCreator 2014-03-21T12:45:23
#
#-------------------------------------------------

QT       += core gui xml sql network

TARGET = xml_work_test
TEMPLATE = app


SOURCES += main.cpp\
        testwindow.cpp \
    odgxmlparser.cpp \
    odgdivision.cpp \
    odgsweeppoint.cpp \
    odgconfdialog.cpp \
    odgaddpartition.cpp \
    odgaddcategory.cpp \
    ../broadcastSender/odgbroadcastasker.cpp \
    odgorptgw.cpp \
    selectitems.cpp \
    odgselectreport.cpp \
    odgdataaddmaster.cpp \
    phonefileitem.cpp

HEADERS  += testwindow.h \
    odgxmlparser.h \
    odgdivision.h \
    odgsweeppoint.h \
    odgconfdialog.h \
    odgaddpartition.h \
    odgaddcategory.h \
    ../broadcastSender/odgbroadcastasker.h \
    odgorptgw.h \
    selectitems.h \
    odgselectreport.h \
    odgdataaddmaster.h \
    phonefileitem.h


FORMS    += testwindow.ui \
    odgconfdialog.ui \
    odgaddpartition.ui \
    odgaddcategory.ui \
    selectitems.ui \
    odgselectreport.ui \
    odgdataaddmaster.ui
