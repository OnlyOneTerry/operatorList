#include "RoModelPlugin.h"

RoModelPlugin::RoModelPlugin()
{

}
void RoModelPlugin::init(const QString& pluginName)
{
    BasePluginInterface::init(pluginName);
    if(PublicClass::init()->_fullVersion)
        this->addAction(tr("Model"));
}
//----------------------GET-----------------------
bool RoModelPlugin::getqVariantToInt(const QString&sKey,int iType,QVariant&var)
{
    switch (iType) {
    case E_BPT_GetWidget:
    {
        SCRobotFileModelWidget * w = Q_NULLPTR;
        if(!WidgetMap.value(sKey)){
            w  = new SCRobotFileModelWidget();
            connect(w,SIGNAL(sigClosedWidget()),this,SLOT(slotClosedWidget()));
            QAction *ac = getActionMapKey(sKey);
            w->setProperty(M_RoPluginName,ac->data().toMap().value(M_RoPluginName));
            w->setProperty(M_WidgetKey,ac->data().toMap().value(M_WidgetKey));
            w->setWindowTitle(ac->text());
            WidgetMap[sKey] = w;
        }
        var.setValue(w);
    }
        break;
    default:
        break;
    }
    return true;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2("RoModelPlugin", RoModelPlugin)
#endif // QT_VERSION < 0x050000
