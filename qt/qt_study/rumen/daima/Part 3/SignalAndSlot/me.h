#ifndef ME_H
#define ME_H

#include <QObject>

class Me : public QObject
{
    Q_OBJECT
public:
    explicit Me(QObject *parent = nullptr);

    // 槽函数

public slots:
    // 槽函数
    void eat();
    void eat(QString msg);

};

#endif // ME_H
