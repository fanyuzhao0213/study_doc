#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentWidget(ui->page_2);

    connect(ui->win1, &QPushButton::clicked, this, [=]()
    {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(ui->win2, &QPushButton::clicked, this, [=]()
    {
        ui->stackedWidget->setCurrentWidget(ui->page_2);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

