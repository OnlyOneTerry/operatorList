#include "ChartPlugin.h"


ChartPlugin::ChartPlugin() :
    BasePluginInterface()
{
}

bool ChartPlugin::getqVariantToInt(const QString &sKey, int iType, QVariant &var)
{
    switch (iType) {
    case E_BPT_GetWidget:
    {
        if(!WidgetMap.value(sKey)){
            ChartWidget *w = new ChartWidget;
            connect(w,SIGNAL(sigClosedWidget()),this,SLOT(slotClosedWidget()));
            QAction *ac = getActionMapKey(sKey);
            w->setProperty(M_RoPluginName,ac->data().toMap().value(M_RoPluginName));
            w->setProperty(M_WidgetKey,ac->data().toMap().value(M_WidgetKey));
            w->setWindowTitle(ac->text());
            WidgetMap[sKey] = w;
        }
        var.setValue(WidgetMap.value(sKey));
    }
        break;
    case E_BPT_ShowWidget:
    {
        getqVariantToInt(sKey,E_BPT_GetWidget,var);
        dynamic_cast<CustomChildBaseDialog * >(WidgetMap[sKey])->show();
    }
        break;
    default:
        break;
    }
    return true;
}

void ChartPlugin::init(const QString &pluginName)
{
    BasePluginInterface::init(pluginName);
     this->addAction(tr("Chart"));
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(ChartPlugin, ChartPlugin)
#endif // QT_VERSION < 0x050000
