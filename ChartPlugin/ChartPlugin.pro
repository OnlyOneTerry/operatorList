#-------------------------------------------------
#
# Project created by QtCreator 2018-11-06T16:02:38
#
#-------------------------------------------------

NAME = RoChartPlugin
TARGET = $${NAME}

include(../HomePlugin.pri)
include(../../../Libs/CLibs/RoCore/RoCore.pri)
include(../../../Libs/CLibs/BaseUi/BaseUi.pri)

CONFIG(debug, debug|release){
    #Debug编译环境
    #指定 Debug 编译输出文件路径
    message($${NAME} " Debug")
    LIBS += -lRoCored -lRoBaseUid
}else{
    #Release编译环境
    #指定 Release 编译输出文件路径
    message($${NAME}"  Release")
    LIBS += -lRoCore -lRoBaseUi
}

SOURCES += \
        ChartPlugin.cpp \
    ChartWidget.cpp \
    QWChartView.cpp \
    ChartForm.cpp

HEADERS += \
        ChartPlugin.h \
    ChartWidget.h \
    QWChartView.h \
    ChartForm.h

FORMS += \
        ChartWidget.ui \
    ChartForm.ui

DISTFILES += ChartPlugin.json 
