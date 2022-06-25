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
    ui->normalBtn->setIcon(QIcon(":/11"));
    ui->normalBtn->setIconSize(QSize(30, 30));
    connect(ui->normalBtn, &QPushButton::clicked, this, [=]()
    {
         qDebug() << "我是一个普通按钮, 是一个屌丝...";
    });


    // 有checked属性的按钮
    ui->checkedBtn->setCheckable(true);
    connect(ui->checkedBtn, &QPushButton::toggled, this, [=](bool bl)
    {
         qDebug() << "我是一个checked按钮, 当前状态为:" << bl;
    });

    // 关联菜单
    ui->menuBtn->setText("你喜欢哪种美女?");
    QMenu* menu = new QMenu;
    QAction* act = menu->addAction("可爱的");
    QAction* act1 =menu->addAction("粘人的");
    QAction* act2 =menu->addAction("胸大的");
    QAction* act3 =menu->addAction("屁股翘的");
    ui->menuBtn->setMenu(menu);
    connect(act, &QAction::triggered, this, [=]{
        qDebug() << "我是一个可爱的女人, 今晚约吗?";
    });
    connect(act1, &QAction::triggered, this, [=]{
        qDebug() << "我是一个粘人的女人, 今晚约吗?";
    });
    connect(act2, &QAction::triggered, this, [=]{
        qDebug() << "我是一个胸大的女人, 今晚约吗?";
    });
    connect(act3, &QAction::triggered, this, [=]{
        qDebug() << "我是一个屁股翘的女人, 今晚约吗?";
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

