

NAME = RoboImg2map
TARGET = $${NAME}
DEFINES += RoImportMapPlugin_LIBRARY

#--------------------------------------------
QT  += xml

#--------------------------------------------

include(../HomePlugin.pri)
include(../../../Libs/CLibs/RoCore/RoCore.pri)
include(../../../Libs/CLibs/BaseUi/BaseUi.pri)
include(../../../Libs/ThirdLibs/Protobuf/Protobuf.pri)
include(../../../Libs/ThirdLibs/Opencv/Opencv.pri)
include(../../../Libs/ThirdLibs/Protobuf/Protobuf_Messages.pri)
include(ImportMapPlugin.pri)
############################
CONFIG(debug, debug|release){
    message($${NAME} " Debug")
    LIBS += -lRoCored \
                   -lRoBaseUid
}else{
    message($${NAME} " Release")
    LIBS += -lRoCore \
                   -lRoBaseUi
}
############################
#headers
HEADERS += \
    ImportImageWidget.h \
    GraphicsView/EditorPublicClass.h

FORMS += \
    ImportImageWidget.ui

SOURCES += \
    GraphicsView/items/BaseItem.cpp \
    GraphicsView/items/EditorArrowItem.cpp \
    GraphicsView/items/EditorBlurItem.cpp \
    GraphicsView/items/EditorCropItem.cpp \
    GraphicsView/items/EditorCurveItem.cpp \
    GraphicsView/items/EditorEllipseItem.cpp \
    GraphicsView/items/EditorEncircleItem.cpp \
    GraphicsView/items/EditorHighlighterItem.cpp \
    GraphicsView/items/EditorImageItem.cpp \
    GraphicsView/items/EditorLineItem.cpp \
    GraphicsView/items/EditorMagnifierItem.cpp \
    GraphicsView/items/EditorPolygonItem.cpp \
    GraphicsView/items/EditorPolylineItem.cpp \
    GraphicsView/items/EditorRectangleItem.cpp \
    GraphicsView/items/EditorTagItem.cpp \
    GraphicsView/items/EditorTextItem.cpp \
    GraphicsView/items/EditorUndoCommand.cpp \
    GraphicsView/items/HandgripItem.cpp \
    GraphicsView/items/SimpleTextItem.cpp \
    GraphicsView/items/TagManager.cpp \
    GraphicsView/EditorScene.cpp \
    GraphicsView/EditorView.cpp \
    GraphicsView/widgets/WidgetBase.cpp \
    GraphicsView/widgets/WidgetBrush.cpp \
    GraphicsView/widgets/WidgetColor.cpp \
    GraphicsView/widgets/WidgetDashStyle.cpp \
    GraphicsView/widgets/WidgetFillStyle.cpp \
    GraphicsView/widgets/WidgetLineWidth.cpp \
    GraphicsView/GroupLayer.cpp \
    GraphicsView/ItemRegistering.cpp \
    GraphicsView/LogHandler.cpp \
    GraphicsView/io/FileParser.cpp \
    GraphicsView/io/IOHelper.cpp \
    GraphicsView/ChipItem.cpp \
    ImportImageThread.cpp \
    ZoomIn.cpp \
    ImportMapPlugin.cpp \
    ImportImageWidget.cpp \
    GraphicsView/EditorPublicClass.cpp

HEADERS += \
    GraphicsView/items/BaseItem.h \
    GraphicsView/items/EditorArrowItem.h \
    GraphicsView/items/EditorBlurItem.h \
    GraphicsView/items/EditorCropItem.h \
    GraphicsView/items/EditorCurveItem.h \
    GraphicsView/items/EditorEllipseItem.h \
    GraphicsView/items/EditorEncircleItem.h \
    GraphicsView/items/EditorHighlighterItem.h \
    GraphicsView/items/EditorImageItem.h \
    GraphicsView/items/EditorLineItem.h \
    GraphicsView/items/EditorMagnifierItem.h \
    GraphicsView/items/EditorPolygonItem.h \
    GraphicsView/items/EditorPolylineItem.h \
    GraphicsView/items/EditorRectangleItem.h \
    GraphicsView/items/EditorTagItem.h \
    GraphicsView/items/EditorTextItem.h \
    GraphicsView/items/EditorUndoCommand.h \
    GraphicsView/items/HandgripItem.h \
    GraphicsView/items/SimpleTextItem.h \
    GraphicsView/items/TagManager.h \
    GraphicsView/EditorScene.h \
    GraphicsView/EditorView.h \
    GraphicsView/widgets/WidgetBase.h \
    GraphicsView/widgets/WidgetBrush.h \
    GraphicsView/widgets/WidgetColor.h \
    GraphicsView/widgets/WidgetDashStyle.h \
    GraphicsView/widgets/WidgetFillStyle.h \
    GraphicsView/widgets/WidgetLineWidth.h \
    GraphicsView/GroupLayer.h \
    GraphicsView/ItemFactory.h \
    GraphicsView/ItemRegistering.h \
    GraphicsView/LogHandler.h \
    GraphicsView/io/FileParser.h \
    GraphicsView/io/IOHelper.h \
    GraphicsView/IMessageHandler.h \
    GraphicsView/ChipItem.h \
    ImportImageThread.h \
    ZoomIn.h \
    ImportMapPlugin.h

#win32: RC_FILE = ICO.rc


DISTFILES += \
    ImportMapPlugin.json
