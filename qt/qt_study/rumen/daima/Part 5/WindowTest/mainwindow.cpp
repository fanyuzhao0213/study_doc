#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "mydialog.h"
#include <QColorDialog>
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QPainter>
#include <QProgressDialog>
#include <QTimer>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置窗口的最大尺寸
    //setMaximumSize(600, 600);
    // 设置窗口的最小尺寸
    //setMinimumSize(300, 300);
    // 设置窗口的固定尺寸
    //setFixedSize(500, 500);
    // 设置窗口标题
    setWindowTitle("hello, Qt");
    // 给显示的窗口设置图标
    setWindowIcon(QIcon(":/reddog"));

    connect(this, &MainWindow::windowTitleChanged, this, [=](const QString &title)
    {
        qDebug() << "新的标题: " << title;
    });

    connect(this, &MainWindow::windowIconChanged, this, [=](const QIcon &icon)
    {
        qDebug() << "当前窗口的图标被修改了...";
    });

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &MainWindow::customContextMenuRequested, this, [=](const QPoint &pos)
    {
        QMenu menu;
        menu.addAction("西红柿");
        menu.addAction("黄瓜");
        menu.addAction("茄子");
        menu.exec(QCursor::pos());
    });

    connect(ui->save_action, &QAction::triggered, this, [=]()
    {
        QMessageBox::information(this, "clicked", "你不要调戏我...");
    });

    // 给工具栏添加按钮和单行输入框
    ui->toolBar->addWidget(new QPushButton("搜索"));
    QLineEdit* edit = new QLineEdit;
    edit->setMaximumWidth(200);
    edit->setFixedWidth(100);
    ui->toolBar->addWidget(edit);

    // 添加第二个工具栏
    QToolBar* toolbar = new QToolBar("toolbar");
    this->addToolBar(Qt::LeftToolBarArea, toolbar);

    // 状态栏添加子控件
    // 按钮
    ui->statusBar->showMessage("我是状态栏....", 3000);
    QPushButton* button = new QPushButton("按钮");
    ui->statusBar->addWidget(button);
    // 标签
    QLabel* label = new QLabel("hello,world");
    ui->statusBar->addWidget(label);

    QTimer::singleShot(5000, this, [=]()
    {
        button->show();
        label->show();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_moveBtn_clicked()
{
    QRect rect = this->frameGeometry();
    move(rect.topLeft() + QPoint(10, 20));
}

void MainWindow::on_positionBtn_clicked()
{
    QRect rect = this->frameGeometry();
    qDebug() << "左上角: " << rect.topLeft()
             << "右上角: " << rect.topRight()
             << "左下角: " << rect.bottomLeft()
             << "右下角: " << rect.bottomRight()
             << "宽度: " << rect.width()
             << "高度: " << rect.height();
}

void MainWindow::on_geometryBtn_clicked()
{
    int x = 100 + rand() % 500;
    int y = 100 + rand() % 500;
    int width = this->width() + 10;
    int height = this->height() + 10;
    setGeometry(x, y, width, height);
}

void MainWindow::on_modifyBtn_clicked()
{
    setWindowTitle("你好, 世界");
    setWindowIcon(QIcon("e:\\temp\\mushroom_life.png"));
}

void MainWindow::on_modalDlg_clicked()
{
    MyDialog dlg;
    connect(&dlg, &MyDialog::finished, this, [=](int res)
    {
        qDebug() << "result: " << res;
    });
    connect(&dlg, &MyDialog::accepted, this, [=]()
    {
        qDebug() << "accepted 信号被发射了...";
    });
    connect(&dlg, &MyDialog::rejected, this, [=]()
    {
        qDebug() << "rejected 信号被发射了...";
    });

    int ret = dlg.exec();
    if(ret == QDialog::Accepted)
    {
        qDebug() << "accept button clicked...";
    }
    else if(ret == QDialog::Rejected)
    {
        qDebug() << "reject button clicked...";
    }
    else
    {
        qDebug() << "done button clicked...";
    }
}

void MainWindow::on_msgbox_clicked()
{
    QMessageBox::about(this, "about",  "这是一个简单的消息提示框!!!");
    QMessageBox::critical(this, "critical", "这是一个错误对话框-critical...");
    int ret = QMessageBox::question(this, "question", "你要保存修改的文件内容吗???",
                                    QMessageBox::Save|QMessageBox::Cancel, QMessageBox::Cancel);
    if(ret == QMessageBox::Save)
    {
        QMessageBox::information(this, "information", "恭喜你保存成功了, o(*￣︶￣*)o!!!");
    }
    else if(ret == QMessageBox::Cancel)
    {
        QMessageBox::warning(this, "warning", "你放弃了保存, ┭┮﹏┭┮ !!!");
    }
}

#define SAVEFILE
void MainWindow::on_filedlg_clicked()
{
#ifdef OPENDIR
    QString dirName = QFileDialog::getExistingDirectory(this, "打开目录", "e:\\temp");
    QMessageBox::information(this, "打开目录", "您选择的目录是: " + dirName);
#endif

#ifdef OPENFILE
    QString arg("Text files (*.txt)");
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("Open File"), "e:\\temp",
                tr("Images (*.png *.jpg);;Text files (*.txt)"), &arg);
    QMessageBox::information(this, "打开文件", "您选择的文件是: " + fileName);
#endif

#ifdef OPENFILES
    QStringList fileNames = QFileDialog::getOpenFileNames(
                this, tr("Open File"), "e:\\temp",
                tr("Images (*.png *.jpg);;Text files (*.txt)"));
    QString names;
    for(int i=0; i<fileNames.size(); ++i)
    {
        names += fileNames.at(i) + " ";
    }
    QMessageBox::information(this, "打开文件(s)", "您选择的文件是: " + names);
#endif

#ifdef SAVEFILE
    QString fileName = QFileDialog::getSaveFileName(this, "保存文件", "e:\\temp");
    QMessageBox::information(this, "保存文件", "您指定的保存数据的文件是: " + fileName);
#endif
}



void MainWindow::on_fontdlg_clicked()
{
#if 0
    bool ok;
    QFont ft = QFontDialog::getFont(
                &ok, QFont("微软雅黑", 12, QFont::Bold), this, "选择字体");
    qDebug() << "ok value is: " << ok;
#else
    QFont ft = QFontDialog::getFont(NULL);
#endif
    ui->fontlabel->setFont(ft);
}

void MainWindow::on_colordlg_clicked()
{
    QColor color = QColorDialog::getColor();
    QBrush brush(color);
    QRect rect(0, 0, ui->color->width(), ui->color->height());
    QPixmap pix(rect.width(), rect.height());
    QPainter p(&pix);
    p.fillRect(rect, brush);
    ui->color->setPixmap(pix);
    QString text = QString("red: %1, green: %2, blue: %3, 透明度: %4")
            .arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    ui->colorlabel->setText(text);
}

#define MULTITEXT
void MainWindow::on_inputdlg_clicked()
{
#ifdef INT
    int ret = QInputDialog::getInt(this, "年龄", "您的当前年龄: ", 10, 1, 100, 2);
    QMessageBox::information(this, "年龄", "您的当前年龄: " + QString::number(ret));
#endif

#ifdef DOUBLE
    double ret = QInputDialog::getDouble(this, "工资", "您的工资: ", 2000, 1000, 6000, 2);
    QMessageBox::information(this, "工资", "您的当前工资: " + QString::number(ret));
#endif

#ifdef ITEM
    QStringList items;
    items << "苹果" << "橙子" << "橘子" << "葡萄" << "香蕉" << "哈密瓜";
    QString item = QInputDialog::getItem(this, "请选择你喜欢的水果", "你最喜欢的水果:", items, 1, false);
    QMessageBox::information(this, "水果", "您最喜欢的水果是: " + item);
#endif

#ifdef TEXT
    QString text = QInputDialog::getText(this, "密码", "请输入新的密码", QLineEdit::Password, "helloworld");
    QMessageBox::information(this, "密码", "您设置的密码是: " + text);
#endif

#ifdef MULTITEXT
    QString info = QInputDialog::getMultiLineText(this, "表白", "您最想对漂亮小姐姐说什么呢?", "呦吼吼...");
    QMessageBox::information(this, "知心姐姐", "您最想对小姐姐说: " + info);
#endif
}

void MainWindow::on_progressdlg_clicked()
{
    // 1. 创建进度条对话框窗口对象
    QProgressDialog *progress = new QProgressDialog(
                "正在拷贝数据...", "取消拷贝", 0, 100, this);
    // 2. 初始化并显示进度条窗口
    progress->setWindowTitle("请稍后");
    progress->setWindowModality(Qt::WindowModal);
    progress->show();

    // 3. 更新进度条
    static int value = 0;
    QTimer *timer = new QTimer;
    connect(timer, &QTimer::timeout, this, [=]()
    {
         progress->setValue(value);
         value++;
         // 当value > 最大值的时候
         if(value > progress->maximum())
         {
             timer->stop();
             value = 0;
             delete progress;
             delete timer;
         }
    });

    connect(progress, &QProgressDialog::canceled, this, [=]()
    {
        timer->stop();
        value = 0;
        delete progress;
        delete timer;
    });

    timer->start(50);
}
