/********************************************************************************
** Form generated from reading UI file 'testwindow.ui'
**
** Created: Thu 31. Jul 15:13:03 2014
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TESTWINDOW_H
#define UI_TESTWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_testWindow
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton_getData;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *testWindow)
    {
        if (testWindow->objectName().isEmpty())
            testWindow->setObjectName(QString::fromUtf8("testWindow"));
        testWindow->resize(400, 300);
        centralWidget = new QWidget(testWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButton_getData = new QPushButton(centralWidget);
        pushButton_getData->setObjectName(QString::fromUtf8("pushButton_getData"));
        pushButton_getData->setGeometry(QRect(80, 80, 75, 23));
        testWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(testWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 21));
        testWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(testWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        testWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(testWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        testWindow->setStatusBar(statusBar);

        retranslateUi(testWindow);

        QMetaObject::connectSlotsByName(testWindow);
    } // setupUi

    void retranslateUi(QMainWindow *testWindow)
    {
        testWindow->setWindowTitle(QApplication::translate("testWindow", "testWindow", 0, QApplication::UnicodeUTF8));
        pushButton_getData->setText(QApplication::translate("testWindow", "PushButton", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class testWindow: public Ui_testWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TESTWINDOW_H
