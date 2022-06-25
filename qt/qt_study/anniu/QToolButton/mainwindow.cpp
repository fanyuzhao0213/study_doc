#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 普通按钮, 没有checked属性
    ui->normalBtn->setText("我是个屌丝");
    ui->normalBtn->setIconSize(QSize(50, 50));
    ui->normalBtn->setIcon(QIcon(":/mario.png"));
    connect(ui->normalBtn, &QToolButton::clicked, this, [=]()
    {
         qDebug() << "我是一个普通按钮, 是一个屌丝...";
    });
    ui->normalBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    QAction* actBtn = new QAction(QIcon(":/mushroom_life.png"), "奥利给");
    ui->actionBtn->setDefaultAction(actBtn);
    connect(ui->actionBtn, &QToolButton::triggered, this, [=](QAction* act)
    {
        act->setText("我是修改之后的马里奥...");
        act->setIcon(QIcon(":/mario.png"));
    });
    ui->actionBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    // 显示一个箭头作为QToolButton的图标。默认情况下，这个属性被设置为Qt::NoArrow。

    ui->arrowBtn->setArrowType(Qt::UpArrow);
    ui->arrowBtn->setText("向上");
    ui->arrowBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);


    // 有checked属性的按钮
    ui->checkedBtn->setCheckable(true);
    connect(ui->checkedBtn, &QToolButton::toggled, this, [=](bool bl)
    {
         qDebug() << "我是一个checked按钮, 当前状态为:" << bl;
    });

    // 关联菜单
    ui->menuBtn->setText("你喜欢哪种美女?");
    QMenu* menu = new QMenu;
    QAction* act = menu->addAction("可爱的");
    menu->addAction("粘人的");
    menu->addAction("胸大的");
    menu->addAction("屁股翘的");
    ui->menuBtn->setMenu(menu);
    connect(act, &QAction::triggered, this, [=]{
        qDebug() << "我是一个可爱的女人, 今晚约吗?";
    });

    // 设置弹出菜单的弹出方式
    ui->popmenu->setMenu(menu);
    /*
    弹出菜单的弹出模式是一个枚举类型: QToolButton::ToolButtonPopupMode, 取值如下
        - QToolButton::DelayedPopup: 延时弹出, 按压工具按钮一段时间后才能弹出, 比如:浏览器的返回按钮
                                     长按按钮菜单弹出, 但是按钮的 clicked 信号不会被发射
        - QToolButton::MenuButtonPopup: 在这种模式下，工具按钮会显示一个特殊的箭头，表示有菜单。
                                        当按下按钮的箭头部分时，将显示菜单。按下按钮部分发射 clicked 信号
        - QToolButton::InstantPopup: 当按下工具按钮时，菜单立即显示出来。
                                     在这种模式下，按钮本身的动作不会被触发(不会发射clicked信号
    */
    ui->popmenu->setPopupMode(QToolButton::MenuButtonPopup);
    connect(ui->popmenu, &QToolButton::clicked, this, [=]()
    {
        qDebug() << "我是popMenu按钮, 好痒呀...";
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

