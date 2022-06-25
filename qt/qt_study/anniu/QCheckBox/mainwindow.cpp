#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置根节点的三态属性
    ui->wives->setTristate(true);

    connect(ui->wives, &QCheckBox::clicked, this, [=](bool bl)
    {
        if(bl)
        {
            ui->jianning->setChecked(true);
            ui->fangyi->setChecked(true);
            ui->longer->setChecked(true);
            ui->zengrou->setChecked(true);
            ui->mujianping->setChecked(true);
            ui->shuanger->setChecked(true);
            ui->ake->setChecked(true);
        }
        else
        {
            ui->jianning->setChecked(false);
            ui->fangyi->setChecked(false);
            ui->longer->setChecked(false);
            ui->zengrou->setChecked(false);
            ui->mujianping->setChecked(false);
            ui->shuanger->setChecked(false);
            ui->ake->setChecked(false);
        }
    });

    connect(ui->jianning, &QCheckBox::stateChanged, this, &MainWindow::statusChanged);
    connect(ui->fangyi, &QCheckBox::stateChanged, this, &MainWindow::statusChanged);
    connect(ui->longer, &QCheckBox::stateChanged, this, &MainWindow::statusChanged);
    connect(ui->zengrou, &QCheckBox::stateChanged, this, &MainWindow::statusChanged);
    connect(ui->mujianping, &QCheckBox::stateChanged, this, &MainWindow::statusChanged);
    connect(ui->shuanger, &QCheckBox::stateChanged, this, &MainWindow::statusChanged);
    connect(ui->ake, &QCheckBox::stateChanged, this, &MainWindow::statusChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_redio_996_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
    {
        qDebug() << "不喜欢公司的996!!!";
    }
    else
    {
        qDebug() << "996忍一忍还是可以接收的...";
    }
}

void MainWindow::on_radio_nosalary_stateChanged(int arg1)
{

}

void MainWindow::on_radio_nogirl_stateChanged(int arg1)
{

}

void MainWindow::on_radio_notbeautiful_stateChanged(int arg1)
{

}

void MainWindow::statusChanged(int state)
{
    if(state == Qt::Checked)
    {
        m_number ++;
    }
    else
    {
        m_number --;
    }

    // 判断根节点是否需要做状态的更新
    if(m_number == 7)
    {
        ui->wives->setCheckState(Qt::Checked);
    }
    else if(m_number == 0)
    {
        ui->wives->setCheckState(Qt::Unchecked);
    }
    else
    {
        ui->wives->setCheckState(Qt::PartiallyChecked);
    }
}
