#include "PublicThreadClass.h"

PublicThreadClass::PublicThreadClass(QObject *parent) : QObject(parent)
{
}

PublicThreadClass::~PublicThreadClass()
{
}

PublicThreadClass* PublicThreadClass::createInstance()
{
    return new PublicThreadClass();
}

PublicThreadClass* PublicThreadClass::instance()
{
    return Singleton<PublicThreadClass>::instance(PublicThreadClass::createInstance);
}

/** 追加到QList
 * @brief PublicThreadClass::addOneToList
 * @param var
 */
void PublicThreadClass::appendToList(const QVariant & var)
{
    _mutex.lock();
    VariantList.append(var);
    _mutex.unlock();
}

/** 删除QList最后一行
 * @brief PublicThreadClass::removeListLast
 */
void PublicThreadClass::removeListLast()
{
    _mutex.lock();
    if(!VariantList.isEmpty())
        VariantList.removeLast();
    _mutex.unlock();
}


/** 获取QList 第0位
 * @brief PublicThreadClass::getListFirst
 * @return
 */
QVariant PublicThreadClass::getListFirst()
{
    QVariant var;
    _mutex.lock();
    if(VariantList.size())
        var = VariantList.first();
    _mutex.unlock();
    return var;
}

/** 删除所有
 * @brief PublicThreadClass::removeAll
 */
void PublicThreadClass::removeAll()
{
    _mutex.lock();
    while(!VariantList.isEmpty()){
        //        delete VariantList.takeFirst();
        VariantList.removeFirst();
    }
    _mutex.unlock();
}

