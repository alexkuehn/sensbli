/********************************************************************************
** Form generated from reading ui file 'recording.ui'
**
** Created: Thu 5. Nov 19:12:12 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_RECORDING_H
#define UI_RECORDING_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_RecordingClass
{
public:
    QLabel *label;
    QLabel *filename;
    QPushButton *bnStopRec;

    void setupUi(QDialog *RecordingClass)
    {
        if (RecordingClass->objectName().isEmpty())
            RecordingClass->setObjectName(QString::fromUtf8("RecordingClass"));
        RecordingClass->resize(327, 102);
        RecordingClass->setModal(true);
        label = new QLabel(RecordingClass);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 46, 14));
        filename = new QLabel(RecordingClass);
        filename->setObjectName(QString::fromUtf8("filename"));
        filename->setGeometry(QRect(10, 40, 301, 16));
        bnStopRec = new QPushButton(RecordingClass);
        bnStopRec->setObjectName(QString::fromUtf8("bnStopRec"));
        bnStopRec->setGeometry(QRect(60, 70, 191, 24));

        retranslateUi(RecordingClass);

        QMetaObject::connectSlotsByName(RecordingClass);
    } // setupUi

    void retranslateUi(QDialog *RecordingClass)
    {
        RecordingClass->setWindowTitle(QApplication::translate("RecordingClass", "Recording", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("RecordingClass", "File:", 0, QApplication::UnicodeUTF8));
        filename->setText(QApplication::translate("RecordingClass", "none", 0, QApplication::UnicodeUTF8));
        bnStopRec->setText(QApplication::translate("RecordingClass", "Stop Recording", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(RecordingClass);
    } // retranslateUi

};

namespace Ui {
    class RecordingClass: public Ui_RecordingClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECORDING_H
