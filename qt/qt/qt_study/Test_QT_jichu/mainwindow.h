#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
struct Person
{
   int id;
   QString name;

};
Q_DECLARE_METATYPE(Person)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //两个变量之间加法运算，可能是整形，也可能是字符串
    QVariant dataPlus(QVariant a, QVariant b);

    // 生成指定个数的随机数
    void randNumbers(int count);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
