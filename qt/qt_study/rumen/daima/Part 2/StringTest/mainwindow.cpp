#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // (唐僧)有(3)个徒弟, 分别是(孙悟空), (猪八戒), (沙僧)
    QString str = QString("(%1)有(%2)个徒弟, 分别是(%3), (%4), (%5)")
            .arg("唐僧").arg(3).arg("孙悟空").arg("猪八戒").arg("沙僧");
    qDebug() << "字符串的值: " << str;
    // 计算对象中的字符串长度
    // 长度是字符个数, 一个汉字算一个字符
    QString tmp = "我很帅呦, 吼吼吼...";
    // 长度是字符串占用的字节数, 一个汉字3个字节
    QByteArray tmp1 = "我很帅呦, 吼吼吼...";
    qDebug() << "QString 字符个数: " << tmp.length();
    qDebug() << "QByteArray 字节的长度: " << tmp1.length();
}

MainWindow::~MainWindow()
{
    delete ui;
}

