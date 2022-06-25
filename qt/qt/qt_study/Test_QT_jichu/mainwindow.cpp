#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDate>
#include <QDebug>
#include <QTime>
#include <QDateTime>

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

    //整形数
    int value = dataPlus(10,20).toInt();
    //字符串
    QString str1 = dataPlus("nihao","shijie").toString();
    qDebug() << "int value:" << value;
    qDebug() << "string value:" << str1;

    // 创建Person对象
    Person p;
    p.id = 250;
    p.name = "张三疯";
#if 1
    QVariant v;
    v.setValue(p);
#else
    QVariant v = QVariant::fromValue(p);
#endif

    // 取出v对象中 的数据
    if(v.canConvert<Person>())
    {
        Person tmp = v.value<Person>();
        qDebug() << "id: " << tmp.id << ", name: " << tmp.name;
    }

    // 获取当前的日期和时间
    QDateTime dt = QDateTime::currentDateTime();
    // 格式化 2020/01/10 12:12:12 上午/下午
    //       yyyy/MM/dd hh:mm:ss ap
    QString strdt = dt.toString("yyyy/MM/dd HH:mm:ss ap");
    qDebug() << "当前的日期和时间: " << strdt;

    // 先取出日期
    QDate d = QDate::currentDate();
    d = dt.date();
    qDebug() << "year: " << d.year() << ", moth: " << d.month() << ", day: " << d.day();

    // 在取出时间
    QTime t = dt.time();
    qDebug() << "hour: " << t.hour() << ", minute: " << t.minute()
             << ", second: " << t.second() << ", millisecond: " << t.msec();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QVariant MainWindow::dataPlus(QVariant a, QVariant b)
{
    QVariant ret;
    //判断当前参数类型
    if(a.type() == QVariant::Int && b.type() == QVariant::Int)
    {
       ret = QVariant(a.toInt() + b.toInt());
    }
    else if(a.type() == QVariant::String && b.type() == QVariant::String)
    {
        ret.setValue(a.toString() + b.toString());
    }
    return  ret;
}

