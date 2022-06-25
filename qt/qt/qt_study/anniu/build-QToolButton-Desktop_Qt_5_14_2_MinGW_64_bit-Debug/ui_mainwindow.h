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
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QToolButton *normalBtn;
    QToolButton *checkedBtn;
    QToolButton *menuBtn;
    QToolButton *actionBtn;
    QToolButton *arrowBtn;
    QToolButton *popmenu;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        normalBtn = new QToolButton(centralwidget);
        normalBtn->setObjectName(QString::fromUtf8("normalBtn"));

        verticalLayout->addWidget(normalBtn);

        checkedBtn = new QToolButton(centralwidget);
        checkedBtn->setObjectName(QString::fromUtf8("checkedBtn"));

        verticalLayout->addWidget(checkedBtn);

        menuBtn = new QToolButton(centralwidget);
        menuBtn->setObjectName(QString::fromUtf8("menuBtn"));

        verticalLayout->addWidget(menuBtn);

        actionBtn = new QToolButton(centralwidget);
        actionBtn->setObjectName(QString::fromUtf8("actionBtn"));

        verticalLayout->addWidget(actionBtn);

        arrowBtn = new QToolButton(centralwidget);
        arrowBtn->setObjectName(QString::fromUtf8("arrowBtn"));

        verticalLayout->addWidget(arrowBtn);

        popmenu = new QToolButton(centralwidget);
        popmenu->setObjectName(QString::fromUtf8("popmenu"));

        verticalLayout->addWidget(popmenu);

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
        normalBtn->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        checkedBtn->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        menuBtn->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        actionBtn->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        arrowBtn->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        popmenu->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
