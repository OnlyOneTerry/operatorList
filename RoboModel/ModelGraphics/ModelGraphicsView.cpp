#include "ModelGraphicsView.h"
#include <QDebug>
#include <QJsonObject>
#include <QKeyEvent>
#include "WidgetModelSetting.h"
#include "QWidget"
#include "ModelBtn.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollBar>
#include<QMessageBox>
#include"UndoStack.h"
#include"StyleObject.h"
ModelGraphicsView::ModelGraphicsView(QWidget *parent):
    SCBaseGraphicsView(parent),
    model_scene_(new ModelGraphicsScene(this)),
    widget_basic_setting_(new ModelBasicSettingWidget(this)),
    widget_property_setting_(new ModelPropertySettingWidget(this)),
    btn_polygon_lowspeed_(new ModelBtn(0,this)),
    btn_polygon_safe_(new ModelBtn(2,this)),
    btn_polygon_stop_(new ModelBtn(1,this)),
    btn_laser_(new ModelBtn(10,this)),
    btn_ultrasonic_(new ModelBtn(11,this)),
    btn_DI_(new ModelBtn(12,this)),
    btn_magneticScesor_(new ModelBtn(13,this)),
    btn_DO_(new ModelBtn(14,this)),
    btn_DI_crash_(new ModelBtn(15,this)),
    btn_RFID_(new ModelBtn(16,this)),
    btn_Vision_(new ModelBtn(17,this)),
    btn_scale_1x_(new QPushButton("1x",this)),
    btn_scale_2x_(new QPushButton("2x",this)),
    btn_scale_3x_(new QPushButton("3x",this)),
    m_undoStack(NULL)

{
    setScene (model_scene_);
    QMatrix view_matrix(1,0,0 ,-1,0,0);
    view_matrix.rotate (90);
    setMatrix(view_matrix,true);
    setRenderHint(QPainter::HighQualityAntialiasing);
    setViewportUpdateMode(FullViewportUpdate);//刷新视口
    init_layout ();

    reset_property_widget ();

    m_ori_scale = 2;
    set_scale_size(2);
    m_undoStack = UndoStack::InitStack();

}

ModelGraphicsView::~ModelGraphicsView()
{

}

ModelPropertySettingWidget *ModelGraphicsView::property_widget() const
{
    return widget_property_setting_;
}

ModelBasicSettingWidget *ModelGraphicsView::basic_widget() const
{
    return widget_basic_setting_;
}

void ModelGraphicsView::setBasic_widget(ModelBasicSettingWidget *basicWid)
{
    widget_basic_setting_ = basicWid;
}

void ModelGraphicsView::mousePressEvent (QMouseEvent *event)
{
    SCBaseGraphicsView::mousePressEvent (event);
    //拖动视口
    if(event->button()==Qt::MidButton)
    {
        beforePos = event->pos();
    }

}
void ModelGraphicsView::mouseMoveEvent (QMouseEvent *event)
{
    SCBaseGraphicsView::mouseMoveEvent (event);

    if(event->buttons()&Qt::LeftButton)
    {


    }
    //拖动视口
    if(event->buttons()==Qt::MidButton)
    {
        afterPos = event->pos();
        qreal deltaX = afterPos.x()-beforePos.x();
        qreal deltaY = afterPos.y()-beforePos.y();

        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value()-deltaY);
        this->horizontalScrollBar()->setValue(this->horizontalScrollBar()->value()-deltaX);
        beforePos  = afterPos;

    }
}
void ModelGraphicsView::mouseReleaseEvent (QMouseEvent *event)
{
    SCBaseGraphicsView::mouseReleaseEvent (event);
    setCursor(Qt::ArrowCursor);
}
void ModelGraphicsView::mouseDoubleClickEvent (QMouseEvent *event)
{
    SCBaseGraphicsView::mouseDoubleClickEvent (event);
}

void ModelGraphicsView::wheelEvent(QWheelEvent *event)
{
    SCBaseGraphicsView::wheelEvent(event);
    // 滚轮的滚动量
    QPoint scrollAmount = event->angleDelta();
    // 正值表示滚轮远离使用者（放大），负值表示朝向使用者（缩小）
    this->verticalScrollBar()->setValue(this->verticalScrollBar()->value()+ scrollAmount.x()/2);
}

/*******************************************************
 * Description: 初始化界面
 *
 * @para
 *******************************************************/
void ModelGraphicsView::init_layout()
{
    verticalLayout = new QVBoxLayout(this);
    verticalLayout->setSpacing(0);
    verticalLayout->setContentsMargins(6,0,6,6);

    hlayout_top = new QHBoxLayout();
    hlayout_top->setSpacing(0);

    vlayout_basic_setting = new QVBoxLayout();
    vlayout_basic_setting->setSpacing(6);
    vlayout_basic_setting->setContentsMargins(12,25,25,0);
    vlayout_basic_setting->addWidget (widget_basic_setting_);
    hlayout_top->addLayout(vlayout_basic_setting);

    space1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout_top->addItem(space1);


    vlayout_property_setting = new QVBoxLayout();
    vlayout_property_setting->setSpacing(6);
    vlayout_property_setting->setContentsMargins(10,25,10,0);
    vlayout_property_setting->addWidget (widget_property_setting_);
    hlayout_top->addLayout(vlayout_property_setting);

    hlayout_bottom = new QHBoxLayout();
    hlayout_bottom->setSpacing(6);
    hlayout_bottom->setContentsMargins(20,0,20,20);

    space_botton_left_ = new QSpacerItem(96, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
    hlayout_bottom->addItem(space_botton_left_);

    space2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout_bottom->addItem(space2);

    hlayout_btn = new QHBoxLayout();
    hlayout_btn->setSpacing(6);
    hlayout_bottom->addLayout(hlayout_btn);

    space3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout_bottom->addItem(space3);

    {
        btn_scale_1x_->setStyleSheet ("QPushButton{ font-family:'\"Microsoft YaHei\"';font-size:12px;font:bold;}");
        btn_scale_2x_->setStyleSheet ("QPushButton{ font-family:'\"Microsoft YaHei\"';font-size:12px;font:bold;}");
        btn_scale_3x_->setStyleSheet ("QPushButton{ font-family:'\"Microsoft YaHei\"';font-size:12px;font:bold;}");

        btn_scale_1x_->setFixedSize(25,25);
        btn_scale_2x_->setFixedSize(25,25);
        btn_scale_3x_->setFixedSize(25,25);

        hlayout_bottom->addWidget(btn_scale_1x_);
        hlayout_bottom->addWidget(btn_scale_2x_);
        hlayout_bottom->addWidget(btn_scale_3x_);


        connect(btn_scale_1x_,SIGNAL(pressed()),this,SLOT(slot_btn_scale_1x()));
        connect(btn_scale_2x_,SIGNAL(pressed()),this,SLOT(slot_btn_scale_2x()));
        connect(btn_scale_3x_,SIGNAL(pressed()),this,SLOT(slot_btn_scale_3x()));
    }

    verticalLayout->addLayout(hlayout_top);
    verticalLayout->addLayout(hlayout_bottom);

    //    vlayout_basic_setting->addWidget (widget_basic_setting_);
    //    vlayout_property_setting->addWidget (widget_property_setting_);
    QString imagePath = StyleObject::init()->getCurrentSkinDirPath()+"/images/"+"/RoModelPlugin/";
    btn_laser_->setIcon (QIcon(imagePath+"btn_laser"));
    btn_laser_->setToolTip (tr("laser"));
    hlayout_btn->addWidget (btn_laser_);
    connect (btn_laser_,SIGNAL(sig_add_polygon(QPoint,int)),model_scene(),SLOT(slot_add_polygon(QPoint,int)));

    btn_ultrasonic_->setIcon (QIcon(imagePath+"btn_ultrasonic"));
    btn_ultrasonic_->setToolTip (tr("ultrasonic"));
    hlayout_btn->addWidget (btn_ultrasonic_);
    connect (btn_ultrasonic_,SIGNAL(sig_add_polygon(QPoint,int)),model_scene(),SLOT(slot_add_polygon(QPoint,int)));

    btn_DI_->setIcon (QIcon(imagePath+"btn_DI"));
    btn_DI_->setToolTip (tr("DI"));
    hlayout_btn->addWidget (btn_DI_);
    connect (btn_DI_,SIGNAL(sig_add_polygon(QPoint,int)),model_scene(),SLOT(slot_add_polygon(QPoint,int)));
    // 防碰撞DI
    btn_DI_crash_->setIcon (QIcon(imagePath+"btn_crashDI"));
    btn_DI_crash_->setToolTip (tr("crashSwitch"));
    hlayout_btn->addWidget (btn_DI_crash_);
    connect (btn_DI_crash_,SIGNAL(sig_add_polygon(QPoint,int)),model_scene(),SLOT(slot_add_polygon(QPoint,int)));


    btn_DO_->setIcon (QIcon(imagePath+"btn_DO"));
    btn_DO_->setToolTip (tr("DO"));
    hlayout_btn->addWidget (btn_DO_);
    connect (btn_DO_,SIGNAL(sig_add_polygon(QPoint,int)),model_scene(),SLOT(slot_add_polygon(QPoint,int)));


    btn_magneticScesor_->setIcon (QIcon(imagePath+"btn_magneticSensor"));
    btn_magneticScesor_->setToolTip (tr("magneticSensor"));
    hlayout_btn->addWidget (btn_magneticScesor_);
    connect (btn_magneticScesor_,SIGNAL(sig_add_polygon(QPoint,int)),model_scene(),SLOT(slot_add_polygon(QPoint,int)));

    btn_RFID_->setIcon(QIcon(imagePath+"btn_RFID"));
    btn_RFID_->setToolTip(tr("RFID"));
    hlayout_btn->addWidget(btn_RFID_);
    connect(btn_RFID_,SIGNAL(sig_add_polygon(QPoint,int)),model_scene(),SLOT(slot_add_polygon(QPoint,int)));

    btn_Vision_->setIcon(QIcon(imagePath+"btn_vision"));
    btn_Vision_->setToolTip(tr("vision"));
    hlayout_btn->addWidget(btn_Vision_);
    connect(btn_Vision_,SIGNAL(sig_add_polygon(QPoint,int)),model_scene(),SLOT(slot_add_polygon(QPoint,int)));

}


void ModelGraphicsView::keyPressEvent (QKeyEvent *event)
{
    if(event->key ()==Qt::Key_Backspace||event->key ()==Qt::Key_Delete)
    {
        model_scene_->delete_operation ();
    }

    if(event->key()==Qt::Key_Space)
    {

    }

    QGraphicsView::keyPressEvent (event);
}

void ModelGraphicsView::resizeEvent(QResizeEvent *event)
{
    SCBaseGraphicsView::resizeEvent(event);
    centerOn(QPointF(0,0));
}

ModelGraphicsScene *ModelGraphicsView::model_scene() const
{
    return model_scene_;
}

void ModelGraphicsView::reset_property_widget()
{
    if(property_widget ()->get_current_property_widget ())
    {
        if(property_widget ()->get_current_property_widget ()->type ()!=WidgetModelSetting::Type)
        {
            WidgetModelSetting* tep_WidModelSeting = model_scene()->model_view()->property_widget()->getWidgetModelSetting();
            model_scene()->model_view()->property_widget()->update_property_widget(tep_WidModelSeting);
        }
    }
}

QVariantMap ModelGraphicsView::get_model_variant_map() const
{
    QVariantMap variant_map_chassis;               //底盘
    QVariantMap variant_map_chassis_param;
    QVariantMap variant_map_vision;                //摄像头
    QVariantMap variant_map_fork;                   //叉车
    QVariantMap variant_map_model;                 //模型
    QVariantMap variant_map_Arm;
    QVariantMap variant_map_Jack;
    QVariantMap variant_map_Roller;
    //根据底盘的形状类型添加相应属性参数
    variant_map_chassis.insert("shape",model_scene()->get_robot_model_data().shapType_);
    ShapeType shape = model_scene()->get_robot_model_data().shapType_;
    switch (shape) {
    case RECTANGLE:
    {
        get_rect_model_variant_map(variant_map_chassis,variant_map_chassis_param);
    }
        break;
    case CIRCLE:
    {
        get_circle_model_variant_map(variant_map_chassis,variant_map_chassis_param);
    }

    default:
        break;
    }

    {
        variant_map_chassis.insert ("gyro",                      basic_widget()->model_base_data_.gyro_type_);
        variant_map_chassis.insert ("autoGyroCal",          basic_widget()->model_base_data_.auto_gyro_cal_);
        variant_map_chassis.insert ("brake",                     (int)basic_widget()->model_base_data_.brake_);
        variant_map_chassis.insert ("autoBrake",              basic_widget()->model_base_data_.auto_brake_);
        variant_map_chassis.insert ("batteryInfo",             basic_widget()->model_base_data_.battery_info_);
        variant_map_chassis.insert ("LED",                        (int)basic_widget()->model_base_data_.LED_);
        variant_map_chassis.insert("mode_param",                    variant_map_chassis_param);
    }

    if(basic_widget()->getForkWidget()->getIsConfigureFork())//配置叉车
    {
        variant_map_fork.insert("blockLaserDist",basic_widget()->getForkWidget()->getForkdata().blockLaserDist);
        variant_map_fork.insert("encoderSteps",basic_widget()->getForkWidget()->getForkdata().encoderSteps);
        variant_map_fork.insert("lengthFactor",basic_widget()->getForkWidget()->getForkdata().lengthFactor);
        variant_map_fork.insert("lengthPerTurn",basic_widget()->getForkWidget()->getForkdata().lengthPerTurn);
        variant_map_fork.insert("maxHeight",basic_widget()->getForkWidget()->getForkdata().maxHeight);
        variant_map_fork.insert("minHeight",basic_widget()->getForkWidget()->getForkdata().minHeight);
        variant_map_fork.insert("pressureSensor",basic_widget()->getForkWidget()->getForkdata().pressureSensor);
        variant_map_fork.insert("pump",basic_widget()->getForkWidget()->getForkdata().pump);
        variant_map_fork.insert("type",basic_widget()->getForkWidget()->getForkdata().type);
        variant_map_fork.insert("downBuffer",basic_widget()->getForkWidget()->getForkdata().downBuffer);
        variant_map_fork.insert("upBuffer",basic_widget()->getForkWidget()->getForkdata().upBuffer);
        //若配置了叉车则写入
        variant_map_model.insert("fork",                      variant_map_fork);
    }

    /***************************************************************/

    variant_map_model.insert ("model",                basic_widget()->model_base_data_.model_name_);
    variant_map_model.insert ("chassis",               variant_map_chassis);
    variant_map_model.insert ("laser",                   model_scene_->get_laser_data ());
    variant_map_model.insert ("ultrasonic",           model_scene_->get_ultrasonic_data ());
    variant_map_model.insert ("DI",                       model_scene_->get_di_data ());
    variant_map_model.insert ("DO",                     model_scene_->get_do_data());
    variant_map_model.insert ("magneticSensor",  model_scene_->get_magnetic_data ());
    variant_map_model.insert ("RFID",                   model_scene()->get_RFID_data());
    variant_map_model.insert ("vision",                 model_scene()->get_Camera_data());
    variant_map_model.insert ("vision",                 model_scene()->get_Camera_data());
    variant_map_model.insert ("Arm",                    variant_map_Arm);
    variant_map_model.insert ("Jack",                   variant_map_Jack);
    variant_map_model.insert ("Roller",                 variant_map_Roller);

    return variant_map_model;
}

void ModelGraphicsView::get_rect_model_variant_map(QVariantMap &variant_map_chassis,QVariantMap& variant_map_chassis_param) const
{
    variant_map_chassis.insert ("width",qRound(model_scene_->get_robot_model_data ().width_*10)/1000.0);
    variant_map_chassis.insert ("head",qRound(model_scene_->get_robot_model_data ().head_*10)/1000.0);
    variant_map_chassis.insert ("tail",qRound(model_scene_->get_robot_model_data ().tail_*10)/1000.0);
    variant_map_chassis.insert ("radius",0.0);

    variant_map_chassis.insert("mode",(int)model_scene_->getItem_robot_model()->robot_model_data_.mvStyle_);

    variant_map_chassis_param.insert("type",model_scene_->getItem_robot_model()->robot_model_data_.chasisType_);
    variant_map_chassis_param.insert("wheelRadius",model_scene_->get_robot_model_data().wheelRadius_*10/1000.0);
    variant_map_chassis_param.insert("reductionRatio", basic_widget()->model_base_data_.reduction_ratio_);
    variant_map_chassis_param.insert("encoderLine",basic_widget()->model_base_data_.encoder_line_);
    variant_map_chassis_param.insert("maxMotorRpm",basic_widget()->model_base_data_.max_motor_rpm_);
    variant_map_chassis_param.insert("inverse",  model_scene()->getRobotInverse_data());
    variant_map_chassis_param.insert("driverID",model_scene()->getRobotDriverId_data());
    variant_map_chassis_param.insert("driver",  20);
    variant_map_chassis_param.insert("driverBrand",basic_widget()->model_base_data_.driverBrand_);


    ChassisType chasisType =model_scene_->get_robot_model_data().chasisType_;
    switch (chasisType) {
    case DIFFERENTIAL_TWOWHEEL://差动两轮
    {
        variant_map_chassis_param.insert("A",model_scene_->get_robot_model_data().chasisModel.DiffTwoWheel.a_axis_distance/100);
    }
        break;
    case DIFFERENTIAL_FOURWHEEL://差动四轮
    {
        variant_map_chassis_param.insert("A",model_scene_->get_robot_model_data().chasisModel.DiffFourWheel.a_axis_distance/100);
        variant_map_chassis_param.insert("B",model_scene_->get_robot_model_data().chasisModel.DiffFourWheel.b_axis_distance/100);
    }
        break;
    case ALLDIRECTION_THREEWHEEL://全向三轮
    {
        variant_map_chassis_param.insert("A",model_scene_->get_robot_model_data().chasisModel.AllDirectThreeWheel.wheel_corePoint_distance/100);
        variant_map_chassis_param.insert("theta",model_scene_->get_robot_model_data().chasisModel.AllDirectThreeWheel.firstWheel_delta_angle);
    }
        break;
    case MECANUM_FOURWHEEL://四轮麦克纳姆
    {
        variant_map_chassis_param.insert("A",model_scene_->get_robot_model_data().chasisModel.MecanumFourWheel.a_axis_distance/100);
        variant_map_chassis_param.insert("B",model_scene_->get_robot_model_data().chasisModel.MecanumFourWheel.b_axis_distance/100);
    }
        break;
    case SINGLESTEERWHEEL_FRONTDRIVER_FRONT_CODERTYPE://前驱+前转动编码器
    case SINGLESTEERWHEEL_FRONTDRIVER_BACK_CODERTYPE://前驱+后转动编码器
    case SINGLESTEERWHEEL_BACKDRIVER_FRONT_CODERTYPE://后驱+前转动编码器
    case SINGLESTEERWHEEL_BACKDRIVER_BACK_CODERTYPE://后驱+后转动编码器
    {
        //        variant_map_chassis_param.insert("type",model_scene_->getItem_robot_model()->robot_model_data_.chasisType_);
        //        variant_map_chassis_param.insert("wheelRadius",model_scene_->get_robot_model_data().wheelRadius_/100.0);
        //        variant_map_chassis_param.insert("reductionRatio",basic_widget()->model_base_data_.reduction_ratio_);
        //        variant_map_chassis_param.insert("encoderLine",basic_widget()->model_base_data_.encoder_line_);
        //        variant_map_chassis_param.insert("maxMotorRpm",basic_widget()->model_base_data_.max_motor_rpm_);
        //        variant_map_chassis_param.insert("inverse",  model_scene()->getRobotInverse_data());
        //        variant_map_chassis_param.insert("driverID",model_scene()->getRobotDriverId_data());
        //        variant_map_chassis_param.insert("driver",  basic_widget()->model_base_data_.driver_);
        //        variant_map_chassis_param.insert("drvierBrand",basic_widget()->model_base_data_.driverBrand_);

        variant_map_chassis_param.insert("A",model_scene_->get_robot_model_data().chasisModel.SingleWheelDriver.a_axis_distance/100.0);
        variant_map_chassis_param.insert("B",model_scene_->get_robot_model_data().chasisModel.SingleWheelDriver.b_axis_distance/100.0);
        variant_map_chassis_param.insert("absEncoder",  model_scene_->get_robot_model_data().chasisModel.SingleWheelDriver.absEncoderType);
        QVariantList encoderRange;
        encoderRange.append(model_scene_->get_robot_model_data().chasisModel.SingleWheelDriver.absEncoderRange_min);
        encoderRange.append(model_scene_->get_robot_model_data().chasisModel.SingleWheelDriver.absEncoderRange_max);
        QVariantList steer_angle;
        steer_angle.append(model_scene_->get_robot_model_data().chasisModel.SingleWheelDriver.steer_angle_min);
        steer_angle.append(model_scene_->get_robot_model_data().chasisModel.SingleWheelDriver.steer_angle_max);
        variant_map_chassis_param.insert("absEncoderRange",  encoderRange);
        variant_map_chassis_param.insert("steerAngle",  steer_angle);
    }
        break;
    default:
        break;
    }
}

void ModelGraphicsView::get_circle_model_variant_map(QVariantMap &map1, QVariantMap &variant_map_chassis_param) const
{
    map1.insert("width",0.0);
    map1.insert("head",0.0);
    map1.insert("tail",0.0);
    map1.insert("radius",qRound(model_scene()->get_robot_model_data().radius_*10)/1000.0);
    map1.insert("mode",(int)model_scene_->get_robot_model_data().mvStyle_);

    variant_map_chassis_param.insert("type",model_scene_->get_robot_model_data().chasisType_);
    variant_map_chassis_param.insert("wheelRadius",model_scene_->get_robot_model_data().wheelRadius_*10/1000.0);
    variant_map_chassis_param.insert("reductionRatio", basic_widget()->model_base_data_.reduction_ratio_);
    variant_map_chassis_param.insert("encoderLine",basic_widget()->model_base_data_.encoder_line_);
    variant_map_chassis_param.insert("maxMotorRpm",basic_widget()->model_base_data_.max_motor_rpm_);
    variant_map_chassis_param.insert("inverse",  model_scene()->getRobotInverse_data());
    variant_map_chassis_param.insert("driverID",model_scene()->getRobotDriverId_data());
    variant_map_chassis_param.insert("driver",  basic_widget()->model_base_data_.driver_);
    variant_map_chassis_param.insert("driverBrand",basic_widget()->model_base_data_.driverBrand_);

    ChassisType chasisType =model_scene_->get_robot_model_data().chasisType_;
    switch (chasisType) {
    case DIFFERENTIAL_TWOWHEEL://差动两轮
    {
        variant_map_chassis_param.insert("A",model_scene_->get_robot_model_data().chasisModel.DiffTwoWheel.a_axis_distance/100);
    }
        break;
    case DIFFERENTIAL_FOURWHEEL://差动四轮
    {
        variant_map_chassis_param.insert("A",model_scene_->get_robot_model_data().chasisModel.DiffFourWheel.a_axis_distance/100);
        variant_map_chassis_param.insert("B",model_scene_->get_robot_model_data().chasisModel.DiffFourWheel.b_axis_distance/100);
    }
        break;
    case ALLDIRECTION_THREEWHEEL://全向三轮
    {
        //        variant_map_chassis_param.insert("type",model_scene_->get_robot_model_data().chasisType_);
        //        variant_map_chassis_param.insert("wheelRadius",model_scene_->get_robot_model_data().wheelRadius_*10/1000.0);
        //        variant_map_chassis_param.insert("reductionRatio", basic_widget()->model_base_data_.reduction_ratio_);
        //        variant_map_chassis_param.insert("encoderLine",basic_widget()->model_base_data_.encoder_line_);
        //        variant_map_chassis_param.insert("maxMotorRpm",basic_widget()->model_base_data_.max_motor_rpm_);
        //        variant_map_chassis_param.insert("inverse",  model_scene()->getRobotInverse_data());
        //        variant_map_chassis_param.insert("driverID",model_scene()->getRobotDriverId_data());
        //        variant_map_chassis_param.insert("driver",  basic_widget()->model_base_data_.driver_);
        //        variant_map_chassis_param.insert("drvierBrand",basic_widget()->model_base_data_.driverBrand_);

        variant_map_chassis_param.insert("A",model_scene_->get_robot_model_data().chasisModel.AllDirectThreeWheel.wheel_corePoint_distance/100);
        variant_map_chassis_param.insert("theta",model_scene_->get_robot_model_data().chasisModel.AllDirectThreeWheel.firstWheel_delta_angle);
    }
        break;
    default:
        break;
    }
}

void ModelGraphicsView::set_model_jobj(const QJsonObject &json_object_robot_model)
{
    json_object_robot_model_=QJsonObject(json_object_robot_model);
    parse_json ();
}

void ModelGraphicsView::parse_json()
{    
    basic_widget()->model_base_data_.model_name_               =json_object_robot_model_["model"].toString ();
    QJsonObject json_object_chassis=json_object_robot_model_["chassis"].toObject ();

    basic_widget()->model_base_data_.gyro_type_                    =json_object_chassis["gyro"].toInt();
    basic_widget()->model_base_data_.auto_gyro_cal_             =json_object_chassis["autoGyroCal"].toBool ();
    basic_widget()->model_base_data_.auto_brake_                 =json_object_chassis["autoBrake"].toBool ();
    basic_widget()->model_base_data_.battery_info_               =json_object_chassis["batteryInfo"].toInt ();
    int brake =  json_object_chassis["brake"].toInt();
    if(brake == 1)
    {
        basic_widget()->model_base_data_.brake_    = true;
    }
    else
    {
        basic_widget()->model_base_data_.brake_    = false;
    }
    int led = json_object_chassis["LED"].toInt();
    if(led==1)
    {
        basic_widget()->model_base_data_.LED_  = true;
    }
    else
    {
        basic_widget()->model_base_data_.LED_  = false;
    }
    RobotModelData robot_model_data;
    robot_model_data.shapType_ =(ShapeType)json_object_chassis["shape"].toInt();//获取底盘形状

    robot_model_data.mvStyle_ =(MoveStyle)json_object_chassis["mode"].toInt();//获取运动方式
    QJsonObject json_object_mode_param = json_object_chassis["mode_param"].toObject();
    robot_model_data.wheelRadius_                       = json_object_mode_param["wheelRadius"].toDouble()*100;
    robot_model_data.chasisType_                   =(ChassisType)json_object_mode_param["type"].toInt();//获取底盘类型
    basic_widget()->model_base_data_.encoder_line_ =json_object_mode_param["encoderLine"].toInt ();
    basic_widget()->model_base_data_.max_motor_rpm_=json_object_mode_param["maxMotorRpm"].toInt ();
    QJsonArray inverseArray    =json_object_mode_param["inverse"].toArray();
    QJsonArray driverIDArray = json_object_mode_param["driverID"].toArray();
    basic_widget()->model_base_data_.driverBrand_   = json_object_mode_param["driverBrand"].toString();
    basic_widget()->model_base_data_.reduction_ratio_=json_object_mode_param["reductionRatio"].toDouble ();
    if(json_object_robot_model_.contains("fork"))
    {
        basic_widget()->getForkWidget()->setIsConfigureFork(true);
        QJsonObject fork_obj = json_object_robot_model_["fork"].toObject();
        ForkData forkData;
        forkData.blockLaserDist = fork_obj["blockLaserDist"].toDouble();
        forkData.encoderSteps = fork_obj["encoderSteps"].toInt();
        forkData.lengthFactor = fork_obj["lengthFactor"].toDouble();
        forkData.lengthPerTurn = fork_obj["lengthPerTurn"].toDouble();
        forkData.maxHeight = fork_obj["maxHeight"].toDouble();
        forkData.minHeight = fork_obj["minHeight"].toDouble();
        forkData.downBuffer = fork_obj["downBuffer"].toDouble();
        forkData.upBuffer = fork_obj["upBuffer"].toDouble();
        forkData.pressureSensor = fork_obj["pressureSensor"].toInt();
        forkData.pump = fork_obj["pump"].toInt();
        forkData.type = fork_obj["type"].toInt();
        basic_widget()->getForkWidget()->setForkData(forkData);
    }
    else
    {
        basic_widget()->getForkWidget()->setIsConfigureFork(false);
    }

    if(robot_model_data.shapType_==RECTANGLE)
    {
        robot_model_data.width_ = json_object_chassis["width"].toDouble()*100;
        robot_model_data.head_ = json_object_chassis["head"].toDouble()*100;
        robot_model_data.tail_ = json_object_chassis["tail"].toDouble()*100;

        switch (robot_model_data.chasisType_) {
        case DIFFERENTIAL_TWOWHEEL://差动两轮
            robot_model_data.chasisModel.DiffTwoWheel.a_axis_distance=json_object_mode_param["A"].toDouble()*100;
            break;
        case DIFFERENTIAL_FOURWHEEL://差动四轮
            robot_model_data.chasisModel.DiffFourWheel.a_axis_distance= json_object_mode_param["A"].toDouble()*100;
            robot_model_data.chasisModel.DiffFourWheel.b_axis_distance= json_object_mode_param["B"].toDouble()*100;
            break;
        case ALLDIRECTION_THREEWHEEL://全向三轮
            robot_model_data.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance=json_object_mode_param["A"].toDouble()*100;
            robot_model_data.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle = json_object_mode_param["theta"].toDouble();
            break;
        case MECANUM_FOURWHEEL://四轮麦克纳姆
            robot_model_data.chasisModel.MecanumFourWheel.a_axis_distance=json_object_mode_param["A"].toDouble()*100;
            robot_model_data.chasisModel.MecanumFourWheel.b_axis_distance=json_object_mode_param["B"].toDouble()*100;
            break;
        case SINGLESTEERWHEEL_FRONTDRIVER_FRONT_CODERTYPE://前驱+前转动编码器
        case SINGLESTEERWHEEL_FRONTDRIVER_BACK_CODERTYPE://前驱+后转动编码器
        case SINGLESTEERWHEEL_BACKDRIVER_FRONT_CODERTYPE://后驱+前转动编码器
        case SINGLESTEERWHEEL_BACKDRIVER_BACK_CODERTYPE://后驱+后转动编码器
        {
            robot_model_data.chasisModel.SingleWheelDriver.a_axis_distance=json_object_mode_param["A"].toDouble()*100;
            robot_model_data.chasisModel.SingleWheelDriver.b_axis_distance=json_object_mode_param["B"].toDouble()*100;
            robot_model_data.chasisModel.SingleWheelDriver.absEncoderType=json_object_mode_param["absEncoder"].toInt();
            QJsonArray  steerArray =json_object_mode_param["steerAngle"].toArray();
            robot_model_data.chasisModel.SingleWheelDriver.steer_angle_min=steerArray.at(0).toDouble();
            robot_model_data.chasisModel.SingleWheelDriver.steer_angle_max=steerArray.at(1).toDouble();
            QJsonArray absEncoderRange = json_object_mode_param["absEncoderRange"].toArray();
            robot_model_data.chasisModel.SingleWheelDriver.absEncoderRange_min=absEncoderRange.at(0).toDouble();
            robot_model_data.chasisModel.SingleWheelDriver.absEncoderRange_max=absEncoderRange.at(1).toDouble();
        }
            break;
        default:
            break;
        }
        //--

    }
    else if(robot_model_data.shapType_==CIRCLE)
    {
        robot_model_data.radius_ = json_object_chassis["radius"].toDouble()*100;

        //--
        switch (robot_model_data.chasisType_) {
        case DIFFERENTIAL_TWOWHEEL://差动两轮
            robot_model_data.chasisModel.DiffTwoWheel.a_axis_distance=json_object_mode_param["A"].toDouble()*100;
            break;
        case DIFFERENTIAL_FOURWHEEL://差动四轮
            robot_model_data.chasisModel.DiffFourWheel.a_axis_distance= json_object_mode_param["A"].toDouble()*100;
            robot_model_data.chasisModel.DiffFourWheel.b_axis_distance= json_object_mode_param["B"].toDouble()*100;
            break;
        case ALLDIRECTION_THREEWHEEL://全向三轮
            robot_model_data.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance=json_object_mode_param["A"].toDouble()*100;
            robot_model_data.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle = json_object_mode_param["theta"].toDouble();
            break;
        default:
            break;
        }
        //--
    }

    model_scene_->parse_robot_data (robot_model_data);
    //    model_scene_->parse_json_polygon (json_object_robot_model_["decelerateArea"].toObject (),0);
    //    model_scene_->parse_json_polygon (json_object_robot_model_["stopArea"].toObject (),1);
    //    model_scene_->parse_json_polygon (json_object_robot_model_["safeArea"].toObject (),2);
    model_scene_->parse_json_laser (json_object_robot_model_["laser"].toObject ());
    model_scene_->parse_json_ultrasonic (json_object_robot_model_["ultrasonic"].toObject ());
    model_scene_->parse_json_di (json_object_robot_model_["DI"].toObject ());
    model_scene_->parse_json_do(json_object_robot_model_["DO"].toObject());
    model_scene_->parse_json_magnetic (json_object_robot_model_["magneticSensor"].toObject ());
    model_scene()->parse_json_RFID(json_object_robot_model_["RFID"].toObject());
    model_scene()->parse_json_Inverse(inverseArray);//获取正反转列表
    model_scene()->parse_json_driverID(driverIDArray);
    model_scene()->parse_json_Cameral(json_object_robot_model_["vision"].toObject());
    widget_basic_setting_->update_panel_infor ();
}

void ModelGraphicsView::hidePropertyWidget()
{
    //隐藏属性框
    widget_basic_setting_->hide();
    property_widget()->hide();
    btn_DI_->hide();
    btn_DO_->hide();
    btn_laser_->hide();
    btn_ultrasonic_->hide();
    btn_magneticScesor_->hide();
    btn_polygon_safe_->hide();
    btn_polygon_stop_->hide();
    btn_polygon_lowspeed_->hide();

    btn_scale_1x_->hide();
    btn_scale_2x_->hide();
    btn_scale_3x_->hide();

}

void ModelGraphicsView::showPropertyWidget()
{
    //隐藏属性框
    widget_basic_setting_->show();
    property_widget()->show();
    btn_DI_->show();
    btn_DO_->show();
    btn_laser_->show();
    btn_ultrasonic_->show();
    btn_magneticScesor_->show();
    btn_polygon_safe_->show();
    btn_polygon_stop_->show();
    btn_polygon_lowspeed_->show();

    btn_scale_1x_->show();
    btn_scale_2x_->show();
    btn_scale_3x_->show();

}

void ModelGraphicsView::zoomInOut(int scale)
{
    resetTransform();
    QMatrix view_matrix(1,0,0 ,-1,0,0);
    view_matrix.rotate (90);
    setMatrix(view_matrix,true);
    this->scale(scale,scale);
    centerOn(QPoint(0,0));
}

void ModelGraphicsView::slot_btn_scale_1x()
{
    zoomInOut(1);
}

void ModelGraphicsView::slot_btn_scale_2x()
{
    zoomInOut(2);

}

void ModelGraphicsView::slot_btn_scale_3x()
{
    //    if(m_undoStack == NULL)
    //        m_undoStack  = UndoStack::InitStack();
    //    m_new_scale = 3;
    //    zoomInOutCommand* cmd = new zoomInOutCommand(model_scene_,m_ori_scale,m_new_scale,NULL);
    //    m_undoStack->push(cmd);
    //    m_ori_scale = 3;

    zoomInOut(3);
}
