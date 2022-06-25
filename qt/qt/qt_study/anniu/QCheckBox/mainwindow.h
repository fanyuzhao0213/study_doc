#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_redio_996_stateChanged(int arg1);

    void on_radio_nosalary_stateChanged(int arg1);

    void on_radio_nogirl_stateChanged(int arg1);

    void on_radio_notbeautiful_stateChanged(int arg1);

    void statusChanged(int state);

private:
    Ui::MainWindow *ui;
    int m_number = 0;
};
#endif // MAINWINDOW_H
