#ifndef ImportMapPlugin_H
#define ImportMapPlugin_H

#include "BasePluginInterface.h"
#include "ImportImageWidget.h"


QT_BEGIN_NAMESPACE
#define ImportMapPluginInterface_iid "seer-robotics.com.Roboshop.ImportMapPluginInterface/1.1.0"
Q_DECLARE_INTERFACE(BasePluginInterface, ImportMapPluginInterface_iid)
QT_END_NAMESPACE

class ImportMapPlugin : public BasePluginInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID ImportMapPluginInterface_iid FILE "ImportMapPlugin.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(BasePluginInterface)
public:
    ImportMapPlugin();
    bool getqVariantToInt(const QString &sKey, int iType, QVariant &var);

    void init(const QString &pluginName);
public slots:

private:

};

#endif // ImportMapPlugin_H
