#ifndef RoModelPlugin_H
#define RoModelPlugin_H

#include "BasePluginInterface.h"
#include "SCRobotFileModelWidget.h"


QT_BEGIN_NAMESPACE
#define RoModelPluginInterface_iid "seer-robotics.com.Roboshop.RoModelPluginInterface/1.1.0"
Q_DECLARE_INTERFACE(BasePluginInterface, RoModelPluginInterface_iid)
QT_END_NAMESPACE

class RoModelPlugin : public BasePluginInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID RoModelPluginInterface_iid FILE "RoModelPlugin.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(BasePluginInterface)
public:
    RoModelPlugin();
    bool getqVariantToInt(const QString &sKey, int iType, QVariant &var);

    void init(const QString &pluginName);
public slots:

private:

};

#endif // RoModelPlugin_H
