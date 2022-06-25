#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ui->toolBox->setCurrentIndex(1);
    ui->toolBox->setCurrentWidget(ui->subwidget2);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_toolBox_currentChanged(int index)
{
    qDebug() << ui->toolBox->itemText(index);
}
