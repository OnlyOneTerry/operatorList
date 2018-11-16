#include<QTransform>
#include<QDebug>
#include "ItemRobotModel.h"
#include "ModelGraphicsView.h"
#include "WidgetRobotModelSetting.h"
#include"ModelCommon.h"
#include"ModelCustomCommand.h"
ItemRobotModel::ItemRobotModel(ModelGraphicsScene * scene,QJsonObject json):
    ItemBase(scene)
  ,moveModelStyle_(DIFFERENTIAL)
  ,chasisModelType_(DIFFERENTIAL_TWOWHEEL)

{
    setFlag (ItemIsMovable,false);
    setAcceptHoverEvents (true);
    setData(Type,1);

    brush_=( QBrush(QColor(9,142,227)));
    pen_=Qt::NoPen;
    //对矩形模型基本参数进行初始化
    robot_model_data_.width_= 0.56*100;
    robot_model_data_.head_ = 0.28*100;
    robot_model_data_.tail_ = 0.28*100;
    robot_model_data_.wheelRadius_ = 10;
    robot_model_data_.radius_= 0.28*100;
    robot_model_data_.chasisModel.DiffTwoWheel.a_axis_distance= 30;
    robot_model_data_.chasisType_=DIFFERENTIAL_TWOWHEEL;
    robot_model_data_.mvStyle_=DIFFERENTIAL;
    robot_model_data_.shapType_=RECTANGLE;
    robot_model_data_.driverID<<1<<2;
    robot_model_data_.inverse_<<false<<false;
    m_ori_head = robot_model_data_.head_;
    m_ori_tail = robot_model_data_.tail_;
    m_ori_width = robot_model_data_.width_;
    //============

    //对圆形模型基本参数进行初始化
    robot_circle_model_data_.radius_=0.28*100;
    robot_circle_model_data_.wheelRadius_ = 10;
    robot_circle_model_data_.chasisModel.DiffTwoWheel.a_axis_distance = 30;
    robot_circle_model_data_.chasisType_ = DIFFERENTIAL_TWOWHEEL;
    robot_circle_model_data_.mvStyle_  = DIFFERENTIAL;
    robot_circle_model_data_.shapType_ = CIRCLE;
    robot_circle_model_data_.driverID<<1<<2;
    robot_circle_model_data_.inverse_<<false<<false;
    m_ori_radius = robot_circle_model_data_.radius_;
}

ItemRobotModel::~ItemRobotModel()
{

}

void ItemRobotModel::update_model()
{
    update ();
    model_scene ()->update ();
}

void ItemRobotModel::setMoveStyle(MoveStyle newStyle)
{
    moveModelStyle_ = newStyle;
}

MoveStyle ItemRobotModel::getMoveStyle()
{
    return moveModelStyle_;
}

void ItemRobotModel::setChasiseType(ChassisType newType)
{
    chasisModelType_ = newType;
}

ChassisType ItemRobotModel::getChasisType()
{
    return chasisModelType_;
}

void ItemRobotModel::setChasisShapeType(ShapeType type)
{
    robot_model_data_.shapType_ = type;
    robot_circle_model_data_.shapType_ = type;
}

ShapeType ItemRobotModel::getChasisShapeType()
{
    return robot_model_data_.shapType_;
}

void ItemRobotModel::setMOriRadius(qreal r)
{
    m_ori_radius = r;
}

void ItemRobotModel::setMOriHead(qreal h)
{
    m_ori_head = h;
}

void ItemRobotModel::setMOriTail(qreal t)
{
    m_ori_tail = t;
}

void ItemRobotModel::setMOriWidth(qreal w)
{
    m_ori_width = w;
}

WidgetSetting *ItemRobotModel::getWidgetSetting()
{
    return widget_setting_;
}

QPainterPath ItemRobotModel::shape() const
{
    QPainterPath path;
    if(robot_model_data_.shapType_==RECTANGLE||robot_circle_model_data_.shapType_ == RECTANGLE)
    {
        path.addRect(-robot_model_data_.tail_-3,-robot_model_data_.width_/2.0-3,
                     robot_model_data_.head_+robot_model_data_.tail_+6,
                     robot_model_data_.width_+6);
    }
    else if(robot_model_data_.shapType_==CIRCLE||robot_circle_model_data_.shapType_ == CIRCLE)
    {
        path.addEllipse(-robot_circle_model_data_.radius_-3,-robot_circle_model_data_.radius_-3,
                        2*robot_circle_model_data_.radius_+6,2*robot_circle_model_data_.radius_+6);
    }
    return path;
}

QRectF ItemRobotModel::boundingRect() const
{
    QRectF rect;

    if(robot_model_data_.shapType_==RECTANGLE||robot_circle_model_data_.shapType_ == RECTANGLE)
    {
        rect = QRectF(-robot_model_data_.tail_,-robot_model_data_.width_/2.0,
                      robot_model_data_.head_+robot_model_data_.tail_,
                      robot_model_data_.width_).adjusted (-10,-10,10,10);
    }
    else if(robot_model_data_.shapType_==CIRCLE||robot_circle_model_data_.shapType_ == CIRCLE)
    {
        rect = QRectF(-robot_circle_model_data_.radius_,-robot_circle_model_data_.radius_,
                      2*robot_circle_model_data_.radius_,2*robot_circle_model_data_.radius_).adjusted (-10,-10,10,10);
    }
    return rect;
}

void ItemRobotModel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen (pen_);
    painter->setBrush (brush_);

    //外接矩形(底盘形状)
    if(robot_model_data_.shapType_ == RECTANGLE || robot_circle_model_data_.shapType_ ==RECTANGLE)
    {
        painter->drawRect(-robot_model_data_.tail_,-robot_model_data_.width_/2.0,
                          robot_model_data_.head_+robot_model_data_.tail_,
                          robot_model_data_.width_);
        paintRectangleModel(painter);
    }
    else if(robot_model_data_.shapType_ == CIRCLE || robot_circle_model_data_.shapType_ ==CIRCLE)
    {
        //外接圆形(底盘形状)
        painter->drawEllipse(-robot_circle_model_data_.radius_,-robot_circle_model_data_.radius_,
                             2*robot_circle_model_data_.radius_,2*robot_circle_model_data_.radius_);
        paintCircleModel(painter);
    }
}

void ItemRobotModel::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    ItemBase::hoverEnterEvent (event);
}

void ItemRobotModel::mousePressEvent (QGraphicsSceneMouseEvent *event)
{
    model_scene()->setCurrentPressItem(this);

    ItemBase::mousePressEvent (event);
    if(event->button()==Qt::LeftButton)
    {
        setNewPos(event->pos());
    }
}
void ItemRobotModel::mouseMoveEvent (QGraphicsSceneMouseEvent *event)
{
    ItemBase::mouseMoveEvent (event);
    QPointF mpos=event->scenePos ();

    if(event->buttons ()&Qt::LeftButton)
    {
        if(getChasisShapeType()==RECTANGLE)
        {
            //拖动底盘外形区域改变大小
            switch (adjust_type_) {
            case C_TL:
            {
                robot_model_data_.width_=qFabs (mpos.y ())*2;
                robot_model_data_.tail_=qFabs (mpos.x ());
            }break;
            case C_TR:
            {
                robot_model_data_.width_=qFabs (mpos.y ())*2;
                robot_model_data_.head_=qFabs (mpos.x ());
            }break;
            case C_BL:
            {
                robot_model_data_.width_=qFabs (mpos.y ())*2;
                robot_model_data_.tail_=qFabs (mpos.x ());
            }break;
            case C_BR:
            {
                robot_model_data_.width_=qFabs (mpos.y ())*2;
                robot_model_data_.head_=qFabs (mpos.x ());
            }break;
            case H_L:
            {
                robot_model_data_.tail_=qFabs (mpos.x());
            }break;
            case H_R:
            {
                robot_model_data_.head_=qFabs (mpos.x ());
            }break;
            case V_T:
            {
                robot_model_data_.width_=qFabs (mpos.y ())*2;
            }break;
            case V_B:
            {
                robot_model_data_.width_=qFabs (mpos.y ())*2;
            }break;
            default:
            {
            }break;
            }

        }
        else if(getChasisShapeType()==CIRCLE)
        {
            //拖动底盘外形区域改变大小
            switch (adjust_type_) {
            case C_TL:
            case C_TR:
            case C_BL:
            case C_BR:
                robot_circle_model_data_.radius_ = qSqrt(mpos.x()*mpos.x()+mpos.y()*mpos.y());
                break;
            case H_L:
            case H_R:
                robot_circle_model_data_.radius_ = qFabs(mpos.x())*1.2;
                break;
            case V_T:
            case V_B:
                robot_circle_model_data_.radius_ = qFabs(mpos.y())*1.2;
                break;
            default:
                break;
            }
        }
    }
    //更新属性框上的对应的属性值
    if(is_ppt_ready (widType::WIDGET_ROBOTMODEL))
    {
        widget_setting_->update_value ();
    }

    update ();
    scene ()->update ();
}

void ItemRobotModel::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

    WidgetRobotModelSetting* roboSetting =NULL;
    if(is_ppt_ready (widType::WIDGET_ROBOTMODEL)&&widget_setting_)
    {
        roboSetting = dynamic_cast<WidgetRobotModelSetting*>(widget_setting_);
    }

    if(m_undoStack==NULL)
    {
        return;
    }

    //判断当前底盘形状圆形还是矩形

    if(getChasisShapeType()==RECTANGLE)
    {
        if(adjust_type_<10&&adjust_type_>0)
        {

            qreal m_new_head  = robot_model_data_.head_;
            qreal m_new_tail  = robot_model_data_.tail_;
            qreal m_new_width = robot_model_data_.width_;

            DragChassisCommand* drag_command = new DragChassisCommand(this,m_ori_head,m_new_head,
                                                                      m_ori_tail,m_new_tail,
                                                                      m_ori_width,m_new_width,
                                                                      roboSetting->getHeadSpinBox(),
                                                                      roboSetting->getTailSpinBox(),
                                                                      roboSetting->getWidthSpinBox(),
                                                                      model_scene_,
                                                                      NULL);
            m_undoStack->push(drag_command);
            m_ori_head = robot_model_data_.head_;
            m_ori_tail = robot_model_data_.tail_;
            m_ori_width = robot_model_data_.width_;
        }

    }
    else if(getChasisShapeType()==CIRCLE)
    {
        if(adjust_type_<10&&adjust_type_>0)
        {
            qreal m_new_radius = robot_circle_model_data_.radius_;
            DragChassisCommand* drag_command = new DragChassisCommand(this,m_ori_radius,
                                                                      m_new_radius,
                                                                      roboSetting->getRadiusSpinbox(),
                                                                      model_scene_,NULL);
            m_undoStack->push(drag_command);
            m_ori_radius = robot_circle_model_data_.radius_;
        }
    }

    ItemBase::mouseReleaseEvent (event);
    adjust_type_=NONE;

    //拖放后属于选中状态
    pen_ = QPen(Qt::magenta);
    update();

}

void ItemRobotModel::add_setting_widget()
{
    if(widget_setting_==NULL)
    {
        widget_setting_= new WidgetRobotModelSetting(this);
    }
}

void ItemRobotModel::paintRectangleModel(QPainter *painter)
{
    switch (robot_model_data_.chasisType_)
    {
    case DIFFERENTIAL_TWOWHEEL://差动两轮
    {

        QPainterPath path;
        painter->setPen(Qt::yellow);
        //轴
        path.addRect(-1,-robot_model_data_.chasisModel.DiffTwoWheel.a_axis_distance/2
                     ,2,robot_model_data_.chasisModel.DiffTwoWheel.a_axis_distance);
        //右轮（从view中观察为右轮）在场景坐标系看是前轮
        path.addRect(-robot_model_data_.wheelRadius_,-robot_model_data_.chasisModel.DiffTwoWheel.a_axis_distance/2-3,
                     robot_model_data_.wheelRadius_*2,6);
        //左轮（从view中观察为左轮）在场景坐标系看是后轮
        path.addRect(-robot_model_data_.wheelRadius_,robot_model_data_.chasisModel.DiffTwoWheel.a_axis_distance/2-3,
                     robot_model_data_.wheelRadius_*2,6);
        painter->drawPath(path);
    }
        break;
    case DIFFERENTIAL_FOURWHEEL://差动四轮
    {
        //外接矩形
        QPainterPath path;
        painter->setPen(Qt::yellow);
        //传动轴
        path.addRect(-robot_model_data_.chasisModel.DiffFourWheel.b_axis_distance/2,-1
                     ,robot_model_data_.chasisModel.DiffFourWheel.b_axis_distance,2);
        //前轴（在view中观察为前轴在场景坐标系中是右轴）
        path.addRect(robot_model_data_.chasisModel.DiffFourWheel.b_axis_distance/2-1
                     ,-robot_model_data_.chasisModel.DiffFourWheel.a_axis_distance/2
                     ,2,robot_model_data_.chasisModel.DiffFourWheel.a_axis_distance);
        //后轴（在view中观察为前轴在场景坐标系中是左轴）
        path.addRect(-robot_model_data_.chasisModel.DiffFourWheel.b_axis_distance/2-1
                     ,-robot_model_data_.chasisModel.DiffFourWheel.a_axis_distance/2
                     ,2,robot_model_data_.chasisModel.DiffFourWheel.a_axis_distance);
        //左前轮（在view中观察为前轴在场景坐标系中是右后轮）
        path.addRect(-robot_model_data_.chasisModel.DiffFourWheel.b_axis_distance/2-robot_model_data_.wheelRadius_
                     ,-robot_model_data_.chasisModel.DiffFourWheel.a_axis_distance/2-3
                     ,robot_model_data_.wheelRadius_*2,6);
        //右前轮（在view中观察为前轴在场景坐标系中是右前轮）
        path.addRect(robot_model_data_.chasisModel.DiffFourWheel.b_axis_distance/2-robot_model_data_.wheelRadius_
                     ,-robot_model_data_.chasisModel.DiffFourWheel.a_axis_distance/2-3
                     ,robot_model_data_.wheelRadius_*2,6);
        //左后轮（在view中观察为前轴在场景坐标系中是左后轮）
        path.addRect(-robot_model_data_.chasisModel.DiffFourWheel.b_axis_distance/2-robot_model_data_.wheelRadius_
                     ,robot_model_data_.chasisModel.DiffFourWheel.a_axis_distance/2-3
                     ,robot_model_data_.wheelRadius_*2,6);
        //右后轮（在view中观察为前轴在场景坐标系中是左前轮）
        path.addRect(robot_model_data_.chasisModel.DiffFourWheel.b_axis_distance/2-robot_model_data_.wheelRadius_
                     ,robot_model_data_.chasisModel.DiffFourWheel.a_axis_distance/2-3
                     ,robot_model_data_.wheelRadius_*2,6);
        painter->drawPath(path);
    }
        break;
    case ALLDIRECTION_THREEWHEEL://全向三轮
    {
        qreal delta_Angle = robot_model_data_.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle;
        qreal distance = robot_model_data_.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance;
        qreal r = robot_model_data_.wheelRadius_;
        //外接矩形

        painter->setPen(Qt::yellow);
        painter->drawLine(0,0,distance,0);//正方向线用黄色标记
        painter->rotate(delta_Angle);
        painter->drawRect(distance-3,-r,8,r*2);
        painter->drawRect(-0.5,-0.5,distance,1);
        painter->rotate(120);

        painter->drawRect(distance-3,-r,8,r*2);
        painter->drawRect(-0.5,-0.5,distance,1);
        painter->rotate(120);

        painter->drawRect(distance-3,-r,8,r*2);
        painter->drawRect(-0.5,-0.5,distance,1);

        painter->rotate(120);
        painter->rotate(-delta_Angle);

        QRectF rect_top(-10, -10, 20, 20);//角所在的矩形区域
        // 扇形起始角度
        int startAngle = 0 * 16;
        // 扇形覆盖范围（120度的）
        int spanAngle = delta_Angle * 16;

        painter->scale(1,-1);//沿x轴翻转
        painter->setBrush(Qt::blue);
        painter->drawPie(rect_top, startAngle, spanAngle);

    }
        break;
    case MECANUM_FOURWHEEL://四轮麦克纳姆
    {
        //外接矩形
        QPainterPath path;
        painter->setPen(Qt::yellow);
        //传动轴
        path.addRect(-robot_model_data_.chasisModel.MecanumFourWheel.b_axis_distance/2,-1
                     ,robot_model_data_.chasisModel.MecanumFourWheel.b_axis_distance,2);
        //前轴（在view中观察为前轴在场景坐标系中是右轴）
        path.addRect(robot_model_data_.chasisModel.MecanumFourWheel.b_axis_distance/2-1
                     ,-robot_model_data_.chasisModel.MecanumFourWheel.a_axis_distance/2
                     ,2,robot_model_data_.chasisModel.MecanumFourWheel.a_axis_distance);
        //后轴（在view中观察为前轴在场景坐标系中是左轴）
        path.addRect(-robot_model_data_.chasisModel.MecanumFourWheel.b_axis_distance/2-1
                     ,-robot_model_data_.chasisModel.MecanumFourWheel.a_axis_distance/2
                     ,2,robot_model_data_.chasisModel.MecanumFourWheel.a_axis_distance);
        //左前轮（在view中观察为前轴在场景坐标系中是右后轮）
        path.addRect(-robot_model_data_.chasisModel.MecanumFourWheel.b_axis_distance/2-robot_model_data_.wheelRadius_
                     ,-robot_model_data_.chasisModel.MecanumFourWheel.a_axis_distance/2-3
                     ,robot_model_data_.wheelRadius_*2,6);
        //右前轮（在view中观察为前轴在场景坐标系中是右前轮）
        path.addRect(robot_model_data_.chasisModel.MecanumFourWheel.b_axis_distance/2-robot_model_data_.wheelRadius_
                     ,-robot_model_data_.chasisModel.MecanumFourWheel.a_axis_distance/2-3
                     ,robot_model_data_.wheelRadius_*2,6);
        //左后轮（在view中观察为前轴在场景坐标系中是左后轮）
        path.addRect(-robot_model_data_.chasisModel.MecanumFourWheel.b_axis_distance/2-robot_model_data_.wheelRadius_
                     ,robot_model_data_.chasisModel.MecanumFourWheel.a_axis_distance/2-3
                     ,robot_model_data_.wheelRadius_*2,6);
        //右后轮（在view中观察为前轴在场景坐标系中是左前轮）
        path.addRect(robot_model_data_.chasisModel.MecanumFourWheel.b_axis_distance/2-robot_model_data_.wheelRadius_
                     ,robot_model_data_.chasisModel.MecanumFourWheel.a_axis_distance/2-3
                     ,robot_model_data_.wheelRadius_*2,6);
        painter->drawPath(path);

    }
        break;
    case SINGLESTEERWHEEL_FRONTDRIVER_FRONT_CODERTYPE://前驱+前转动编码器
    case SINGLESTEERWHEEL_FRONTDRIVER_BACK_CODERTYPE://前驱+后转动编码器
    case SINGLESTEERWHEEL_BACKDRIVER_FRONT_CODERTYPE://后驱+前转动编码器
    case SINGLESTEERWHEEL_BACKDRIVER_BACK_CODERTYPE://后驱+后转动编码器
    {
        //外接矩形
        painter->drawRect(-robot_model_data_.tail_,-robot_model_data_.width_/2.0,
                          robot_model_data_.head_+robot_model_data_.tail_,
                          robot_model_data_.width_);
        painter->setPen(Qt::yellow);
        //左轮（在view中观察为左轮）
        painter->drawRect(-robot_model_data_.wheelRadius_
                          ,robot_model_data_.chasisModel.SingleWheelDriver.a_axis_distance/2-3
                          ,robot_model_data_.wheelRadius_*2,6);
        //右轮
        painter->drawRect(-robot_model_data_.wheelRadius_
                          ,-robot_model_data_.chasisModel.SingleWheelDriver.a_axis_distance/2-3
                          ,robot_model_data_.wheelRadius_*2,6);
        //纵轴
        painter->drawRect(0,-1,robot_model_data_.chasisModel.SingleWheelDriver.b_axis_distance,2);
        //横轴
        painter->drawRect(-1,-robot_model_data_.chasisModel.SingleWheelDriver.a_axis_distance/2
                          ,2,robot_model_data_.chasisModel.SingleWheelDriver.a_axis_distance);

        //        //前轮
        painter->translate(robot_model_data_.chasisModel.SingleWheelDriver.b_axis_distance,0);//painter 默认在item中心，先移动到目标中心旋转后再移到原点
        painter->rotate(30);
        QRectF rect(-robot_model_data_.wheelRadius_,-3,robot_model_data_.wheelRadius_*2,6);
        painter->drawRect(rect);
    }
        break;
    default:

        break;
    }

}

void ItemRobotModel::paintCircleModel(QPainter *painter)
{
    switch (robot_circle_model_data_.chasisType_)
    {
    case DIFFERENTIAL_TWOWHEEL://差动两轮
    {
        QPainterPath path;

        painter->setPen(Qt::yellow);
        //轴
        path.addRect(-1,-robot_circle_model_data_.chasisModel.DiffTwoWheel.a_axis_distance/2
                     ,2,robot_circle_model_data_.chasisModel.DiffTwoWheel.a_axis_distance);
        //右轮（从view中观察为右轮）在场景坐标系看是前轮
        path.addRect(-robot_circle_model_data_.wheelRadius_,-robot_circle_model_data_.chasisModel.DiffTwoWheel.a_axis_distance/2-3,
                     robot_circle_model_data_.wheelRadius_*2,6);
        //左轮（从view中观察为左轮）在场景坐标系看是后轮
        path.addRect(-robot_circle_model_data_.wheelRadius_,robot_circle_model_data_.chasisModel.DiffTwoWheel.a_axis_distance/2-3,
                     robot_circle_model_data_.wheelRadius_*2,6);
        painter->drawPath(path);
    }
        break;
    case DIFFERENTIAL_FOURWHEEL://差动四轮
    {

        QPainterPath path;

        painter->setPen(Qt::yellow);
        //传动轴
        path.addRect(-robot_circle_model_data_.chasisModel.DiffFourWheel.b_axis_distance/2,-1
                     ,robot_circle_model_data_.chasisModel.DiffFourWheel.b_axis_distance,2);
        //前轴（在view中观察为前轴在场景坐标系中是右轴）
        path.addRect(robot_circle_model_data_.chasisModel.DiffFourWheel.b_axis_distance/2-1
                     ,-robot_circle_model_data_.chasisModel.DiffFourWheel.a_axis_distance/2
                     ,2,robot_circle_model_data_.chasisModel.DiffFourWheel.a_axis_distance);
        //后轴（在view中观察为前轴在场景坐标系中是左轴）
        path.addRect(-robot_circle_model_data_.chasisModel.DiffFourWheel.b_axis_distance/2-1
                     ,-robot_circle_model_data_.chasisModel.DiffFourWheel.a_axis_distance/2
                     ,2,robot_circle_model_data_.chasisModel.DiffFourWheel.a_axis_distance);
        //左前轮（在view中观察为前轴在场景坐标系中是右后轮）
        path.addRect(-robot_circle_model_data_.chasisModel.DiffFourWheel.b_axis_distance/2-robot_circle_model_data_.wheelRadius_
                     ,-robot_circle_model_data_.chasisModel.DiffFourWheel.a_axis_distance/2-3
                     ,robot_circle_model_data_.wheelRadius_*2,6);
        //右前轮（在view中观察为前轴在场景坐标系中是右前轮）
        path.addRect(robot_circle_model_data_.chasisModel.DiffFourWheel.b_axis_distance/2-robot_circle_model_data_.wheelRadius_
                     ,-robot_circle_model_data_.chasisModel.DiffFourWheel.a_axis_distance/2-3
                     ,robot_circle_model_data_.wheelRadius_*2,6);
        //左后轮（在view中观察为前轴在场景坐标系中是左后轮）
        path.addRect(-robot_circle_model_data_.chasisModel.DiffFourWheel.b_axis_distance/2-robot_circle_model_data_.wheelRadius_
                     ,robot_circle_model_data_.chasisModel.DiffFourWheel.a_axis_distance/2-3
                     ,robot_circle_model_data_.wheelRadius_*2,6);
        //右后轮（在view中观察为前轴在场景坐标系中是左前轮）
        path.addRect(robot_circle_model_data_.chasisModel.DiffFourWheel.b_axis_distance/2-robot_circle_model_data_.wheelRadius_
                     ,robot_circle_model_data_.chasisModel.DiffFourWheel.a_axis_distance/2-3
                     ,robot_circle_model_data_.wheelRadius_*2,6);
        painter->drawPath(path);

    }
        break;
    case ALLDIRECTION_THREEWHEEL://全向三轮
    {

        qreal delta_Angle = robot_circle_model_data_.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle;

        qreal distance = robot_circle_model_data_.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance;
        qreal r = robot_circle_model_data_.wheelRadius_;

        painter->setPen(Qt::yellow);
        painter->drawLine(0,0,distance,0);//正方向线用黄色标记

        painter->rotate(delta_Angle);

        painter->drawRect(distance-3,-r,8,r*2);
        painter->drawRect(-0.5,-0.5,distance,1);
        painter->rotate(120);

        painter->drawRect(distance-3,-r,8,r*2);
        painter->drawRect(-0.5,-0.5,distance,1);

        painter->rotate(120);

        painter->drawRect(distance-3,-r,8,r*2);
        painter->drawRect(-0.5,-0.5,distance,1);

        painter->rotate(120);
        painter->rotate(-delta_Angle);

        QRectF rect_top(-10, -10, 20, 20);//角所在的矩形区域
        // 扇形起始角度
        int startAngle = 0 * 16;
        // 扇形覆盖范围（120度的）
        int spanAngle = delta_Angle * 16;

        painter->scale(1,-1);//沿x轴翻转
        painter->setBrush(Qt::blue);
        painter->drawPie(rect_top, startAngle, spanAngle);

    }
        break;
    default:
        break;
    }

}

void ItemRobotModel::position_changed()
{

}

//切换底盘时恢复初始状态
void ItemRobotModel::slot_recover_polygons()
{
    robot_model_data_.width_= 0.56*100;
    robot_model_data_.head_ = 0.28*100;
    robot_model_data_.tail_ = 0.28*100;
    robot_model_data_.wheelRadius_=0.10*100;
    model_scene()->update();
}

void ItemRobotModel::hoverMoveEvent (QGraphicsSceneHoverEvent *event)
{
    // 底盘轮廓响应区域
    ItemBase::hoverMoveEvent (event);
    QPointF mpos=event->scenePos ();

    QRectF rect_L;
    QRectF rect_R;
    QRectF rect_T;
    QRectF rect_B;
    QRectF rect_TL;
    QRectF rect_TR;
    QRectF rect_BL;
    QRectF rect_BR;

    if(getChasisShapeType()==RECTANGLE)//矩形轮廓响应区域
    {
        rect_L=QRectF(-robot_model_data_.tail_-3,-robot_model_data_.width_/2.0,6,robot_model_data_.width_);
        rect_R=QRectF(robot_model_data_.head_-3,-robot_model_data_.width_/2.0,6,robot_model_data_.width_);
        rect_T=QRectF(-robot_model_data_.tail_-3,-robot_model_data_.width_/2.0-3,robot_model_data_.tail_+robot_model_data_.head_,6);
        rect_B=QRectF(-robot_model_data_.tail_-3,robot_model_data_.width_/2.0-2,robot_model_data_.tail_+robot_model_data_.head_,6);

        rect_TL=QRectF(-robot_model_data_.tail_-3,-robot_model_data_.width_/2.0-3,6,6);
        rect_TR=QRectF(robot_model_data_.head_-3,-robot_model_data_.width_/2.0-3,6,6);
        rect_BL=QRectF(-robot_model_data_.tail_-3,robot_model_data_.width_/2.0-3,6,6);
        rect_BR=QRectF(robot_model_data_.head_-3,robot_model_data_.width_/2.0-3,6,6);

    }
    else if(getChasisShapeType()==CIRCLE)
    {
        rect_L=QRectF(-robot_circle_model_data_.radius_-3,-robot_circle_model_data_.radius_/2,6,robot_circle_model_data_.radius_);
        rect_R=QRectF(robot_circle_model_data_.radius_-3,-robot_circle_model_data_.radius_/2,6,robot_circle_model_data_.radius_);
        rect_T=QRectF(-robot_circle_model_data_.radius_/2,-robot_circle_model_data_.radius_-3,robot_circle_model_data_.radius_,6);
        rect_B=QRectF(-robot_circle_model_data_.radius_/2,robot_circle_model_data_.radius_-3,robot_circle_model_data_.radius_,6);

        rect_TL=QRectF(-robot_circle_model_data_.radius_,-robot_circle_model_data_.radius_,robot_circle_model_data_.radius_/2,robot_circle_model_data_.radius_/2);
        rect_TR=QRectF(robot_circle_model_data_.radius_/2,-robot_circle_model_data_.radius_,robot_circle_model_data_.radius_/2,robot_circle_model_data_.radius_/2);
        rect_BL=QRectF(-robot_circle_model_data_.radius_,robot_circle_model_data_.radius_/2,robot_circle_model_data_.radius_/2,robot_circle_model_data_.radius_/2);
        rect_BR=QRectF(robot_circle_model_data_.radius_/2,robot_circle_model_data_.radius_/2,robot_circle_model_data_.radius_/2,robot_circle_model_data_.radius_/2);
    }

    QCursor cursor;
    if(rect_TL.contains (mpos))
    {
        adjust_type_=C_TL;
        cursor=Qt::SizeFDiagCursor;
    }
    else if(rect_TR.contains (mpos))
    {
        adjust_type_=C_TR;
        cursor=Qt::SizeBDiagCursor;
    }
    else if(rect_BL.contains (mpos))
    {
        adjust_type_=C_BL;
        cursor=Qt::SizeBDiagCursor;
    }
    else if(rect_BR.contains (mpos))
    {
        adjust_type_=C_BR;
        cursor=Qt::SizeFDiagCursor;
    }
    else if(rect_L.contains (mpos))
    {
        adjust_type_=H_L;
        cursor=Qt::SizeVerCursor;
    }
    else if(rect_R .contains (mpos))
    {
        adjust_type_=H_R;
        cursor=Qt::SizeVerCursor;
    }
    else if(rect_T.contains (mpos))
    {
        adjust_type_=V_T;
        cursor=Qt::SizeHorCursor;
    }
    else if(rect_B.contains (mpos))
    {
        adjust_type_=V_B;
        cursor=Qt::SizeHorCursor;
    }
    else
    {
        adjust_type_ = NONE;
        cursor=Qt::ArrowCursor;
    }
    setCursor (cursor);

}
void ItemRobotModel::hoverLeaveEvent (QGraphicsSceneHoverEvent *event)
{
    ItemBase::hoverLeaveEvent (event);
}
