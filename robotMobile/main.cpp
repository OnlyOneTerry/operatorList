#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>

#include "ListModel.h"
#include "LoadGraphicsViewWid.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    ListModel * mylistModel = new ListModel;
    mylistModel->append("AMB150","192.168.1.114","qrc:/RobotContent.qml");
    // 向 QML 域注册 ObjectModel 类
    qmlRegisterType<ListModel, 1>("ListModel", 1, 0,"ListModel");
    qmlRegisterType<LoadGraphicsViewWid>("LoadGraphicsViewWid", 1, 0, "LoadGraphicsViewWid");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    engine.rootContext()->setContextProperty("mylistModel", mylistModel);
    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
