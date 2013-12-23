/********************************************************************************
** Form generated from reading UI file 'infooutput.ui'
**
** Created: Fri 29. Nov 12:51:12 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFOOUTPUT_H
#define UI_INFOOUTPUT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtWebKit/QWebView>

QT_BEGIN_NAMESPACE

class Ui_InfoOutput
{
public:
    QHBoxLayout *horizontalLayout;
    QWebView *webView;

    void setupUi(QDialog *InfoOutput)
    {
        if (InfoOutput->objectName().isEmpty())
            InfoOutput->setObjectName(QString::fromUtf8("InfoOutput"));
        InfoOutput->resize(400, 300);
        horizontalLayout = new QHBoxLayout(InfoOutput);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        webView = new QWebView(InfoOutput);
        webView->setObjectName(QString::fromUtf8("webView"));
        webView->setUrl(QUrl("about:blank"));

        horizontalLayout->addWidget(webView);


        retranslateUi(InfoOutput);

        QMetaObject::connectSlotsByName(InfoOutput);
    } // setupUi

    void retranslateUi(QDialog *InfoOutput)
    {
        InfoOutput->setWindowTitle(QApplication::translate("InfoOutput", "Dialog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class InfoOutput: public Ui_InfoOutput {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFOOUTPUT_H
