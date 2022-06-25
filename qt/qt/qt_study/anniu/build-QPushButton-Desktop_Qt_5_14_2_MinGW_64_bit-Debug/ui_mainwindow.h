/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *normalBtn;
    QPushButton *checkedBtn;
    QPushButton *menuBtn;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        normalBtn = new QPushButton(centralwidget);
        normalBtn->setObjectName(QString::fromUtf8("normalBtn"));
        normalBtn->setGeometry(QRect(210, 110, 151, 41));
        checkedBtn = new QPushButton(centralwidget);
        checkedBtn->setObjectName(QString::fromUtf8("checkedBtn"));
        checkedBtn->setGeometry(QRect(200, 210, 161, 51));
        menuBtn = new QPushButton(centralwidget);
        menuBtn->setObjectName(QString::fromUtf8("menuBtn"));
        menuBtn->setGeometry(QRect(190, 320, 171, 61));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        normalBtn->setText(QCoreApplication::translate("MainWindow", "\346\231\256\351\200\232\346\214\211\351\222\256", nullptr));
        checkedBtn->setText(QCoreApplication::translate("MainWindow", "check\345\261\236\346\200\247\346\214\211\351\222\256", nullptr));
        menuBtn->setText(QCoreApplication::translate("MainWindow", "111", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
