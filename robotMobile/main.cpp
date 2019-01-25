#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>
#include "ListModel.h"
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    ListModel * listModel = new ListModel;
    // 向 QML 域注册 ObjectModel 类
    qmlRegisterType<ListModel, 1>("Listmodel", 1, 0,"ListModel");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    engine.rootContext()->setContextProperty("customModel", listModel);
    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
