#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "DataModel.h"
#include <QQmlContext>
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    DataModel model;
    model.append("orange item","orange");

    qmlRegisterType<DataModel>("DataModel",1,0,"DataModel");
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("$Model",&model);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
