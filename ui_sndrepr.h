/********************************************************************************
** Form generated from reading UI file 'sndrepr.ui'
**
** Created: Fri 29. Nov 12:51:12 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SNDREPR_H
#define UI_SNDREPR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_sndRepr
{
public:

    void setupUi(QDialog *sndRepr)
    {
        if (sndRepr->objectName().isEmpty())
            sndRepr->setObjectName(QString::fromUtf8("sndRepr"));
        sndRepr->resize(400, 300);

        retranslateUi(sndRepr);

        QMetaObject::connectSlotsByName(sndRepr);
    } // setupUi

    void retranslateUi(QDialog *sndRepr)
    {
        sndRepr->setWindowTitle(QApplication::translate("sndRepr", "Dialog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class sndRepr: public Ui_sndRepr {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SNDREPR_H
