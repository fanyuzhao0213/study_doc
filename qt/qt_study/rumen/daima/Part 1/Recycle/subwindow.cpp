#include "subwindow.h"
#include "ui_subwindow.h"
#include <QDebug>

SubWindow::SubWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SubWindow)
{
    ui->setupUi(this);
}

SubWindow::~SubWindow()
{
    qDebug() << "我是SubWindow, 我被析构了...";
    delete ui;
}
