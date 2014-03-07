/********************************************************************************
** Form generated from reading ui file 'sensbli.ui'
**
** Created: Thu 5. Nov 19:12:13 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SENSBLI_H
#define UI_SENSBLI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_sensbliClass
{
public:
    QLabel *label;
    QPushButton *bnQuit;
    QPushButton *bnCalib;
    QPushButton *bnRecord;
    QPushButton *bnSetup;
    QLabel *liveview;
    QLabel *label_2;
    QLabel *statusinfo;

    void setupUi(QWidget *sensbliClass)
    {
        if (sensbliClass->objectName().isEmpty())
            sensbliClass->setObjectName(QString::fromUtf8("sensbliClass"));
        sensbliClass->setWindowModality(Qt::NonModal);
        sensbliClass->resize(817, 599);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(sensbliClass->sizePolicy().hasHeightForWidth());
        sensbliClass->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/sensbli/icons/cartoon_eye.svg"), QSize(), QIcon::Normal, QIcon::Off);
        sensbliClass->setWindowIcon(icon);
        label = new QLabel(sensbliClass);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 30, 81, 16));
        bnQuit = new QPushButton(sensbliClass);
        bnQuit->setObjectName(QString::fromUtf8("bnQuit"));
        bnQuit->setGeometry(QRect(700, 550, 101, 24));
        bnCalib = new QPushButton(sensbliClass);
        bnCalib->setObjectName(QString::fromUtf8("bnCalib"));
        bnCalib->setGeometry(QRect(700, 60, 101, 24));
        bnRecord = new QPushButton(sensbliClass);
        bnRecord->setObjectName(QString::fromUtf8("bnRecord"));
        bnRecord->setGeometry(QRect(700, 100, 101, 24));
        bnSetup = new QPushButton(sensbliClass);
        bnSetup->setObjectName(QString::fromUtf8("bnSetup"));
        bnSetup->setGeometry(QRect(700, 150, 101, 24));
        liveview = new QLabel(sensbliClass);
        liveview->setObjectName(QString::fromUtf8("liveview"));
        liveview->setGeometry(QRect(40, 50, 640, 480));
        sizePolicy.setHeightForWidth(liveview->sizePolicy().hasHeightForWidth());
        liveview->setSizePolicy(sizePolicy);
        liveview->setFrameShape(QFrame::StyledPanel);
        liveview->setPixmap(QPixmap(QString::fromUtf8(":/sensbli/icons/testpattern.svg")));
        liveview->setScaledContents(false);
        label_2 = new QLabel(sensbliClass);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(40, 550, 51, 16));
        statusinfo = new QLabel(sensbliClass);
        statusinfo->setObjectName(QString::fromUtf8("statusinfo"));
        statusinfo->setGeometry(QRect(100, 550, 121, 16));

        retranslateUi(sensbliClass);

        QMetaObject::connectSlotsByName(sensbliClass);
    } // setupUi

    void retranslateUi(QWidget *sensbliClass)
    {
        sensbliClass->setWindowTitle(QApplication::translate("sensbliClass", "sensbli", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("sensbliClass", "LiveView", 0, QApplication::UnicodeUTF8));
        bnQuit->setText(QApplication::translate("sensbliClass", "Quit", 0, QApplication::UnicodeUTF8));
        bnCalib->setText(QApplication::translate("sensbliClass", "Calibrate", 0, QApplication::UnicodeUTF8));
        bnRecord->setText(QApplication::translate("sensbliClass", "Record", 0, QApplication::UnicodeUTF8));
        bnSetup->setText(QApplication::translate("sensbliClass", "Setup", 0, QApplication::UnicodeUTF8));
        liveview->setText(QString());
        label_2->setText(QApplication::translate("sensbliClass", "Status:", 0, QApplication::UnicodeUTF8));
        statusinfo->setText(QApplication::translate("sensbliClass", "Waiting", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(sensbliClass);
    } // retranslateUi

};

namespace Ui {
    class sensbliClass: public Ui_sensbliClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SENSBLI_H
