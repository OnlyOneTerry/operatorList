#ifndef COMMON_H
#define COMMON_H

#include<QVariantMap>
#include<QList>
#include<QDebug>

#define  CONFIGFILE_PATH "./appInfo/model/model.cfg"
#define  COMPAREMODEL_PATH "./appInfo/model/Compare.cmp"
#define  DEFAULTCONFIGMODEL_PATH "./appInfo/model/template/"


//引导窗口类型
typedef enum GuidType_
{
    GUID_BASIC,
    GUID_SIMPLEDEVICE,
    GUID_OPTIONALDEVICE,
    GUID_MODEL_NAME,
    GUID_CHASSIS_SHAPE,
    GUID_CHASSIS_TYPE,
    GUID_CHASSIS,
    GUID_DI,
    GUID_DO,
    GUID_LASER,
    GUID_MAGENATICSENSOR,
    GUID_RFIDSENSOR,
    GUID_ULTRASONIC,
    GUID_FINISHED

}GuidType;

enum widType //属性窗口类型
{
    WIDGET_BASIC,
    WIDGET_DO,
    WIDGET_DI,
    WIDGET_LASER,
    WIDGET_ROBOTMODEL,
    WIDGET_MAGENETIC,
    WIDGET_ULTRASIONIC,
    WIDGET_RFID,
    WIDGET_CRASHDI,
    WIDGET_CRASHDIPOS,
    WIDGET_VISION,
    WIDGET_INVALID

};
//底盘外轮廓形状
typedef  enum ShapeType_
{
    RECTANGLE = 1,
    CIRCLE
}ShapeType;

//运动类型
typedef  enum MoveStyle_
{
    DIFFERENTIAL=1,//差动
    ALLDIRECTION,//全向
    MECANUM,//马克纳姆
    SINGLESTEERWHEEL//单舵轮
}MoveStyle;


//底盘类型
typedef enum ChassisType_
{
    DIFFERENTIAL_TWOWHEEL=10,//差动两轮
    DIFFERENTIAL_FOURWHEEL,//差动四轮
    ALLDIRECTION_THREEWHEEL=20,//全向三轮
    MECANUM_FOURWHEEL=30,//四轮麦克纳姆

    SINGLESTEERWHEEL_FRONTDRIVER_FRONT_CODERTYPE=40,//前驱+前转动编码器
    SINGLESTEERWHEEL_FRONTDRIVER_BACK_CODERTYPE,//前驱+后转动编码器
    SINGLESTEERWHEEL_BACKDRIVER_FRONT_CODERTYPE,//后驱+前转动编码器
    SINGLESTEERWHEEL_BACKDRIVER_BACK_CODERTYPE,//后驱+后转动编码器
}ChassisType;

typedef struct RobotModelData_
{
    ShapeType shapType_;//底盘形状
    MoveStyle mvStyle_;//运动类型
    ChassisType chasisType_;//底盘类型
    qreal width_;   // 底盘外接矩形宽, cm [0, 400]
    qreal head_;    // 底盘外接矩形头, cm [0, 200]
    qreal tail_;    //底盘外接矩形尾, cm [0, 200]
    qreal radius_; //轮廓半径
    qreal wheelRadius_;//轮半径, cm[0,200]
    QList<bool> inverse_;                       // 前后左右反转
    QList<int> driverID;
    union
    {
        //    两轮差动
        struct
        {
            qreal  a_axis_distance;//a轴距  左右轮距
        }DiffTwoWheel;

        //    四轮差动
        struct
        {
            qreal a_axis_distance;//a轴距  左右轮距
            qreal b_axis_distance;//b轴距
        }DiffFourWheel;

        //    三轮全向
        struct
        {
            qreal   wheel_corePoint_distance;//轮到中心距离(A轴距)
            qreal   firstWheel_delta_angle;//0号车轮与正方向夹角
        }AllDirectThreeWheel;

        //    四轮麦克纳姆
        struct
        {
            qreal a_axis_distance;//a轴距 左右轮距
            qreal b_axis_distance;//b轴距
        }MecanumFourWheel;

        //   单舵轮
        struct
        {
            qint32  steer_angle_min;//舵角范围
            qint32  steer_angle_max;
            qint32  absEncoderRange_min;// 绝对值编码器范围
            qint32  absEncoderRange_max;// 绝对值编码器范围
            qreal  a_axis_distance;//a轴距
            qreal  b_axis_distance;//b轴距
            qint32    absEncoderType;//1,2,3
        }SingleWheelDriver;

    }chasisModel;
}RobotModelData;


typedef struct  itemdata_
{
    int index;
    QString describtion;
}ItemData;

//配置文件结构类型
typedef struct  configure_data_
{
    int defaultidx;
    QList<ItemData> dataList;
    configure_data_ operator=(const configure_data_ da)
    {
        defaultidx = da.defaultidx;
        dataList = da.dataList;
        return *this;
    }
}ConfigureData;


//基本数据类型
struct ModelBaseData
{
    QString model_name_;        // 模型名
    qreal reduction_ratio_;          // 减速比(浮点数) [1, 50]
    quint32 encoder_line_;          // 编码器线数(整数) [0, 5000]
    quint32 max_motor_rpm_;    // 电机最大转速(整数) [0, 10000]
    QList<bool> inverse_;                       // 前后左右反转
    quint8 driver_;                      // 驱动器类型, 1 = Copley[232], 2 = SeerDriverBoard, 3 = Copley[CanOpen], 4 = SeerDriverBoard(Hall)
    QList<int> driverID;
    //    quint32 driverBrand_;//驱动器品牌
    QString driverBrand_;//驱动器品牌
    quint8 gyro_type_;                 // 陀螺仪
    bool auto_gyro_cal_;              // 陀螺仪是否自动标零
    bool brake_;                          // 抱闸是否存在
    bool auto_brake_;                  // 是否自动抱闸
    quint8 battery_info_;              // 电池检测类型, 0 = 没有, 1 = Pylon, 2 = modbus
    bool LED_;                             // LED 彩灯是否存在
    quint8 RFID_;                         // RFID 传感器类型, 0 = 没有, 1 =
    quint8 magnetic_sensor_;       // 磁传感器类型, 0 = 没有, 1 =
    quint8 camera_;                        // 摄像头类型, 0 = 没有
    quint8 max_di_num_;             // 最大 DI 的数量 [0, 16]
    quint8 max_do_num_;            // 最大 DO 的数量 [0, 16]
    quint8 ultrasonic_;                 // 超声接口类型, 0 = 没有, 1 = SeerUltrasonic, 2 = KS136
    quint8 max_ultrasonic_num_; // 最大超声数量 [0,24]

    ModelBaseData():
        RFID_(0),
        magnetic_sensor_(0),
        camera_(0),
        max_di_num_(16),
        max_do_num_(16),
        ultrasonic_(0),
        max_ultrasonic_num_(24),
        model_name_("AMB"),
        reduction_ratio_(20),
        encoder_line_(2500),
        max_motor_rpm_(3000),
        driver_(20),
        gyro_type_(1),
        auto_gyro_cal_(true),
        brake_(false),
        auto_brake_(false),
        battery_info_(1),
        LED_(false)
    {}
};


struct crashDiData
{
    quint8 id_;
    quint8 func_;
    quint8 type_;
    bool inverse_;
    QList<QPointF> posList; //碰撞条上的点

    crashDiData()
        :id_(0),
          func_(0),
          type_(3),
          inverse_(false)
    {}

    crashDiData(const crashDiData& data_)
    {
        id_ = data_.id_;
        func_ = data_.func_;
        type_ = data_.type_;
        inverse_ = data_.inverse_;
        posList = data_.posList;
    }
};

struct DiData
{
    quint8 id_;
    quint8 func_;
    quint8 type_;
    qreal x_;
    qreal y_;
    qreal z_;
    qreal r_;
    qreal rang_;
    bool inverse_;
    qreal max_speed_;
    qreal minDist_;
    qreal maxDist_;
    DiData():
        id_(0),
        func_(2),
        type_(0),
        x_(0),
        y_(0),
        z_(0),
        r_(0),
        rang_(60),
        max_speed_(0.),
        minDist_(0.1),
        maxDist_(0.5),
        inverse_(false){}

    DiData(const DiData& data_)
    {
        id_ = data_.id_;
        func_ = data_.func_;
        type_ = data_.type_;
        x_ = data_.x_;
        y_ = data_.y_;
        z_ = data_.z_;
        r_ = data_.r_;
        minDist_ = data_.minDist_;
        maxDist_ = data_.maxDist_;
        rang_ = data_.rang_;
        max_speed_ = data_.max_speed_;
    }

    DiData& operator=(const DiData& data_)
    {
        this->id_ = data_.id_;
        this->func_ = data_.func_;
        this->type_ = data_.type_;
        this->x_ = data_.x_;
        this->y_ = data_.y_;
        this->z_ = data_.z_;
        this->r_ = data_.r_;
        this->minDist_ = data_.minDist_;
        this->maxDist_ = data_.maxDist_;
        this->rang_ = data_.rang_;
        this->max_speed_ = data_.max_speed_;
        return *this;
    }
};

struct DOData
{
    quint8 id_;
    quint8 func_;
    quint8 type_;
    qreal x_;
    qreal y_;
    qreal z_;
    qreal r_;
    bool inverse_;
    DOData():
        id_(0),
        func_(2),
        type_(3),
        r_(0),
        x_(0),
        y_(0),
        z_(0),
        inverse_(false){}

    DOData(const DOData& data)
    {
        id_ = data.id_;
        func_ = data.func_;
        type_ = data.type_;
        x_ = data.x_;
        y_ = data.y_;
        z_ = data.z_;
        r_ = data.r_;
        inverse_ = data.inverse_;
    }

    DOData& operator =(const DOData& data)
    {
        this->id_ = data.id_;
        this->func_ = data.func_;
        this->type_ = data.type_;
        this->x_ = data.x_;
        this->y_ = data.y_;
        this->z_ = data.z_;
        this->r_ = data.r_;
        this->inverse_ = data.inverse_;
        return *this;
    }
};

struct LaserData
{
    qint8 id_;
    QString model_;                  // 激光型号, SickLMS or HokuyoURG
    qreal x_;
    qreal y_;
    qreal z_;
    qreal r_;
    //    qreal aperture_;                  // 激光扫描范围角(浮点), 度 [0, 360]
    //    qreal real_step_;                  // 相邻激光束角度间隔, 度 [0, 5]
    qreal step_;                         // 实际使用的相邻激光束角度间隔, 度 [0, 5]
    /*qreal resolution_scale_;       // 分辨率(浮点) [1, 2]
    qreal range_;  */                     // 激光有效距离(浮点), m [0, 100]
    qreal min_angle_;                // 使用的最小角度值(浮点), 度 [-180, 0]
    qreal max_angle_;               // 使用的最大角度值(浮点), 度 [0, 180]
    bool upside_;                      // 激光是否正装
    QString ip_;
    quint16 port_;
    bool use_for_localization_;  // 是否用来做定位
    LaserData():
        id_(0),
        model_("SickLMS"),
        r_(0),
        x_(0),
        y_(0),
        z_(0),
        step_(0.5),
        min_angle_(-90),
        max_angle_(90),
        upside_(true),
        ip_("192.168.192.100"),
        port_(2111),
        use_for_localization_(false)
    {}

    LaserData(const LaserData& data_)
    {
        model_ = data_.model_;
        x_ = data_.x_;
        y_ = data_.y_;
        z_ = data_.z_;
        r_ = data_.r_;
        step_ = data_.step_;
        min_angle_ = data_.max_angle_;
        max_angle_ = data_.max_angle_;
        upside_ = data_.upside_;
        ip_ = data_.ip_;
        port_ = data_.port_;
        use_for_localization_ = data_.use_for_localization_;
    }

    LaserData& operator =(const LaserData& data_)
    {
        this->model_ = data_.model_;
        this->x_ = data_.x_;
        this->y_ = data_.y_;
        this->z_ = data_.z_;
        this->r_ = data_.r_;
        this->step_ = data_.step_;
        this->min_angle_ = data_.max_angle_;
        this->max_angle_ = data_.max_angle_;
        this->upside_ = data_.upside_;
        this->ip_ = data_.ip_;
        this->port_ = data_.port_;
        this->use_for_localization_ = data_.use_for_localization_;

        return *this;
    }
};

struct ForkData
{
    qreal blockLaserDist;
    int encoderSteps;
    qreal lengthFactor;
    qreal lengthPerTurn;
    qreal maxHeight;
    qreal minHeight;
    int pressureSensor;
    int pump;
    int type;
    qreal downBuffer;
    qreal upBuffer;

    ForkData():
        blockLaserDist(1.6),
        encoderSteps(16384),
        lengthFactor(1.0),
        lengthPerTurn(0.23),
        maxHeight(1.5),
        minHeight(0.0),
        pressureSensor(0),
        pump(1),
        type(2),
        downBuffer(0.01),
        upBuffer(0.01)
    {}

    ForkData(const ForkData& data)
    {
        this->blockLaserDist = data.blockLaserDist;
        this->downBuffer = data.downBuffer;
        this->upBuffer = data.upBuffer;
        this->encoderSteps = data.encoderSteps;
        this->lengthFactor = data.lengthFactor;
        this->lengthPerTurn = data.lengthPerTurn;
        this->maxHeight = data.maxHeight;
        this->minHeight = data.minHeight;
        this->pressureSensor = data.pressureSensor;
        this->pump = data.pump;
        this->type = data.type;
        this->downBuffer = data.downBuffer;
        this->upBuffer = data.upBuffer;
    }

};


struct RFIDSensorData
{
    qreal x_;
    qreal y_;
    qreal z_;
    RFIDSensorData():
        x_(0),
        y_(0),
        z_(0)
    {}

    RFIDSensorData(const RFIDSensorData& data_)
    {
        x_ = data_.x_;
        y_ = data_.y_;
        z_ = data_.z_;
    }

    RFIDSensorData& operator =(const RFIDSensorData& data_)
    {
        this->x_ = data_.x_;
        this->y_ = data_.y_;
        this->z_ = data_.z_;

        return *this;
    }
};

struct MagneticSensorData
{
    quint8 id_;              // 磁导的ID号,
    qreal x_;
    qreal y_;
    qreal z_;
    qreal r_;
    qreal resolution_;   // 感应点数目
    qreal step_;        // 感应点间距, m
    MagneticSensorData():
        id_(0),
        r_(0),
        x_(0),
        y_(0),
        z_(0),
        resolution_(8),
        step_(0.01)
    {}

    MagneticSensorData(const MagneticSensorData& data_)
    {
        x_ = data_.x_;
        y_ = data_.y_;
        z_ = data_.z_;
        r_ = data_.r_;
        step_ = data_.step_;
        resolution_ = data_.resolution_;
    }

    MagneticSensorData& operator =(const MagneticSensorData& data_)
    {
        this->x_ = data_.x_;
        this->y_ = data_.y_;
        this->z_ = data_.z_;
        this->r_ = data_.r_;
        this->step_ = data_.step_;
        this->resolution_ = data_.resolution_;

        return *this;
    }
};


struct PolygonData
{
    qint8 id_;
    qreal max_speed_;
    quint8 working_mode_;
    QList<QPointF> vertex_list_;      //（m）
    quint8 vertex_num(){return vertex_list_.size ();}
    PolygonData():
        id_(0),
        max_speed_(0.3),
        working_mode_(3)
    {}

    PolygonData(const PolygonData& data_)
    {
        max_speed_ = data_.max_speed_;
        working_mode_ = data_.working_mode_;
        vertex_list_ = data_.vertex_list_;
    }
};

struct UltrasonicData
{
    quint8 id_;          // 位于超声接口的 id 号
    quint8 func_;      // 0 = 没有功能, 1 = 减速, 2 = 停止
    qreal x_;
    qreal y_;
    qreal z_;
    qreal r_;
    qreal min_dist_;  // 最小有效距离, m [0, 10]
    qreal max_dist_; // 最大有效距离, m [0, 10]
    qreal range_;      // 探测范围角度, 度 [0, 360]
    qreal max_speed_;
    UltrasonicData():
        id_(0),
        func_(0),
        r_(0),
        x_(0),
        y_(0),
        z_(0),
        min_dist_(0.13),
        max_dist_(1.0),
        range_(85),
        max_speed_(0.)
    {}

    UltrasonicData(const UltrasonicData& data_)
    {
        id_ = data_.id_;
        func_ = data_.func_;
        x_ = data_.x_;
        y_ = data_.y_;
        z_ = data_.z_;
        r_ = data_.r_;
        min_dist_ = data_.min_dist_;
        max_dist_ = data_.max_dist_;
        range_ = data_.range_;
        max_speed_ = data_.max_speed_;
    }

    UltrasonicData& operator =(const UltrasonicData& data_)
    {
        this->id_ = data_.id_;
        this->func_ = data_.func_;
        this->x_ = data_.x_;
        this->y_ = data_.y_;
        this->z_ = data_.z_;
        this->r_ = data_.r_;
        this->min_dist_ = data_.min_dist_;
        this->max_dist_ = data_.max_dist_;
        this->range_ = data_.range_;
        this->max_speed_ = data_.max_speed_;

        return *this;
    }
};

struct CameraData
{
    quint8 type_;
    qreal x_;
    qreal y_;
    qreal z_;
    qreal yaw_;
    qreal pitch_;
    qreal roll_;

    CameraData()
        :type_(0),
          x_(0),
          y_(0),
          z_(0),
          yaw_(0),
          pitch_(0),
          roll_(0)
    {

    }

    CameraData(const CameraData& data)
    {
        this->type_ = data.type_;
        this->x_ = data.x_;
        this->y_ = data.y_;
        this->z_ = data.z_;
        this->yaw_ = data.yaw_;
        this->pitch_ = data.pitch_;
        this->roll_ = data.roll_;
    }
};

typedef struct dataIndex_
{
    int a_index;//运动方式comboxIndex
    int b_index;//底盘选型comboxIndex

    dataIndex_():a_index(0),b_index(0)
    {

    }

    dataIndex_(const dataIndex_& data)
    {
        a_index = data.a_index;
        b_index = data.b_index;
    }
}DataIndex;

//底盘控件属性控件ID

enum  WidgetType
{
    ID_LABEL_WIDTH = 0,
    ID_LABEL_HEAD,
    ID_LABEL_TAIL,
    ID_LABEL_RADIUS,//轮子半径
    ID_LABEL_OUTLINE_RADIUS,//圆形轮廓半径
    ID_LABEL_A_AXIS,
    ID_LABEL_B_AXIS,
    ID_LABLE_ALLDIRECTION_A_AXIS,
    ID_LABEL_ALLDIRECTION_ANGLE,
    ID_LABEL_STEER_ANGEL_MIN,
    ID_LABEL_STEER_ANGEL_MAX,
    ID_LABEL_ABSENCODER_RANGLE_MIN,
    ID_LABEL_ABSENCODER_RANGLE_MAX,
    ID_LABEL_ABSENCODER_TYPE,

    ID_DOUBLESPINBOX_WIDTH,
    ID_DOUBLESPINBOX_HEAD,
    ID_DOUBLESPINBOX_TAIL,
    ID_DOUBLESPINBOX_RADIUS,
    ID_DOUBLESPINBOX_OUTLINE_RADIUS,
    ID_DOUBLESPINBOX_A_AXIS,
    ID_DOUBLESPINBOX_B_AXIS,
    ID_DOUBLESPINBOX_ALLDIRECION_AXIS,
    ID_DOUBLESPINBOX_ALLDIRECTION_ANGLE,
    ID_DOUBLESPINBOX_STEER_ANGLE_MIN,
    ID_DOUBLESPINBOX_STEER_ANGLE_MAX,
    ID_DOUBLESPINBOX_ABSENCODER_RANGLE_MIN,
    ID_DOUBLESPINBOX_ABSENCODER_RANGLE_MAX,

    ID_COMBOX_ABSENCODER_TYPE


};
//控件的度量单位
enum dataType
{
    TYPE_METER,//米
    TYPE_DEGREE,//度
    TYPE_NONE
};

enum ItemType
{
    I_polygon_pos=2000,
    I_polygon=2001,
    I_laser=2002,
    I_di=2003,
    I_magnetic_sensor=2004,
    I_robot_model=2005,
    I_ultrasonic=2006,
    I_do=2007,
    I_crashDi = 2008,
    I_crashDipos=2009,
    I_circle_robot_model = 2010,
    I_scanArea_model = 2011,
    I_RFID_sensor = 2012,
    I_Camera =2013,
    I_angle_controller=6001,
    I_Axis=10000,
    INVALIDE
};

enum AdjustType
{
    NONE=0,
    H_L,                //水平-左边
    H_R,
    V_T,
    V_B,
    C_TL,               //角 - 左上
    C_TR,
    C_BL,
    C_BR,
    Rot                 //旋转
};



#endif // COMMON_H
