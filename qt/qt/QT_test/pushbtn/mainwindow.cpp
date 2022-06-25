#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 普通按钮, 没有checked属性
    ui->normalBtn->setText("我是个哈哈");       //重命名
    ui->normalBtn->setIcon(QIcon(":/11"));     //设置图标
    ui->normalBtn->setIconSize(QSize(30, 30));  //设置图标大小
    connect(ui->normalBtn,&QPushButton::clicked,this,[=]()
    {
       qDebug() << "我是一个普通按钮, 是一个屌丝...";
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

