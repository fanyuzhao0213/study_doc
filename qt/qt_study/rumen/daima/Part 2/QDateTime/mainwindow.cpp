#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDate>
#include <QDebug>
#include <QTime>
#include <QElapsedTimer>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 获取当前的日期
    QDate d = QDate::currentDate();
    // 第一种方式:
    qDebug() << "year: " << d.year() << ", moth: " << d.month() << ", day: " << d.day();

    // 第二种方式 - 2000-01-01
    QString str = d.toString("yyyy-MM-dd");
    qDebug() << "date str: " << str;

    // 获取当前时间
    QTime curtime = QTime::currentTime();
    // 方式1
    qDebug() << "hour: " << curtime.hour() << ", minute: " << curtime.minute()
             << ", second: " << curtime.second() << ", millisecond: " << curtime.msec();
    // 方式2
    QString strtm = curtime.toString("hh:mm:ss.zzz");
    qDebug() << "格式化的日期: " << strtm;

#if 0
    // 统计函数运行的时间
    QTime tt;
    tt.start();
    randNumbers(100);
    int ms = tt.elapsed();
    qDebug() << "函数执行所用的时间长度为: " << ms << "毫秒";
#else
    // 统计函数运行的时间
    QElapsedTimer tt;
    tt.start();
    randNumbers(100);
    int ms = tt.elapsed();
    qDebug() << "函数执行所用的时间长度为: " << ms << "毫秒";
#endif

    // 获取当前的日期和时间
    QDateTime dt = QDateTime::currentDateTime();
    // 格式化 2020/01/10 12:12:12 上午/下午
    //       yyyy/MM/dd hh:mm:ss ap
    QString strdt = dt.toString("yyyy/MM/dd HH:mm:ss ap");
    qDebug() << "当前的日期和时间: " << strdt;

    // 先取出日期
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

void MainWindow::randNumbers(int count)
{
    srand(time(NULL));
    for(int i=0; i<count; ++i)
    {
        int num = rand() % 10000;
        qDebug() << num;
    }
}

