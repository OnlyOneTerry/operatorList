#include "WriteThread.h"
#include "PublicThreadClass.h"
#include <QDebug>
#include <QVariantMap>

WriteThread::WriteThread(QObject *parent) : BaseThread(parent)
{

}
WriteThread::~WriteThread()
{
    this->stopThread();
}
void WriteThread::run()
{
    while(!isNeedStop()){
        auto map =  PublicThreadClass::instance()->getListFirst();
        if(map.isNull()){ //没有数据就休息一会
            QThread::msleep(1000);
        }else{
            SCDebug<<"WriteThread:"<<QThread::currentThreadId()<<"map:"<<map.toMap().value("index").toInt();
             PublicThreadClass::instance()->removeListLast();
        }
    }
}
