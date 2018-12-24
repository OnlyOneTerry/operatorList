#include "EditorPublicClass.h"
#include <QSettings>
#include <QMutex>

EditorPublicClass * EditorPublicClass::pEditorPublicClass = Q_NULLPTR;

EditorPublicClass::EditorPublicClass(QObject *parent) : QObject(parent)
{

}
EditorPublicClass* EditorPublicClass::init()
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    if(!pEditorPublicClass){
        pEditorPublicClass = new EditorPublicClass;
    }
    return pEditorPublicClass;
}

