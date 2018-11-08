#ifndef CHARTPLUGIN_H
#define CHARTPLUGIN_H

#include <QGenericPlugin>
#include"BasePluginInterface.h"
#include"ChartWidget.h"
QT_BEGIN_NAMESPACE
#define ChartPluginInterface_iid "seer-robotics.com.Roboshop.ChartPluginInterface/1.1.0"
Q_DECLARE_INTERFACE(BasePluginInterface, ChartPluginInterface_iid)
QT_END_NAMESPACE
class ChartPlugin : public BasePluginInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID ChartPluginInterface_iid FILE "ChartPlugin.json")
#endif // QT_VERSION >= 0x050000

public:
    ChartPlugin();
    bool getqVariantToInt(const QString &sKey, int iType, QVariant &var);
    void init(const QString &pluginName);
};

#endif // CHARTPLUGIN_H
