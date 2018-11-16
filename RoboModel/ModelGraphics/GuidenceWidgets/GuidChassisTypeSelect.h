#ifndef GUIDCHASSISTYPESELECT_H
#define GUIDCHASSISTYPESELECT_H
#include"GuidBaseSetting.h"
#include <QWidget>

namespace Ui {
class GuidChassisTypeSelect;
}
class GuidChassisShape;
class GuidChassisTypeSelect : public GuidBaseSetting
{
    Q_OBJECT

public:
    explicit GuidChassisTypeSelect(GuidChassisShape *shpe = NULL, QWidget *parent = 0);
    ~GuidChassisTypeSelect();
    void initWidgetData();
     ConfigureData readAbsEncoderTypeFile(const QString& filePath);
     void enableNextStep();
     void checkIfNeedEnableByFuncType();
     void updateData();//切换窗口更新来自前一个窗口传来的最新数据
public slots:
     void slot_shape_currentIndexChanged(ShapeType shape);
private slots:

    void on_comboBox_moveStyle_currentIndexChanged(int index);

    void on_comboBox_funcType_currentIndexChanged(int index);

    void on_doubleSpinBox_wheelRadius_valueChanged(double arg1);

    void on_doubleSpinBox_Aaxis_valueChanged(double arg1);

    void on_doubleSpinBox_Baxis_valueChanged(double arg1);

    void on_doubleSpinBox_theta_valueChanged(double arg1);

    void on_doubleSpinBox_steerMinAngel_valueChanged(double arg1);

    void on_doubleSpinBox_steerMaxAngel_valueChanged(double arg1);

    void on_doubleSpinBox_encoderMin_valueChanged(double arg1);

    void on_doubleSpinBox_encoderMax_valueChanged(double arg1);

    void on_comboBox_encoderType_currentIndexChanged(int index);

private:
    Ui::GuidChassisTypeSelect *ui;
    bool AaxisFlag;
    bool BaxisFlag;
    bool angleFlag;
public:
    RobotModelData roboData_;
    QString imagePath_;//图片路径
private:
    GuidChassisShape* shapeWid_;
};

#endif // GUIDCHASSISTYPESELECT_H
