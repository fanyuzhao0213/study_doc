#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "testwidget.h"
#include "testdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 一般在qt的构造函数中进行初始化操作(窗口, 数据, ...)
    // 显示当前窗口的时候, 显示另外一个窗口 TestWidget
#if 1
    // 创建窗口对象, 没有给w对象指定父对象, 这个窗口是一个独立窗口
    // 要显示这个独立窗口必须要进行show()操作
    TestWidget* w = new TestWidget;
    // 显示当前窗口
    w->show();
#else
    // 创建窗口对象, 没有给w对象指定父对象
    // explicit TestWidget(QWidget *parent = nullptr);
    // 如果创建一个窗口对象的时候给其指定了父对象, 这个窗口就不是一个独立窗口
    // 这样的话当前父窗口显示的时候, 子窗口就一并被显示出来了
    // 这时候子窗口是没有边框的
    TestWidget* w = new TestWidget(this);
#endif

#if 1
    // 创建对话框窗口
    TestDialog* dlg = new TestDialog(this);
    // 非模态
    dlg->move(100, 100);
    dlg->show();
#else
    // 创建对话框窗口
    TestDialog* dlg = new TestDialog(this);
    // 模态, exec()
    // 阻塞程序的执行
    dlg->exec();
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

