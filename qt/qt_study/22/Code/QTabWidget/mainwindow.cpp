#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, [=](int index)
    {
        // 保存信息
        QWidget* wg = ui->tabWidget->widget(index);
        QString title = ui->tabWidget->tabText(index);
        m_widgets.enqueue(wg);
        m_names.enqueue(title);
        // 移除tab页
        ui->tabWidget->removeTab(index);
        ui->addBtn->setEnabled(true);
    });

    connect(ui->tabWidget, &QTabWidget::tabBarClicked, this, [=](int index)
    {
        qDebug() << "我被点击了一下, 我的标题是: " << ui->tabWidget->tabText(index);
    });

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, [=](int index)
    {
        qDebug() << "当前显示的tab页, 我的标题是: " << ui->tabWidget->tabText(index);
    });

    connect(ui->addBtn, &QPushButton::clicked, this, [=]()
    {
        // 将被删除的标签页添加到窗口中
        // 1. 知道窗口对象, 窗口的标题
        // 2. 知道添加函数
        ui->tabWidget->addTab(m_widgets.dequeue(), m_names.dequeue());
        if(m_widgets.empty())
        {
            ui->addBtn->setDisabled(true);
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

