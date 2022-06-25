#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_me = new Me;
    m_girl = new GirlFriend;
//    void (GirlFriend::*girl1)() = &GirlFriend::hungry;
    void (GirlFriend::*girl2)(QString) = &GirlFriend::hungry;
    void (Me::*mypoint)(QString) = &Me::eat;
    connect(m_girl, girl2, m_me, mypoint);
    connect(m_girl, girl2, this, &MainWindow::eatSlot);

    // 使用Qt4的方式进行连接
    connect(m_girl, SIGNAL(hungry()), m_me, SLOT(eat()));
    connect(m_girl, SIGNAL(hungry(QString)), m_me, SLOT(eat(QString)));

    // 信号连接信号
    // connect(ui->hungry, &QPushButton::clicked, m_girl, girl1);
    // 信号槽连接
    //connect(ui->hungry, &QPushButton::clicked, this, &MainWindow::hungrySlot);
    connect(ui->hungry, &QPushButton::clicked, this, [=](){
        // 发射自定义信号
        m_girl->hungry();
        m_girl->hungry("意大利面");
    });

    connect(ui->closeBtn, &QPushButton::clicked, this, &MainWindow::close);


    // 匿名函数的定义, 代码执行这个匿名函数是不会被调用的
    [](){
        qDebug() << "hello, 我是一个lambda表达式...";
    };

    // 调用匿名函数
    int a=100, b=200, c=300;
    [&](){
        qDebug() << "hello, 我是一个lambda表达式...";
        qDebug() << "使用引用的方式传递数据: ";
        qDebug() << "a+1:" << a++ << ", b+c= " << b+c;
    }();

    // 值拷贝的方式使用外部数据
    [=](int m, int n)mutable{
        qDebug() << "hello, 我是一个lambda表达式...";
        qDebug() << "使用拷贝的方式传递数据: ";
        // 拷贝的外部数据在函数体内部是只读的
        qDebug() << "a+1:" << a++ << ", b+c= " << b+c;
        qDebug() << "m+1: " << ++m << ", n: " << n;
    }(1, 2);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::hungrySlot()
{
    // 发射自定义信号
    m_girl->hungry();
    m_girl->hungry("意大利面");
}

void MainWindow::eatSlot()
{
    qDebug() << "我带你去吃海鲜...";
}


