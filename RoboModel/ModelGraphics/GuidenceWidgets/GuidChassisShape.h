#ifndef GUIDCHASSISSHAPE_H
#define GUIDCHASSISSHAPE_H

#include"GuidBaseSetting.h"
namespace Ui {
class GuidChassisShape;
}

class GuidChassisShape : public GuidBaseSetting
{
    Q_OBJECT

public:
    explicit GuidChassisShape(QWidget *parent = 0);
    ~GuidChassisShape();
    ShapeType getShape();
    void InitUI();
    void enableNextStep();
    void checkIfNeedEnableByShape();
signals:
    void sig_shape_change(ShapeType);
private slots:
    void on_comboBox_shape_currentIndexChanged(int index);

    void on_doubleSpinBox_head_valueChanged(double arg1);

    void on_doubleSpinBox_tail_valueChanged(double arg1);

    void on_doubleSpinBox_width_valueChanged(double arg1);

    void on_doubleSpinBox_radius_valueChanged(double arg1);

private:
    Ui::GuidChassisShape *ui;

    ShapeType ShapeType_;
    QImage image_;

    bool radiusFlag;
    bool headFlag;
    bool tailFlag;
    bool widthFlag;
public:
    RobotModelData robotData_;
private:
    QString imagePath_;
};

#endif // GUIDCHASSISSHAPE_H
