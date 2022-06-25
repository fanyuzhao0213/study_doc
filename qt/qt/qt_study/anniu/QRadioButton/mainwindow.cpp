#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->radioButton_6->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_redio_996_clicked()
{
    qDebug() << "996";
}

void MainWindow::on_radio_nosalary_clicked()
{
    qDebug() << "没有加班费";
}

void MainWindow::on_radio_nogirl_clicked()
{
    qDebug() << "公司没有妹子...";
}

void MainWindow::on_radio_notbeautiful_clicked(bool checked)
{
    qDebug() << "前台小姐姐不好看!!!";
}
