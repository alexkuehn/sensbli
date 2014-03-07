/********************************************************************************
** Form generated from reading ui file 'setup.ui'
**
** Created: Thu 5. Nov 19:12:13 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SETUP_H
#define UI_SETUP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_setupClass
{
public:
    QTabWidget *setupTabs;
    QWidget *tab;
    QGroupBox *groupBox_2;
    QLabel *label_3;
    QSpinBox *camindex;
    QCheckBox *dbgenable;
    QWidget *tab_2;
    QGroupBox *groupBox;
    QLabel *label;
    QLineEdit *serveraddress;
    QLabel *label_2;
    QSpinBox *serverport;
    QPushButton *bnApply;
    QPushButton *bnCancel;

    void setupUi(QDialog *setupClass)
    {
        if (setupClass->objectName().isEmpty())
            setupClass->setObjectName(QString::fromUtf8("setupClass"));
        setupClass->setWindowModality(Qt::NonModal);
        setupClass->resize(482, 254);
        setupClass->setModal(true);
        setupTabs = new QTabWidget(setupClass);
        setupTabs->setObjectName(QString::fromUtf8("setupTabs"));
        setupTabs->setGeometry(QRect(10, 20, 461, 191));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        groupBox_2 = new QGroupBox(tab);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 10, 421, 81));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 30, 51, 16));
        camindex = new QSpinBox(groupBox_2);
        camindex->setObjectName(QString::fromUtf8("camindex"));
        camindex->setGeometry(QRect(60, 30, 71, 22));
        camindex->setMinimum(-1);
        dbgenable = new QCheckBox(tab);
        dbgenable->setObjectName(QString::fromUtf8("dbgenable"));
        dbgenable->setGeometry(QRect(20, 110, 141, 19));
        setupTabs->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        groupBox = new QGroupBox(tab_2);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 431, 131));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 30, 61, 16));
        serveraddress = new QLineEdit(groupBox);
        serveraddress->setObjectName(QString::fromUtf8("serveraddress"));
        serveraddress->setGeometry(QRect(90, 30, 171, 20));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 70, 61, 16));
        serverport = new QSpinBox(groupBox);
        serverport->setObjectName(QString::fromUtf8("serverport"));
        serverport->setGeometry(QRect(90, 70, 61, 22));
        serverport->setMaximum(65535);
        setupTabs->addTab(tab_2, QString());
        bnApply = new QPushButton(setupClass);
        bnApply->setObjectName(QString::fromUtf8("bnApply"));
        bnApply->setGeometry(QRect(310, 220, 75, 24));
        bnCancel = new QPushButton(setupClass);
        bnCancel->setObjectName(QString::fromUtf8("bnCancel"));
        bnCancel->setGeometry(QRect(400, 220, 75, 24));

        retranslateUi(setupClass);

        setupTabs->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(setupClass);
    } // setupUi

    void retranslateUi(QDialog *setupClass)
    {
        setupClass->setWindowTitle(QApplication::translate("setupClass", "sensbli Setup", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("setupClass", "Camera", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("setupClass", "Index:", 0, QApplication::UnicodeUTF8));
        dbgenable->setText(QApplication::translate("setupClass", "Enable Debug", 0, QApplication::UnicodeUTF8));
        setupTabs->setTabText(setupTabs->indexOf(tab), QApplication::translate("setupClass", "Input", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("setupClass", "Message Server", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("setupClass", "Address:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("setupClass", "Port:", 0, QApplication::UnicodeUTF8));
        setupTabs->setTabText(setupTabs->indexOf(tab_2), QApplication::translate("setupClass", "Output", 0, QApplication::UnicodeUTF8));
        bnApply->setText(QApplication::translate("setupClass", "Apply", 0, QApplication::UnicodeUTF8));
        bnCancel->setText(QApplication::translate("setupClass", "Cancel", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(setupClass);
    } // retranslateUi

};

namespace Ui {
    class setupClass: public Ui_setupClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUP_H
