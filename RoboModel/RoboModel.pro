
NAME = RoModel
TARGET = $${NAME}
DEFINES += RoModel_LIBRARY

#--------------------------------------------
include(../TabPlugins.pri)
include(../../../Libs/CLibs/RoCore/RoCore.pri)
include(../../../Libs/CLibs/BaseUi/BaseUi.pri)
include(../../../Libs/CLibs/RoNetwork/RoNetwork.pri)
include(SCBaseGraphics/SCBaseGraphics.pri)
include(RoModelPlugin.pri)
############################

CONFIG(debug, debug|release){
    message($${NAME} " Debug")
    LIBS += -lRoCored -lRoBaseUid -lRoQJsond \
                 -lRoNetworkd
}else{
    message($${NAME} " Release")
    LIBS += -lRoCore -lRoBaseUi -lRoQJson \
                  -lRoNetwork
              
}


##########################################

DISTFILES += \
    RoModelPlugin.json

FORMS += \
    ModelGraphics/GuidenceWidgets/BasicContentSetting.ui \
    ModelGraphics/GuidenceWidgets/CrashDiContentSetting.ui \
    ModelGraphics/GuidenceWidgets/DIContentSetting.ui \
    ModelGraphics/GuidenceWidgets/DOContentSetting.ui \
    ModelGraphics/GuidenceWidgets/GuidBasicSetting.ui \
    ModelGraphics/GuidenceWidgets/GuidChassisShape.ui \
    ModelGraphics/GuidenceWidgets/GuidChassisTypeSelect.ui \
    ModelGraphics/GuidenceWidgets/GuidDISetting.ui \
    ModelGraphics/GuidenceWidgets/GuidDoSetting.ui \
    ModelGraphics/GuidenceWidgets/GuidFinishedWid.ui \
    ModelGraphics/GuidenceWidgets/GuidLaserSetting.ui \
    ModelGraphics/GuidenceWidgets/GuidMagneticSensorSetting.ui \
    ModelGraphics/GuidenceWidgets/GuidModelName.ui \
    ModelGraphics/GuidenceWidgets/GuidoptionalDevice.ui \
    ModelGraphics/GuidenceWidgets/GuidRFIDSetting.ui \
    ModelGraphics/GuidenceWidgets/GuidSimpleDevice.ui \
    ModelGraphics/GuidenceWidgets/GuidUltrasonicSetting.ui \
    ModelGraphics/GuidenceWidgets/LaserContentSetting.ui \
    ModelGraphics/GuidenceWidgets/MagSensorContentSetting.ui \
    ModelGraphics/GuidenceWidgets/NumSetting.ui \
    ModelGraphics/GuidenceWidgets/UltrasonicContentSetting.ui \
    ModelGraphics/SettingWidgets/WidgetCircleRobotModelSetting.ui \
    ModelGraphics/SettingWidgets/WidgetCrashDiPosSetting.ui \
    ModelGraphics/SettingWidgets/WidgetCrashDISetting.ui \
    ModelGraphics/SettingWidgets/WidgetDISetting.ui \
    ModelGraphics/SettingWidgets/WidgetDOSetting.ui \
    ModelGraphics/SettingWidgets/WidgetDOState.ui \
    ModelGraphics/SettingWidgets/WidgetGuidance.ui \
    ModelGraphics/SettingWidgets/WidgetLaserSetting.ui \
    ModelGraphics/SettingWidgets/WidgetMagneticSensorSetting.ui \
    ModelGraphics/SettingWidgets/WidgetModelSetting.ui \
    ModelGraphics/SettingWidgets/WidgetPlygonSetting.ui \
    ModelGraphics/SettingWidgets/WidgetPosSetting.ui \
    ModelGraphics/SettingWidgets/WidgetRectRobotModelSetting.ui \
    ModelGraphics/SettingWidgets/WidgetRFIDsetting.ui \
    ModelGraphics/SettingWidgets/WidgetUltrasonicSetting.ui \
    ModelGraphics/ModelBasicSettingWidget.ui \
    ModelGraphics/modelgraphicsview.ui \
    ModelGraphics/ModelPropertySettingWidget.ui \
    ModelHelper/PropertyMissPromtWidget.ui \
    DialogModelChose.ui \
    DialogSaveAsTemplate.ui \
    EditRobotModelWidget.ui \
    SCRobotFileModelWidget.ui \
    ModelGraphics/SettingWidgets/WidgetCameraSetting.ui \
    ModelGraphics/SettingWidgets/WidgetForkSetting.ui

HEADERS += \
    ModelGraphics/GuidenceWidgets/BasicContentSetting.h \
    ModelGraphics/GuidenceWidgets/CrashDiContentSetting.h \
    ModelGraphics/GuidenceWidgets/DIContentSetting.h \
    ModelGraphics/GuidenceWidgets/DOContentSetting.h \
    ModelGraphics/GuidenceWidgets/GuidBaseSetting.h \
    ModelGraphics/GuidenceWidgets/GuidBasicSetting.h \
    ModelGraphics/GuidenceWidgets/GuidChassisShape.h \
    ModelGraphics/GuidenceWidgets/GuidChassisTypeSelect.h \
    ModelGraphics/GuidenceWidgets/GuidDISetting.h \
    ModelGraphics/GuidenceWidgets/GuidDoSetting.h \
    ModelGraphics/GuidenceWidgets/GuidFinishedWid.h \
    ModelGraphics/GuidenceWidgets/GuidLaserSetting.h \
    ModelGraphics/GuidenceWidgets/GuidMagneticSensorSetting.h \
    ModelGraphics/GuidenceWidgets/GuidModelName.h \
    ModelGraphics/GuidenceWidgets/GuidoptionalDevice.h \
    ModelGraphics/GuidenceWidgets/GuidRFIDSetting.h \
    ModelGraphics/GuidenceWidgets/GuidSimpleDevice.h \
    ModelGraphics/GuidenceWidgets/GuidUltrasonicSetting.h \
    ModelGraphics/GuidenceWidgets/LaserContentSetting.h \
    ModelGraphics/GuidenceWidgets/MagSensorContentSetting.h \
    ModelGraphics/GuidenceWidgets/NumSetting.h \
    ModelGraphics/GuidenceWidgets/UltrasonicContentSetting.h \
    ModelGraphics/ModelGraphicsItems/ItemAngelCtroller.h \
    ModelGraphics/ModelGraphicsItems/ItemAxis.h \
    ModelGraphics/ModelGraphicsItems/ItemBase.h \
    ModelGraphics/ModelGraphicsItems/ItemCrashDI.h \
    ModelGraphics/ModelGraphicsItems/ItemCrashDIpos.h \
    ModelGraphics/ModelGraphicsItems/ItemDI.h \
    ModelGraphics/ModelGraphicsItems/ItemDO.h \
    ModelGraphics/ModelGraphicsItems/ItemLaser.h \
    ModelGraphics/ModelGraphicsItems/ItemMagneticSensor.h \
    ModelGraphics/ModelGraphicsItems/ItemPolygon.h \
    ModelGraphics/ModelGraphicsItems/ItemPolypos.h \
    ModelGraphics/ModelGraphicsItems/ItemRFID.h \
    ModelGraphics/ModelGraphicsItems/ItemRobotModel.h \
    ModelGraphics/ModelGraphicsItems/ItemScanArea.h \
    ModelGraphics/ModelGraphicsItems/ItemUltrasonic.h \
    ModelGraphics/SettingWidgets/WidgetCircleRobotModelSetting.h \
    ModelGraphics/SettingWidgets/WidgetCrashDiPosSetting.h \
    ModelGraphics/SettingWidgets/WidgetCrashDISetting.h \
    ModelGraphics/SettingWidgets/WidgetDISetting.h \
    ModelGraphics/SettingWidgets/WidgetDOSetting.h \
    ModelGraphics/SettingWidgets/WidgetDOState.h \
    ModelGraphics/SettingWidgets/WidgetGuidance.h \
    ModelGraphics/SettingWidgets/WidgetLaserSetting.h \
    ModelGraphics/SettingWidgets/WidgetMagneticSensorSetting.h \
    ModelGraphics/SettingWidgets/WidgetModelSetting.h \
    ModelGraphics/SettingWidgets/WidgetPlygonSetting.h \
    ModelGraphics/SettingWidgets/WidgetPosSetting.h \
    ModelGraphics/SettingWidgets/WidgetRectRobotModelSetting.h \
    ModelGraphics/SettingWidgets/WidgetRFIDsetting.h \
    ModelGraphics/SettingWidgets/WidgetRobotModelSetting.h \
    ModelGraphics/SettingWidgets/WidgetSetting.h \
    ModelGraphics/SettingWidgets/WidgetUltrasonicSetting.h \
    ModelGraphics/ModelBasicSettingWidget.h \
    ModelGraphics/ModelBtn.h \
    ModelGraphics/ModelGraphicsScene.h \
    ModelGraphics/ModelGraphicsView.h \
    ModelGraphics/ModelPropertySettingWidget.h \
    ModelGraphics/PropertyBasicWidget.h \
    ModelHelper/GlobalVariant.h \
    ModelHelper/ModelCustomCommand.h \
    ModelHelper/ModelCustomWidget.h \
    ModelHelper/ModelHelper.h \
    ModelHelper/PropertyMissPromtWidget.h \
    ModelHelper/UndoStack.h \
    DialogModelChose.h \
    DialogSaveAsTemplate.h \
    EditRobotModelWidget.h \
    ModelCommon.h \
    RoModelPlugin.h \
    SCRobotFileModelWidget.h \
    ModelGraphics/SettingWidgets/WidgetCameraSetting.h \
    ModelGraphics/ModelGraphicsItems/ItemCamera.h \
    ModelGraphics/SettingWidgets/WidgetForkSetting.h \
    ModelGraphics/GestureBaseWidget.h

SOURCES += \
    ModelGraphics/GuidenceWidgets/BasicContentSetting.cpp \
    ModelGraphics/GuidenceWidgets/CrashDiContentSetting.cpp \
    ModelGraphics/GuidenceWidgets/DIContentSetting.cpp \
    ModelGraphics/GuidenceWidgets/DOContentSetting.cpp \
    ModelGraphics/GuidenceWidgets/GuidBaseSetting.cpp \
    ModelGraphics/GuidenceWidgets/GuidBasicSetting.cpp \
    ModelGraphics/GuidenceWidgets/GuidChassisShape.cpp \
    ModelGraphics/GuidenceWidgets/GuidChassisTypeSelect.cpp \
    ModelGraphics/GuidenceWidgets/GuidDISetting.cpp \
    ModelGraphics/GuidenceWidgets/GuidDoSetting.cpp \
    ModelGraphics/GuidenceWidgets/GuidFinishedWid.cpp \
    ModelGraphics/GuidenceWidgets/GuidLaserSetting.cpp \
    ModelGraphics/GuidenceWidgets/GuidMagneticSensorSetting.cpp \
    ModelGraphics/GuidenceWidgets/GuidModelName.cpp \
    ModelGraphics/GuidenceWidgets/GuidoptionalDevice.cpp \
    ModelGraphics/GuidenceWidgets/GuidRFIDSetting.cpp \
    ModelGraphics/GuidenceWidgets/GuidSimpleDevice.cpp \
    ModelGraphics/GuidenceWidgets/GuidUltrasonicSetting.cpp \
    ModelGraphics/GuidenceWidgets/LaserContentSetting.cpp \
    ModelGraphics/GuidenceWidgets/MagSensorContentSetting.cpp \
    ModelGraphics/GuidenceWidgets/NumSetting.cpp \
    ModelGraphics/GuidenceWidgets/UltrasonicContentSetting.cpp \
    ModelGraphics/ModelGraphicsItems/ItemAngelCtroller.cpp \
    ModelGraphics/ModelGraphicsItems/ItemAxis.cpp \
    ModelGraphics/ModelGraphicsItems/ItemBase.cpp \
    ModelGraphics/ModelGraphicsItems/ItemCrashDI.cpp \
    ModelGraphics/ModelGraphicsItems/ItemCrashDIpos.cpp \
    ModelGraphics/ModelGraphicsItems/ItemDI.cpp \
    ModelGraphics/ModelGraphicsItems/ItemDO.cpp \
    ModelGraphics/ModelGraphicsItems/ItemLaser.cpp \
    ModelGraphics/ModelGraphicsItems/ItemMagneticSensor.cpp \
    ModelGraphics/ModelGraphicsItems/ItemPolygon.cpp \
    ModelGraphics/ModelGraphicsItems/ItemPolypos.cpp \
    ModelGraphics/ModelGraphicsItems/ItemRFID.cpp \
    ModelGraphics/ModelGraphicsItems/ItemRobotModel.cpp \
    ModelGraphics/ModelGraphicsItems/ItemScanArea.cpp \
    ModelGraphics/ModelGraphicsItems/ItemUltrasonic.cpp \
    ModelGraphics/SettingWidgets/WidgetCircleRobotModelSetting.cpp \
    ModelGraphics/SettingWidgets/WidgetCrashDiPosSetting.cpp \
    ModelGraphics/SettingWidgets/WidgetCrashDISetting.cpp \
    ModelGraphics/SettingWidgets/WidgetDISetting.cpp \
    ModelGraphics/SettingWidgets/WidgetDOSetting.cpp \
    ModelGraphics/SettingWidgets/WidgetDOState.cpp \
    ModelGraphics/SettingWidgets/WidgetGuidance.cpp \
    ModelGraphics/SettingWidgets/WidgetLaserSetting.cpp \
    ModelGraphics/SettingWidgets/WidgetMagneticSensorSetting.cpp \
    ModelGraphics/SettingWidgets/WidgetModelSetting.cpp \
    ModelGraphics/SettingWidgets/WidgetPlygonSetting.cpp \
    ModelGraphics/SettingWidgets/WidgetPosSetting.cpp \
    ModelGraphics/SettingWidgets/WidgetRectRobotModelSetting.cpp \
    ModelGraphics/SettingWidgets/WidgetRFIDsetting.cpp \
    ModelGraphics/SettingWidgets/WidgetRobotModelSetting.cpp \
    ModelGraphics/SettingWidgets/WidgetSetting.cpp \
    ModelGraphics/SettingWidgets/WidgetUltrasonicSetting.cpp \
    ModelGraphics/ModelBasicSettingWidget.cpp \
    ModelGraphics/ModelBtn.cpp \
    ModelGraphics/ModelGraphicsScene.cpp \
    ModelGraphics/ModelGraphicsView.cpp \
    ModelGraphics/ModelPropertySettingWidget.cpp \
    ModelGraphics/PropertyBasicWidget.cpp \
    ModelHelper/GlobalVariant.cpp \
    ModelHelper/ModelCustomCommand.cpp \
    ModelHelper/ModelCustomWidget.cpp \
    ModelHelper/ModelHelper.cpp \
    ModelHelper/PropertyMissPromtWidget.cpp \
    ModelHelper/UndoStack.cpp \
    DialogModelChose.cpp \
    DialogSaveAsTemplate.cpp \
    EditRobotModelWidget.cpp \
    RoModelPlugin.cpp \
    SCRobotFileModelWidget.cpp \
    ModelGraphics/SettingWidgets/WidgetCameraSetting.cpp \
    ModelGraphics/ModelGraphicsItems/ItemCamera.cpp \
    ModelGraphics/SettingWidgets/WidgetForkSetting.cpp \
    ModelGraphics/GestureBaseWidget.cpp

