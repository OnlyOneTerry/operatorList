INCLUDEPATH +=$$PWD \
                            $$PWD/SCBaseGraphicsItems \
                            $$PWD/SCGraphicsHelper


HEADERS += \
    $$PWD/SCBaseGraphicsScene.h \
    $$PWD/SCBaseGraphicsView.h \
    $$PWD/SCBaseGraphicsItems/BaseGraphicsItemAxis.h \
    $$PWD/SCBaseGraphicsItems/BaseGraphicsItem.h \
    $$PWD/SCGraphicsHelper/GraphicsHelper.h \
    $$PWD/SCBaseGraphicsItems/ItemDashedFrame.h
SOURCES += \
    $$PWD/SCBaseGraphicsScene.cpp \
    $$PWD/SCBaseGraphicsView.cpp \
    $$PWD/SCBaseGraphicsItems/BaseGraphicsItemAxis.cpp \
    $$PWD/SCBaseGraphicsItems/BaseGraphicsItem.cpp \
    $$PWD/SCGraphicsHelper/GraphicsHelper.cpp \
    $$PWD/SCBaseGraphicsItems/ItemDashedFrame.cpp

#win32: RC_FILE = ICO.rc


