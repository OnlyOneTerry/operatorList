#include "ReadThread.h"
#include "PublicThreadClass.h"

ReadThread::ReadThread(QObject *parent) : BaseThread(parent)
{

}
ReadThread::~ReadThread()
{
    this->stopThread();
}
void ReadThread::run()
{
    int index = 0;
    while(!isNeedStop()){
        QVariantMap map;
        map.insert("index",++index);
        PublicThreadClass::instance()->appendToList(map);
        SCDebug<<"ReadThread:"<<QThread::currentThreadId()<<index;
        QThread::msleep(1000);
    }
}
