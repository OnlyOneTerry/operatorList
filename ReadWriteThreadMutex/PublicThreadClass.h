#ifndef PublicThreadClass_H
#define PublicThreadClass_H

#include <QObject>
#include <QMutexLocker>
#include <QVariant>
#include "SingletonClassOne.h"
#include <QDebug>

//通用打印宏（可用于多线程中）//QDateTime::currentDateTime().toString("hh:mm:ss:zzz")
#define SCDebug  qDebug() <<"[Debug]["<<__FILE__<<"]$"<<0<<"$"<<"["<<__LINE__<<"]["<<__FUNCTION__ <<"]:"
#define SCInfo  qInfo() <<"[Info]["<<__FILE__<<"]$"<<0<<"$"<<"["<<__LINE__<<"]["<<__FUNCTION__<<"]:"
#define SCWarning  qWarning()<<"[Warning][" <<__FILE__<<"]$"<<0<<"$"<<"["<<__LINE__<<"]["<<__FUNCTION__<<"]:"
#define SCritical  qCritical() <<"[Error]["<<__FILE__<<"]$"<<0<<"$"<<"["<<__LINE__<<"]["<<__FUNCTION__<<"]:"

class PublicThreadClass : public QObject
{
    Q_OBJECT
public:
    //初始化
    static PublicThreadClass *instance();
    ~PublicThreadClass();
    //获取QList第0位
    QVariant getListFirst();
    //删除QList最后1位
    void removeListLast();
    //删除QList所有
    void removeAll();
    //追加到QList
    void appendToList(const QVariant &var);
signals:

public slots:

private:
    //私有构造
    explicit PublicThreadClass(QObject *parent = nullptr);
    static PublicThreadClass *createInstance();

    /*
     * QT说:
QList<T>, QLinkedList<T>, and QVector<T> provide similar functionality. Here's an overview:
For most purposes, QList is the right class to use. Its index-based API is more convenient than QLinkedList's iterator-based API, and it is usually faster than QVector because of the way it stores its items in memory. It also expands to less code in your executable.
If you need a real linked list, with guarantees of constant time insertions in the middle of the list and iterators to items rather than indexes, use QLinkedList.
If you want the items to occupy adjacent memory positions, use QVector.
*/
    //QList
    QList<QVariant> VariantList;
    QMutex _mutex;

};

#endif // PublicThreadClass_H
