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
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *open_action;
    QAction *save_action;
    QWidget *centralwidget;
    QPushButton *moveBtn;
    QPushButton *positionBtn;
    QPushButton *geometryBtn;
    QPushButton *modifyBtn;
    QPushButton *modalDlg;
    QPushButton *msgbox;
    QPushButton *filedlg;
    QGroupBox *groupBox;
    QPushButton *fontdlg;
    QLabel *fontlabel;
    QGroupBox *groupBox_2;
    QPushButton *colordlg;
    QLabel *colorlabel;
    QLabel *color;
    QPushButton *inputdlg;
    QPushButton *progressdlg;
    QMenuBar *menubar;
    QMenu *menu;
    QToolBar *toolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget_2;
    QWidget *dockWidgetContents_2;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 646);
        open_action = new QAction(MainWindow);
        open_action->setObjectName(QString::fromUtf8("open_action"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/img/mario.png"), QSize(), QIcon::Normal, QIcon::Off);
        open_action->setIcon(icon);
        save_action = new QAction(MainWindow);
        save_action->setObjectName(QString::fromUtf8("save_action"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/reddog"), QSize(), QIcon::Normal, QIcon::Off);
        save_action->setIcon(icon1);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        moveBtn = new QPushButton(centralwidget);
        moveBtn->setObjectName(QString::fromUtf8("moveBtn"));
        moveBtn->setGeometry(QRect(50, 40, 93, 28));
        positionBtn = new QPushButton(centralwidget);
        positionBtn->setObjectName(QString::fromUtf8("positionBtn"));
        positionBtn->setGeometry(QRect(50, 90, 111, 28));
        geometryBtn = new QPushButton(centralwidget);
        geometryBtn->setObjectName(QString::fromUtf8("geometryBtn"));
        geometryBtn->setGeometry(QRect(30, 140, 231, 28));
        modifyBtn = new QPushButton(centralwidget);
        modifyBtn->setObjectName(QString::fromUtf8("modifyBtn"));
        modifyBtn->setGeometry(QRect(50, 180, 131, 28));
        modalDlg = new QPushButton(centralwidget);
        modalDlg->setObjectName(QString::fromUtf8("modalDlg"));
        modalDlg->setGeometry(QRect(60, 240, 121, 28));
        msgbox = new QPushButton(centralwidget);
        msgbox->setObjectName(QString::fromUtf8("msgbox"));
        msgbox->setGeometry(QRect(60, 280, 101, 28));
        filedlg = new QPushButton(centralwidget);
        filedlg->setObjectName(QString::fromUtf8("filedlg"));
        filedlg->setGeometry(QRect(60, 320, 111, 28));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(20, 370, 271, 161));
        fontdlg = new QPushButton(groupBox);
        fontdlg->setObjectName(QString::fromUtf8("fontdlg"));
        fontdlg->setGeometry(QRect(80, 30, 101, 28));
        fontlabel = new QLabel(groupBox);
        fontlabel->setObjectName(QString::fromUtf8("fontlabel"));
        fontlabel->setGeometry(QRect(10, 70, 251, 81));
        fontlabel->setFrameShape(QFrame::Panel);
        fontlabel->setAlignment(Qt::AlignCenter);
        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(340, 20, 431, 151));
        colordlg = new QPushButton(groupBox_2);
        colordlg->setObjectName(QString::fromUtf8("colordlg"));
        colordlg->setGeometry(QRect(30, 40, 121, 28));
        colorlabel = new QLabel(groupBox_2);
        colorlabel->setObjectName(QString::fromUtf8("colorlabel"));
        colorlabel->setGeometry(QRect(20, 90, 401, 41));
        colorlabel->setFrameShape(QFrame::Box);
        colorlabel->setAlignment(Qt::AlignCenter);
        colorlabel->setWordWrap(true);
        color = new QLabel(groupBox_2);
        color->setObjectName(QString::fromUtf8("color"));
        color->setGeometry(QRect(270, 30, 111, 41));
        color->setFrameShape(QFrame::Box);
        inputdlg = new QPushButton(centralwidget);
        inputdlg->setObjectName(QString::fromUtf8("inputdlg"));
        inputdlg->setGeometry(QRect(360, 200, 131, 28));
        progressdlg = new QPushButton(centralwidget);
        progressdlg->setObjectName(QString::fromUtf8("progressdlg"));
        progressdlg->setGeometry(QRect(360, 260, 141, 28));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 26));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        MainWindow->setMenuBar(menubar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setIconSize(QSize(48, 48));
        toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        dockWidget_2 = new QDockWidget(MainWindow);
        dockWidget_2->setObjectName(QString::fromUtf8("dockWidget_2"));
        dockWidget_2->setFloating(false);
        dockWidget_2->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        pushButton = new QPushButton(dockWidgetContents_2);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(10, 20, 93, 28));
        pushButton_2 = new QPushButton(dockWidgetContents_2);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(0, 80, 93, 28));
        pushButton_3 = new QPushButton(dockWidgetContents_2);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(0, 130, 93, 28));
        dockWidget_2->setWidget(dockWidgetContents_2);
        MainWindow->addDockWidget(Qt::LeftDockWidgetArea, dockWidget_2);

        menubar->addAction(menu->menuAction());
        menu->addAction(open_action);
        menu->addSeparator();
        menu->addAction(save_action);
        toolBar->addAction(save_action);
        toolBar->addAction(open_action);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        open_action->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200", nullptr));
#if QT_CONFIG(tooltip)
        open_action->setToolTip(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\346\226\207\344\273\266", nullptr));
#endif // QT_CONFIG(tooltip)
        save_action->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230", nullptr));
        moveBtn->setText(QCoreApplication::translate("MainWindow", "\347\247\273\345\212\250\347\252\227\345\217\243", nullptr));
        positionBtn->setText(QCoreApplication::translate("MainWindow", "\350\216\267\345\217\226\344\275\215\347\275\256\344\277\241\346\201\257", nullptr));
        geometryBtn->setText(QCoreApplication::translate("MainWindow", "\344\277\256\346\224\271\347\252\227\345\217\243\347\232\204\344\275\215\347\275\256\345\222\214\345\260\272\345\257\270\344\277\241\346\201\257", nullptr));
        modifyBtn->setText(QCoreApplication::translate("MainWindow", "\344\277\256\346\224\271\346\240\207\351\242\230\345\222\214\345\233\276\346\240\207", nullptr));
        modalDlg->setText(QCoreApplication::translate("MainWindow", "\346\230\276\347\244\272\346\250\241\346\200\201\345\257\271\350\257\235\346\241\206", nullptr));
        msgbox->setText(QCoreApplication::translate("MainWindow", "QMessageBox", nullptr));
        filedlg->setText(QCoreApplication::translate("MainWindow", "QFileDialog", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\345\255\227\344\275\223\351\200\211\346\213\251", nullptr));
        fontdlg->setText(QCoreApplication::translate("MainWindow", "QFontDialog", nullptr));
        fontlabel->setText(QCoreApplication::translate("MainWindow", "\346\210\221\346\230\257\350\246\201\346\210\220\344\270\272\346\265\267\350\264\274\347\216\213\347\232\204\347\224\267\344\272\272!!!", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "\351\242\234\350\211\262\351\200\211\346\213\251", nullptr));
        colordlg->setText(QCoreApplication::translate("MainWindow", "QColorDialog", nullptr));
        colorlabel->setText(QString());
        color->setText(QString());
        inputdlg->setText(QCoreApplication::translate("MainWindow", "QInputDialog", nullptr));
        progressdlg->setText(QCoreApplication::translate("MainWindow", "QProgressDialog", nullptr));
        menu->setTitle(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
        dockWidget_2->setWindowTitle(QCoreApplication::translate("MainWindow", "\346\210\221\346\230\257\344\270\200\344\270\252\346\265\256\345\212\250\347\252\227\345\217\243", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
