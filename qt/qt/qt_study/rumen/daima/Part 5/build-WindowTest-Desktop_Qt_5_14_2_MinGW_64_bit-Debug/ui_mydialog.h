/********************************************************************************
** Form generated from reading UI file 'mydialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYDIALOG_H
#define UI_MYDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_MyDialog
{
public:
    QPushButton *acceptBtn;
    QPushButton *rejectBtn;
    QPushButton *donBtn;

    void setupUi(QDialog *MyDialog)
    {
        if (MyDialog->objectName().isEmpty())
            MyDialog->setObjectName(QString::fromUtf8("MyDialog"));
        MyDialog->resize(400, 300);
        acceptBtn = new QPushButton(MyDialog);
        acceptBtn->setObjectName(QString::fromUtf8("acceptBtn"));
        acceptBtn->setGeometry(QRect(100, 60, 93, 28));
        rejectBtn = new QPushButton(MyDialog);
        rejectBtn->setObjectName(QString::fromUtf8("rejectBtn"));
        rejectBtn->setGeometry(QRect(100, 130, 93, 28));
        donBtn = new QPushButton(MyDialog);
        donBtn->setObjectName(QString::fromUtf8("donBtn"));
        donBtn->setGeometry(QRect(100, 210, 93, 28));

        retranslateUi(MyDialog);

        QMetaObject::connectSlotsByName(MyDialog);
    } // setupUi

    void retranslateUi(QDialog *MyDialog)
    {
        MyDialog->setWindowTitle(QCoreApplication::translate("MyDialog", "Dialog", nullptr));
        acceptBtn->setText(QCoreApplication::translate("MyDialog", "Accept", nullptr));
        rejectBtn->setText(QCoreApplication::translate("MyDialog", "Reject", nullptr));
        donBtn->setText(QCoreApplication::translate("MyDialog", "Done", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MyDialog: public Ui_MyDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYDIALOG_H
