#ifndef WIDGETGUIDANCE_H
#define WIDGETGUIDANCE_H

#include<QDialog>
#include<QLabel>
#include"GuidenceWidgets/GuidChassisTypeSelect.h"
#include"GuidenceWidgets/GuidDISetting.h"
#include"GuidenceWidgets/GuidDoSetting.h"
#include"GuidenceWidgets/GuidLaserSetting.h"
#include"GuidenceWidgets/GuidMagneticSensorSetting.h"
#include"GuidenceWidgets/GuidUltrasonicSetting.h"
#include"GuidenceWidgets/GuidFinishedWid.h"
#include"GuidenceWidgets/GuidBasicSetting.h"
#include"GuidenceWidgets/GuidRFIDSetting.h"
#include"GuidenceWidgets/GuidChassisShape.h"
#include"GuidenceWidgets/GuidModelName.h"
#include"GuidenceWidgets/GuidSimpleDevice.h"
#include"GuidenceWidgets/GuidoptionalDevice.h"
#include "CustomBaseDialog/CustomChildBaseDialog.h"

namespace Ui {
class WidgetGuidance;
}

class WidgetGuidance : public CustomChildBaseDialog
{
    Q_OBJECT

public:
    explicit WidgetGuidance(QWidget *parent = 0);
    ~WidgetGuidance();
    void SetGuidInstruction();
    void setNextStepEnabled(GuidType type);
    void ShowCurrentGuidWid(int idx);
    QList<GuidBaseSetting*>* getGuidWidList();
     bool willCloseWidget();
     void setfinished(bool flg);
signals:
    void sig_finished();
private slots:
    void on_pushButton_backward_clicked();

    void on_pushButton_forward_clicked();
    void Slot_updateForwardStatus();
    void Slot_enableForwardBtn(bool flag);

    void  Slot_shape_changed(ShapeType shp);
private:
    Ui::WidgetGuidance *ui;

    GuidChassisTypeSelect* m_chassisWid;
    GuidBasicSetting* m_basicWid;
    GuidDISetting* m_diWid;
    GuidDoSetting* m_doWid;
    GuidLaserSetting* m_laserWid;
    GuidMagneticSensorSetting* m_magneticWid;
    GuidUltrasonicSetting* m_ultrasonicWid;
    GuidFinishedWid* m_finishedWid;
    GuidRFIDSetting* m_RFIDWid;
    GuidModelName* m_name;
    GuidChassisShape* m_shapeWid;
    GuidSimpleDevice* m_simpleDev;
    GuidoptionalDevice* m_optionalDev;
    QList<GuidBaseSetting*> m_guidWidList;
    static int m_idx ;
    QList<QLabel*> m_guidArrowList;
    QList<QLabel*> m_LocationLabelList;
    bool m_isFinished = false;
};

#endif // WIDGETGUIDANCE_H
