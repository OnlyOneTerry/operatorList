#include "widget.h"
#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("MusicPlayer"));
    QDir::setCurrent(a.applicationDirPath());
    Widget w;
    w.show();
    w.initShow();

    return a.exec();
}
