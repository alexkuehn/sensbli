/********************************************************************************
** Form generated from reading ui file 'calibview.ui'
**
** Created: Thu 5. Nov 19:12:13 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CALIBVIEW_H
#define UI_CALIBVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_CalibViewClass
{
public:
    QLabel *satview;
    QLabel *label;

    void setupUi(QDialog *CalibViewClass)
    {
        if (CalibViewClass->objectName().isEmpty())
            CalibViewClass->setObjectName(QString::fromUtf8("CalibViewClass"));
        CalibViewClass->resize(659, 518);
        CalibViewClass->setSizeGripEnabled(false);
        satview = new QLabel(CalibViewClass);
        satview->setObjectName(QString::fromUtf8("satview"));
        satview->setGeometry(QRect(10, 30, 640, 480));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(satview->sizePolicy().hasHeightForWidth());
        satview->setSizePolicy(sizePolicy);
        satview->setFrameShape(QFrame::StyledPanel);
        satview->setPixmap(QPixmap(QString::fromUtf8(":/sensbli/icons/testpattern.svg")));
        satview->setScaledContents(false);
        label = new QLabel(CalibViewClass);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 161, 16));

        retranslateUi(CalibViewClass);

        QMetaObject::connectSlotsByName(CalibViewClass);
    } // setupUi

    void retranslateUi(QDialog *CalibViewClass)
    {
        CalibViewClass->setWindowTitle(QApplication::translate("CalibViewClass", "sensbli: Calibration Saturation View", 0, QApplication::UnicodeUTF8));
        satview->setText(QString());
        label->setText(QApplication::translate("CalibViewClass", "Saturation View", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CalibViewClass);
    } // retranslateUi

};

namespace Ui {
    class CalibViewClass: public Ui_CalibViewClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALIBVIEW_H
