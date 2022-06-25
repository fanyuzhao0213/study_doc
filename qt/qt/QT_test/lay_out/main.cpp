#include "mainwindow.h"
#include "login.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    login login;
    login.exec();

    MainWindow w;
    w.show();
    return a.exec();
}
