#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QVBoxLayout* vlayout = new QVBoxLayout;

    for(int i=0; i<11; ++i)
    {
        QLabel* pic = new QLabel;
        QString name = QString(":/images/%1.png").arg(i+1);
        pic->setPixmap(QPixmap(name));
        pic->setAlignment(Qt::AlignHCenter);
        vlayout->addWidget(pic);
    }

    QWidget* wg = new QWidget;
    wg->setLayout(vlayout);
    ui->scrollArea->setWidget(wg);
}

MainWindow::~MainWindow()
{
    delete ui;
}

