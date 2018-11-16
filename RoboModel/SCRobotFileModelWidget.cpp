#include "SCRobotFileModelWidget.h"
#include "ui_SCRobotFileModelWidget.h"
#include "PublicClass.h"
#include "UiClass.h"
#include <QFileDialog>
#include <QFileInfo>
#include"ItemPolygon.h"
#include"ItemLaser.h"
#include"ItemMagneticSensor.h"
#include"ItemDI.h"
#include"ItemDO.h"
#include"ItemUltrasonic.h"
#include"WidgetDOState.h"
#include"ItemCrashDI.h"
#include"ItemRFID.h"
#include<QGridLayout>
#include<QHBoxLayout>
#include<QDebug>
#include<QMessageBox>
#include"ModelGraphicsView.h"
#include"ItemAngelCtroller.h"
#include"ModelHelper.h"
#include "SendGlobalMsg.h"
#include"UndoStack.h"//单例模式
#include"StyleObject.h"
#include"GlobalVariant.h"
#include"../GuidenceWidgets/LaserContentSetting.h"
#include"../GuidenceWidgets/UltrasonicContentSetting.h"
#include"../GuidenceWidgets/MagSensorContentSetting.h"
#include"../GuidenceWidgets/DOContentSetting.h"
#include"../GuidenceWidgets/DIContentSetting.h"
#include"../GuidenceWidgets/GuidModelName.h"
#include"../GuidenceWidgets/GuidSimpleDevice.h"
#include"../GuidenceWidgets/GuidoptionalDevice.h"
#include"../GuidenceWidgets/NumSetting.h"
#include<QScrollBar>
#include "MaskWidget.h"
#include <QShortcut>
#include<QDesktopServices>

SCRobotFileModelWidget::SCRobotFileModelWidget( QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SCRobotFileModelWidget)
  ,m_undoStack(Q_NULLPTR)
  ,_isSaved(true)
  ,_modelchoseDlg(new DialogModelChose)
{
    ui->setupUi(this);
    _maskWid = new MaskWidget(this);
    _maskWid->hide();
    _promptWid = new PropertyMissPromtWidget(this);
    _promptWid->hide();

    initUI();
    connect(_promptWid,SIGNAL(sig_promptWid_close()),this,SLOT(slot_Prompt_close()));
    connect(_modelchoseDlg,SIGNAL(sig_confirm()),SLOT(slot_choseConfirm()));
    connect(SendGlobalMsg::init(),SIGNAL(sigSendGlobalMsgResult(int,QVariant,bool)),
            this,SLOT(slotSendGlobalMsgResult(int,QVariant,bool)));

    //新建相关
    connect(m_newByDefault,SIGNAL(triggered(bool)),SLOT(slotNewModelbyDefault()));
    connect(m_newByGuidence,SIGNAL(triggered(bool)),SLOT(slotNewModelbyGudience()));
    connect(m_newByTemplate,SIGNAL(triggered(bool)),SLOT(slotNewModelbyTemplate()));
    connect(m_act_loadModel,SIGNAL(triggered(bool)),SLOT(slotLoadModel()));
    connect(m_act_saveModel,SIGNAL(triggered(bool)),SLOT(slotSaveModel()));
    connect(m_act_saveModelAs,SIGNAL(triggered(bool)),SLOT(slotSaveModelAs()));
    connect(m_act_upload,SIGNAL(triggered(bool)),SLOT(slotUpload()));
    connect(m_act_saveAsTemplate,SIGNAL(triggered(bool)),SLOT(slotSaveAsTemp()));
    connect(m_act_openTemplate,SIGNAL(triggered(bool)),SLOT(slotOpenTemp()));
    connect(m_act_openTempLocation,SIGNAL(triggered(bool)),SLOT(slotOpenTempLocation()));

    //undostack change signal
    connect(m_undoStack,SIGNAL(canRedoChanged(bool)),SLOT(slotEnableSaveMode()));
    connect(m_undoStack,SIGNAL(canUndoChanged(bool)),SLOT(slotEnableSaveMode()));
    //
    //-------------------------
    //默认加载一个模型文件
    QString modelDir = PublicClass::init()->getModelsPath();
    SCDebug<<"read file path is "<<modelDir;
    QFile file(modelDir+"/robot.model");
    SCDebug<<"file path is "<<modelDir+"/robot.model";
    if(file.exists())
    {
        if(loadFile(modelDir+"/robot.model"))
        {
            setCurrentFilePath(modelDir+"/robot.model");
            set_file_existed(true);
            //初始化时设置当前文件类型
            _filetype = FILE_MODEL;
        }
        else
        {
            //打开文件失败时，禁用菜单中保存，另存为等按钮
            set_file_existed(false);
            setCurrentFilePath("");
        }
        fitPropertyWidget();
    }
    else
    {
        set_file_existed(false);
        setCurrentFilePath("");
    }
    connect(RobotManager::init(),SIGNAL(sigRoTcpManagerStatus(QString,RobotInterface::RobotCellStatus)),
            this,SLOT(slotRoTcpManagerStatus(QString,RobotInterface::RobotCellStatus)));
    init();
    //初始为已保存状态
    m_act_saveModel->setEnabled(false);
}

SCRobotFileModelWidget::~SCRobotFileModelWidget()
{
    SCDebug<<"~SCRobotFileModelWidget";
    UiClass::init()->releaseSCToastr(this);
}
void SCRobotFileModelWidget::init()
{
    ///TODO -----init tcp------
    //获取当前通信类
    RoTcpManager *tcpManager = RobotManager::init()->getCurrentRobot();
    if(tcpManager)
    {
        connect(tcpManager,SIGNAL(sigConfigTcpReceiveAll(QString,QString,QVariant,int,bool)),
                this,SLOT(slotConfigTcpReceiveAll(QString,QString,QVariant,int,bool)),Qt::UniqueConnection);
        connect(tcpManager,SIGNAL(sigStatusTcpReceiveAll(QString,QString,QVariant,int,bool)),
                this,SLOT(slotStatusTcpReceiveAll(QString,QString,QVariant,int,bool)),Qt::UniqueConnection);

    }
    if(RobotManager::init()->isConnected()){

        //判断连接状态-是否可上传下载
        setUpandDownloadBtnEnable(true);
    }else{
        //判断连接状态-是否可上传下载
        setUpandDownloadBtnEnable(false);
        _isShowToolTipWidget = false;
        UiClass::init()->slotDeleteToolTipWidget(_toolTipWidgetKey);
        _toolTipWidgetKey = 0;
    }
}
void SCRobotFileModelWidget::initUI()
{
    QString imagePath = StyleObject::init()->getCurrentSkinDirPath()+"/images/"+"/RoModelPlugin/";

    m_toolBar = new QToolBar(this);
    m_toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_toolBar->setContextMenuPolicy(Qt::CustomContextMenu);

    m_newModelMenu = new QMenu;
    m_newByDefault = new QAction(tr("New by default")) ;//
    m_newByGuidence = new QAction(tr("New with Gudience")) ;//
    m_newByTemplate = new QAction(tr("New using templates")) ;//
    m_newModelMenu->addAction(m_newByGuidence);
    m_newModelMenu->addAction(m_newByTemplate);
    m_newModelMenu->addAction(m_newByDefault);

    m_act_newModel = new  QAction(QIcon(imagePath+"newModel"),tr("New"))  ;

    QToolButton*  pNewModelToolButton = new QToolButton(this);
    pNewModelToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    pNewModelToolButton->setDefaultAction(m_act_newModel);
    pNewModelToolButton->setPopupMode(QToolButton::InstantPopup);
    pNewModelToolButton->setMenu(m_newModelMenu);
    m_toolBar->addWidget(pNewModelToolButton);

    m_act_loadModel =  new  QAction(QIcon(imagePath+"load"),tr("Load"))  ;
    m_act_saveModel = new  QAction(QIcon(imagePath+"save"),tr("Save"))  ;
    m_act_saveModelAs =new  QAction(QIcon(imagePath+"saveAs"),tr("Save As"))  ;
    m_act_upload =new  QAction(QIcon(imagePath+"upload"),tr("Upload"))  ;
    m_act_Template = new QAction(QIcon(imagePath+"saveAstemp"),tr("Template"));
    m_act_saveAsTemplate = new QAction(tr("Save As Template"));
    m_act_openTemplate = new QAction(tr("Open Template"));
    m_act_openTempLocation = new QAction(tr("Open Template file directory"));
    QToolButton* ptempToolButton = new QToolButton(this);
    ptempToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ptempToolButton->setDefaultAction(m_act_Template);
    ptempToolButton->setPopupMode(QToolButton::InstantPopup);
    m_tempMenu = new QMenu;
    m_tempMenu->addAction(m_act_openTemplate);
    m_tempMenu->addAction(m_act_saveAsTemplate);
    m_tempMenu->addAction(m_act_openTempLocation);
    ptempToolButton->setMenu(m_tempMenu);
    m_toolBar->addWidget(ptempToolButton);

    m_undoStack = UndoStack::InitStack();//单例模式
    undoAction = m_undoStack->createUndoAction(this);
    redoAction = m_undoStack->createRedoAction(this);

    undoAction->setIcon(QIcon(StyleObject::init()->getCurrentSkinDirPath()+"/images/btn_undo"));
    redoAction->setIcon(QIcon(StyleObject::init()->getCurrentSkinDirPath()+"/images/btn_redo"));

    undoAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Z));
    redoAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Y));

    QToolButton* btn_redo = new QToolButton;
    QToolButton* btn_undo = new QToolButton;
    btn_redo->setDefaultAction(redoAction);
    btn_redo->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btn_undo->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    //    btn_redo->setText(tr("Redo"));
    //    btn_undo->setText(tr("Undo"));
    btn_undo->setDefaultAction(undoAction);

    connect(btn_redo,SIGNAL(pressed()),SLOT(slot_redo_clicked()));
    connect(btn_undo,SIGNAL(pressed()) ,SLOT(slot_undo_clicked()));
    //    m_toolBar->addAction(m_act_newModel);
    m_toolBar->addAction(m_act_loadModel);
    m_toolBar->addAction(m_act_saveModel);
    m_toolBar->addAction(m_act_saveModelAs);
    //    m_toolBar->addAction(m_act_Template);
    m_toolBar->addSeparator();
    m_toolBar->addAction(m_act_upload);
    m_toolBar->addSeparator();
    m_toolBar->addWidget(btn_undo);
    m_toolBar->addWidget(btn_redo);

    ui->hlayout_menu->addWidget(m_toolBar);
    //选择按钮样式
    ToolButtonStyleList<<Qt::ToolButtonIconOnly<<Qt::ToolButtonTextOnly
                      <<Qt::ToolButtonTextUnderIcon;

}
void SCRobotFileModelWidget::keyPressEvent(QKeyEvent *event)
{
    _is_Ctrl_Key_pressed = event->modifiers();
    qDebug()<<"val is ------------"<<_is_Ctrl_Key_pressed;

    if(event->modifiers() && event->key() == Qt::Key_Z){
        if(! event->isAutoRepeat())
        {

            SCDebug<<"event->isAutoRepeat()";
        }
    }
    else if(event->modifiers()&&event->key()==Qt::Key_S)
    {
        slotSaveModel();
    }
    QWidget::keyPressEvent(event);

}

bool SCRobotFileModelWidget::isAllowExec()
{
    if(!pEditRobotModelWidget) return true;
    if(!_isSaved)
    {
        QStringList list;
        list<<tr("Yes")<<tr("No");
        int rb = UiClass::init()->showDialog(tr("Do you want to save the current model?"),tr("Information"),0,list);
        if(rb ==0)
        {
            slotSaveModel();
            if(!_isSaved)
            {
                //没有保存成功
                return false;
            }
        }
        else
        {
            return true;
            //不保存继续往下执行
        }
    }
    return true;
}

bool SCRobotFileModelWidget::isGuidenceFinished()
{
    if(_isGeneratedByGuidence)
    {
        bool isfinished = pEditRobotModelWidget->getGraphicView()->model_scene()->isGuidenceFinished();

        if(isfinished)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    return true;
}

QStringList SCRobotFileModelWidget::getModelNames()//.model or .tmp
{
    QStringList namelist;
    QString  dirPath = PublicClass::init()->getModelsPath();
    QDir dir(dirPath);
    QStringList filter;
    filter<<"*.model";
    dir.setNameFilters(filter);
    QFileInfoList fileInfoList=dir.entryInfoList(filter);
    dir.setCurrent(qApp->applicationDirPath());
    for (int i=0;i<fileInfoList.count();i++)
    {
        namelist<<fileInfoList[i].fileName();
    }
    return namelist;
}

bool SCRobotFileModelWidget::getIsGeneratbyGuid()
{
    return _isGeneratedByGuidence;
}

void SCRobotFileModelWidget::setIsGeneratbyGuid(bool guid)
{
    _isGeneratedByGuidence = guid;
}

void SCRobotFileModelWidget::closeEvent(QCloseEvent *event)
{
    SCDebug<<_isSaved;
    if(willCloseWidget()){
        //先closeEvent
        QWidget::closeEvent(event);
        emit sigClosedWidget();
    }else{
        event->ignore();
    }
}
/** 将要关闭界面
 * @brief SCRobotFileModelWidget::willCloseWidget
 * @return
 */
bool SCRobotFileModelWidget::willCloseWidget()
{
    SCDebug<<_isSaved;
    if(m_act_saveModel->isEnabled())
    {
        QStringList list;
        list<<tr("Save and Close")<<tr("Do not Save")<<tr("Cancel");
        int rb = UiClass::init()->showDialog(tr("Do you want to close the current model ?"),tr("Information"),0,list);
        if(rb ==0)
        {
            pEditRobotModelWidget->saveFile(pEditRobotModelWidget->filePath());
            if(pEditRobotModelWidget->getIsSaveSuccess())//如果保存 成功
            {
                UndoStack::InitStack()->clear();//退出后清空undoStack
                return true;
            }
            else
            {
                return false;
            }
        }
        else if(rb==1)
        {
            UndoStack::InitStack()->clear();
        }
        else if(rb == 2 || -1 == rb)
        {
            return false;
        }
    }
    return true;
}
void SCRobotFileModelWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    _maskWid->move(0,0);
    QRect rect = this->rect();
    _maskWid->setGeometry(rect);
    _promptWid->updateWidget();
}

void SCRobotFileModelWidget::newModel()
{
    //取消填充空白区域
    if(pEditRobotModelWidget)
    {
        delete pEditRobotModelWidget;
    }
    GlobalVariant::getInst()->flag = false;
    //新建模型是清空undoStack
    m_undoStack->clear();
    //保存的名称去重
    QStringList namelist = getModelNames();

    int idex = namelist.size();
    QString filePath = QString("new%1").arg(idex);
    while(1)
    {
        if(namelist.contains(filePath+".model"))
        {
            idex++;
            filePath = QString("new%1").arg(idex);
        }
        else
        {
            break;
        }
    }

    pEditRobotModelWidget = new EditRobotModelWidget(this);
    pEditRobotModelWidget->setFilePath(filePath);
    pEditRobotModelWidget->show();
    ui->vlayout_model->addWidget(pEditRobotModelWidget);

    pEditRobotModelWidget->setFileName(filePath);
    setCurrentFilePath(filePath);//记录当前模型路径
    set_file_existed(true);

    //新建时默认保存，用于第一次上传
    QString modelDir = PublicClass::init()->getModelsPath();
    QString fileName = QString("/new%1.model").arg(idex);
    pEditRobotModelWidget->setFilePath(modelDir+fileName);
    pEditRobotModelWidget->saveFile(modelDir+fileName);//进行保存
    setCurrentFilePath(modelDir+fileName);

    //对basicWidget宽度自适应
    fitPropertyWidget();
}

void SCRobotFileModelWidget::updateToolButtonStyle(int index)
{
    _currentToolButtonStyleIndex = index;
    if(m_toolBar)
    {
        m_toolBar->setToolButtonStyle(ToolButtonStyleList.at(index));
    }
}

void SCRobotFileModelWidget::fitPropertyWidget()
{
    if(!pEditRobotModelWidget) return;
    //对basicWidget宽度自适应
    int outWidth1 = pEditRobotModelWidget->getGraphicView()->basic_widget()->width();
    int inWidth1 = pEditRobotModelWidget->getGraphicView()->basic_widget()->getScrollAreaContent()->width();

    int setWidth = outWidth1 >= inWidth1? outWidth1:inWidth1;

    pEditRobotModelWidget->getGraphicView()->basic_widget()->setMinimumWidth(setWidth+42);

    int outWidth2 = pEditRobotModelWidget->getGraphicView()->property_widget()->width();
    int inWidth2 = pEditRobotModelWidget->getGraphicView()->property_widget()->getScrollcontentWidget()->width();

    setWidth = outWidth2 >= inWidth2? outWidth2:inWidth2;
    pEditRobotModelWidget->getGraphicView()->property_widget()->setMinimumWidth(setWidth+30);
}

void SCRobotFileModelWidget::slotGuidNewModel()
{
    if(!_guidenceWidget->isHidden()){
        _guidenceWidget->close();
    }
    newModel();
    //向场景中添加要配置的设备
    QList<GuidBaseSetting *>* widlist =  _guidenceWidget->getGuidWidList();
    ModelGraphicsScene* scene = pEditRobotModelWidget->getGraphicView()->model_scene();
    ModelBasicSettingWidget *widget_basic_setting_ = pEditRobotModelWidget->getGraphicView()->basic_widget();
    scene->getItem_robot_model()->isCreatedByLoading_ = true;
    for(int i = 0; i < widlist->size();i++)
    {
        GuidType type = widlist->at(i)->getType();
        switch (type) {
        case GUID_MODEL_NAME:
        {
            GuidModelName* temp = dynamic_cast<GuidModelName*>(widlist->at(i));
            widget_basic_setting_->getModelName()->blockSignals(true);
            widget_basic_setting_->getModelName()->setText(temp->basicData_.model_name_);
            widget_basic_setting_->getModelName()->blockSignals(false);
            widget_basic_setting_->getModelName()->setOriValue(temp->basicData_.model_name_);
            widget_basic_setting_->model_base_data_.model_name_ = temp->basicData_.model_name_;
        }
            break;
        case GUID_SIMPLEDEVICE:
        {
            GuidSimpleDevice* temp = dynamic_cast<GuidSimpleDevice*>(widlist->at(i));
            widget_basic_setting_->getGyro()->blockSignals(true);
            int idx = widget_basic_setting_->getGyro()->findData(temp->basicData_.gyro_type_);
            widget_basic_setting_->getGyro()->setCurrentIndex(idx);
            widget_basic_setting_->getGyro()->blockSignals(false);
            widget_basic_setting_->model_base_data_.gyro_type_ = temp->basicData_.gyro_type_;
            widget_basic_setting_->getGyro()->setOri_index(idx);
            widget_basic_setting_->getBattery()->blockSignals(true);
            idx = widget_basic_setting_->getBattery()->findData(temp->basicData_.battery_info_);
            widget_basic_setting_->getBattery()->setCurrentIndex(idx);
            widget_basic_setting_->model_base_data_.battery_info_ = temp->basicData_.battery_info_;
            widget_basic_setting_->getBattery()->blockSignals(false);
            widget_basic_setting_->getBattery()->setOri_index(idx);
            widget_basic_setting_->getDrive()->blockSignals(true);
            idx = widget_basic_setting_->getDrive()->findData(temp->basicData_.driver_);
            widget_basic_setting_->getDrive()->setCurrentIndex(idx);
            widget_basic_setting_->model_base_data_.driver_ = temp->basicData_.driver_;
            widget_basic_setting_->getDrive()->blockSignals(false);
            widget_basic_setting_->getDrive()->setOri_index(idx);
            widget_basic_setting_->getDriveBrand()->blockSignals(true);
            widget_basic_setting_->getDriveBrand()->setCurrentText(temp->basicData_.driverBrand_);
            widget_basic_setting_->model_base_data_.driverBrand_ = temp->basicData_.driverBrand_;
            qDebug()<<"driverBrand is --------"<<temp->basicData_.driverBrand_;
            int idex = widget_basic_setting_->getDriveBrand()->currentIndex();
            widget_basic_setting_->getDriveBrand()->setOri_index(idex);
            widget_basic_setting_->getDriveBrand()->blockSignals(false);
        }
            break;
        case GUID_OPTIONALDEVICE:
        {
            GuidoptionalDevice* temp = dynamic_cast<GuidoptionalDevice*>(widlist->at(i));
            widget_basic_setting_->getAutoBrake()->blockSignals(true);
            widget_basic_setting_->getAutoBrake()->setChecked(temp->basicData_.auto_brake_);
            widget_basic_setting_->model_base_data_.auto_brake_ = temp->basicData_.auto_brake_;
            widget_basic_setting_->getAutoBrake()->blockSignals(false);
            widget_basic_setting_->getAutoBrake()->setOriValue(temp->basicData_.auto_brake_);
            widget_basic_setting_->getBrake()->blockSignals(true);
            widget_basic_setting_->getBrake()->setChecked(temp->basicData_.brake_);
            widget_basic_setting_->model_base_data_.brake_ = temp->basicData_.brake_;
            widget_basic_setting_->getBrake()->blockSignals(false);
            temp->basicData_.brake_?widget_basic_setting_->getAutoBrake()->setEnabled(true):widget_basic_setting_->getAutoBrake()->setEnabled(false);
            widget_basic_setting_->getBrake()->setOriValue(temp->basicData_.brake_);
            widget_basic_setting_->getAutoGyroCal()->blockSignals(true);
            widget_basic_setting_->getAutoGyroCal()->setChecked(temp->basicData_.auto_gyro_cal_);
            widget_basic_setting_->model_base_data_.auto_gyro_cal_ = temp->basicData_.auto_gyro_cal_;
            widget_basic_setting_->getAutoGyroCal()->blockSignals(false);
            widget_basic_setting_->getAutoGyroCal()->setOriValue(temp->basicData_.auto_gyro_cal_);
            widget_basic_setting_->getLed()->blockSignals(true);
            widget_basic_setting_->getLed()->setChecked(temp->basicData_.LED_);
            widget_basic_setting_->model_base_data_.LED_ = temp->basicData_.LED_;
            widget_basic_setting_->getLed()->blockSignals(false);
            widget_basic_setting_->getLed()->setOriValue(temp->basicData_.LED_);
        }
            break;
        case GUID_BASIC:
        {
            GuidBasicSetting* temp = dynamic_cast<GuidBasicSetting*>(widlist->at(i));

            // widget_basic_setting_->getModelName()->blockSignals(true);
            // widget_basic_setting_->getModelName()->setText(temp->basicData_.model_name_);
            // widget_basic_setting_->getModelName()->blockSignals(false);
            // widget_basic_setting_->getModelName()->setOriValue(temp->basicData_.model_name_);

            widget_basic_setting_->getReduceRadio()->blockSignals(true);
            widget_basic_setting_->getReduceRadio()->setValue(temp->basicData_.reduction_ratio_);
            widget_basic_setting_->getReduceRadio()->blockSignals(false);
            widget_basic_setting_->getReduceRadio()->setOriValue(temp->basicData_.reduction_ratio_);
            widget_basic_setting_->model_base_data_.reduction_ratio_ = temp->basicData_.reduction_ratio_;

            widget_basic_setting_->getEncoderLine()->blockSignals(true);
            widget_basic_setting_->getEncoderLine()->setValue(temp->basicData_.encoder_line_);
            widget_basic_setting_->getEncoderLine()->blockSignals(false);
            widget_basic_setting_->getEncoderLine()->setOriValue(temp->basicData_.encoder_line_);
            widget_basic_setting_->model_base_data_.encoder_line_ = temp->basicData_.encoder_line_;
            widget_basic_setting_->getMaxRpm()->blockSignals(true);
            widget_basic_setting_->getMaxRpm()->setValue(temp->basicData_.max_motor_rpm_);
            widget_basic_setting_->getMaxRpm()->blockSignals(false);
            widget_basic_setting_->getMaxRpm()->setOriValue(temp->basicData_.max_motor_rpm_);
            widget_basic_setting_->model_base_data_.max_motor_rpm_ = temp->basicData_.max_motor_rpm_;

        }
            break;
        case GUID_CHASSIS_TYPE:
        {
            //设置底盘属性
            GuidChassisTypeSelect* temp =  dynamic_cast<GuidChassisTypeSelect*>(widlist->at(i));

            ShapeType shape = temp->roboData_.shapType_;

            if(shape == RECTANGLE)
            {
                scene->getItem_robot_model()->setChasisShapeType(shape);
                MoveStyle mvstyle = temp->roboData_.mvStyle_;
                scene->getItem_robot_model()->robot_model_data_.mvStyle_ = mvstyle;
                ChassisType chastype = temp->roboData_.chasisType_;
                scene->getItem_robot_model()->setChasiseType(chastype);
                scene->getItem_robot_model()->robot_model_data_.chasisType_ = chastype;
                scene->getItem_robot_model()->robot_model_data_.wheelRadius_ = temp->roboData_.wheelRadius_;
                scene->getItem_robot_model()->robot_model_data_.head_ = temp->roboData_.head_;
                scene->getItem_robot_model()->robot_model_data_.tail_ = temp->roboData_.tail_;
                scene->getItem_robot_model()->robot_model_data_.width_ = temp->roboData_.width_;
                scene->getItem_robot_model()->setMOriHead(temp->roboData_.head_);
                scene->getItem_robot_model()->setMOriTail(temp->roboData_.tail_);
                scene->getItem_robot_model()->setMOriWidth(temp->roboData_.width_);
                switch (chastype)
                {
                case DIFFERENTIAL_TWOWHEEL://差动两轮
                    //model切换记录属性参数
                    scene->getItem_robot_model()->robot_model_data_.chasisModel.DiffTwoWheel.a_axis_distance
                            = temp->roboData_.chasisModel.DiffTwoWheel.a_axis_distance;
                    //设置电机正反转及编号默认值（由于不确定电机位置引导中没有进行引导配置）
                    scene->getItem_robot_model()->robot_model_data_.driverID.clear();
                    scene->getItem_robot_model()->robot_model_data_.driverID.append(1);
                    scene->getItem_robot_model()->robot_model_data_.driverID.append(2);
                    scene->getItem_robot_model()->robot_model_data_.inverse_.clear();
                    scene->getItem_robot_model()->robot_model_data_.inverse_.append(false);
                    scene->getItem_robot_model()->robot_model_data_.inverse_.append(false);
                    scene->getItem_robot_model()->inverse_list_.clear();
                    scene->getItem_robot_model()->inverse_list_.append(false);
                    scene->getItem_robot_model()->inverse_list_.append(false);
                    break;
                case DIFFERENTIAL_FOURWHEEL://差动四轮

                    //model切换记录属性参数
                    scene->getItem_robot_model()->robot_model_data_.chasisModel.DiffFourWheel.a_axis_distance
                            = temp->roboData_.chasisModel.DiffTwoWheel.a_axis_distance;
                    scene->getItem_robot_model()->robot_model_data_.chasisModel.DiffFourWheel.b_axis_distance
                            = temp->roboData_.chasisModel.DiffFourWheel.b_axis_distance;
                    //设置电机正反转及编号默认值（由于不确定电机位置引导中没有进行引导配置）
                    scene->getItem_robot_model()->robot_model_data_.driverID.clear();
                    scene->getItem_robot_model()->robot_model_data_.driverID.append(1);
                    scene->getItem_robot_model()->robot_model_data_.driverID.append(2);
                    scene->getItem_robot_model()->robot_model_data_.driverID.append(3);
                    scene->getItem_robot_model()->robot_model_data_.driverID.append(4);
                    scene->getItem_robot_model()->robot_model_data_.inverse_.clear();
                    scene->getItem_robot_model()->robot_model_data_.inverse_.append(false);
                    scene->getItem_robot_model()->robot_model_data_.inverse_.append(false);
                    scene->getItem_robot_model()->robot_model_data_.inverse_.append(false);
                    scene->getItem_robot_model()->robot_model_data_.inverse_.append(false);
                    scene->getItem_robot_model()->inverse_list_.clear();
                    scene->getItem_robot_model()->inverse_list_.append(false);
                    scene->getItem_robot_model()->inverse_list_.append(false);
                    scene->getItem_robot_model()->inverse_list_.append(false);
                    scene->getItem_robot_model()->inverse_list_.append(false);
                    break;
                case ALLDIRECTION_THREEWHEEL://全向三轮
                    //model切换记录属性参数
                    scene->getItem_robot_model()->robot_model_data_.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle
                            = temp->roboData_.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle ;
                    scene->getItem_robot_model()->robot_model_data_.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance
                            = temp->roboData_.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance;
                    //设置电机正反转及编号默认值（由于不确定电机位置引导中没有进行引导配置）
                    scene->getItem_robot_model()->robot_model_data_.driverID.clear();
                    scene->getItem_robot_model()->robot_model_data_.driverID.append(1);
                    scene->getItem_robot_model()->robot_model_data_.driverID.append(2);
                    scene->getItem_robot_model()->robot_model_data_.driverID.append(3);
                    scene->getItem_robot_model()->robot_model_data_.inverse_.clear();
                    scene->getItem_robot_model()->robot_model_data_.inverse_.append(false);
                    scene->getItem_robot_model()->robot_model_data_.inverse_.append(false);
                    scene->getItem_robot_model()->robot_model_data_.inverse_.append(false);
                    scene->getItem_robot_model()->inverse_list_.clear();
                    scene->getItem_robot_model()->inverse_list_.append(false);
                    scene->getItem_robot_model()->inverse_list_.append(false);
                    scene->getItem_robot_model()->inverse_list_.append(false);
                    break;
                case MECANUM_FOURWHEEL://四轮麦克纳姆
                    //model切换记录属性参数
                    scene->getItem_robot_model()->robot_model_data_.chasisModel.MecanumFourWheel.a_axis_distance
                            = temp->roboData_.chasisModel.MecanumFourWheel.a_axis_distance;
                    scene->getItem_robot_model()->robot_model_data_.chasisModel.MecanumFourWheel.b_axis_distance
                            = temp->roboData_.chasisModel.MecanumFourWheel.b_axis_distance;
                    //设置电机正反转及编号默认值（由于不确定电机位置引导中没有进行引导配置）
                    scene->getItem_robot_model()->robot_model_data_.driverID.clear();
                    scene->getItem_robot_model()->robot_model_data_.driverID.append(1);
                    scene->getItem_robot_model()->robot_model_data_.driverID.append(2);
                    scene->getItem_robot_model()->robot_model_data_.driverID.append(3);
                    scene->getItem_robot_model()->robot_model_data_.driverID.append(4);
                    scene->getItem_robot_model()->robot_model_data_.inverse_.clear();
                    scene->getItem_robot_model()->robot_model_data_.inverse_.append(false);
                    scene->getItem_robot_model()->robot_model_data_.inverse_.append(false);
                    scene->getItem_robot_model()->robot_model_data_.inverse_.append(false);
                    scene->getItem_robot_model()->robot_model_data_.inverse_.append(false);
                    scene->getItem_robot_model()->inverse_list_.clear();
                    scene->getItem_robot_model()->inverse_list_.append(false);
                    scene->getItem_robot_model()->inverse_list_.append(false);
                    scene->getItem_robot_model()->inverse_list_.append(false);
                    scene->getItem_robot_model()->inverse_list_.append(false);
                    break;
                case SINGLESTEERWHEEL_FRONTDRIVER_FRONT_CODERTYPE://前驱+前转动编码器
                case SINGLESTEERWHEEL_FRONTDRIVER_BACK_CODERTYPE://前驱+后转动编码器
                case SINGLESTEERWHEEL_BACKDRIVER_FRONT_CODERTYPE://后驱+前转动编码器
                case SINGLESTEERWHEEL_BACKDRIVER_BACK_CODERTYPE://后驱+后转动编码器
                    //model切换记录属性参数
                    scene->getItem_robot_model()->robot_model_data_.chasisModel.SingleWheelDriver.a_axis_distance
                            = temp->roboData_.chasisModel.SingleWheelDriver.a_axis_distance;
                    scene->getItem_robot_model()->robot_model_data_.chasisModel.SingleWheelDriver.b_axis_distance
                            = temp->roboData_.chasisModel.SingleWheelDriver.b_axis_distance;
                    scene->getItem_robot_model()->robot_model_data_.chasisModel.SingleWheelDriver.steer_angle_max
                            = temp->roboData_.chasisModel.SingleWheelDriver.steer_angle_max;
                    scene->getItem_robot_model()->robot_model_data_.chasisModel.SingleWheelDriver.steer_angle_min
                            = temp->roboData_.chasisModel.SingleWheelDriver.steer_angle_min;
                    scene->getItem_robot_model()->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderRange_max
                            = temp->roboData_.chasisModel.SingleWheelDriver.absEncoderRange_max;
                    scene->getItem_robot_model()->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderRange_min
                            = temp->roboData_.chasisModel.SingleWheelDriver.absEncoderRange_min;
                    scene->getItem_robot_model()->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderType
                            = temp->roboData_.chasisModel.SingleWheelDriver.absEncoderType;
                    //设置电机正反转及编号默认值（由于不确定电机位置引导中没有进行引导配置）
                    scene->getItem_robot_model()->robot_model_data_.driverID.clear();
                    scene->getItem_robot_model()->robot_model_data_.driverID.append(1);
                    scene->getItem_robot_model()->robot_model_data_.inverse_.clear();
                    scene->getItem_robot_model()->robot_model_data_.inverse_.append(false);
                    scene->getItem_robot_model()->inverse_list_.clear();
                    scene->getItem_robot_model()->inverse_list_.append(false);
                    break;
                default:
                    break;
                }

            }
            else if(shape == CIRCLE)
            {
                scene->getItem_robot_model()->setChasisShapeType(shape);
                MoveStyle mvstyle = temp->roboData_.mvStyle_;
                scene->getItem_robot_model()->robot_circle_model_data_.mvStyle_ = mvstyle;
                ChassisType chastype = temp->roboData_.chasisType_;
                scene->getItem_robot_model()->robot_circle_model_data_.chasisType_ = chastype;
                scene->getItem_robot_model()->robot_circle_model_data_.radius_ = temp->roboData_.radius_;
                scene->getItem_robot_model()->robot_circle_model_data_.wheelRadius_ = temp->roboData_.wheelRadius_;
                scene->getItem_robot_model()->setMOriRadius(temp->roboData_.radius_);
                switch (chastype)
                {
                case DIFFERENTIAL_TWOWHEEL://差动两轮
                    //model切换记录属性参数
                    scene->getItem_robot_model()->robot_circle_model_data_.chasisModel.DiffTwoWheel.a_axis_distance
                            = temp->roboData_.chasisModel.DiffTwoWheel.a_axis_distance;
                    //设置电机正反转及编号默认值（由于不确定电机位置引导中没有进行引导配置）
                    scene->getItem_robot_model()->robot_circle_model_data_.driverID.clear();
                    scene->getItem_robot_model()->robot_circle_model_data_.driverID.append(1);
                    scene->getItem_robot_model()->robot_circle_model_data_.driverID.append(2);
                    scene->getItem_robot_model()->robot_circle_model_data_.inverse_.clear();
                    scene->getItem_robot_model()->robot_circle_model_data_.inverse_.append(false);
                    scene->getItem_robot_model()->robot_circle_model_data_.inverse_.append(false);
                    scene->getItem_robot_model()->inverse_list_.clear();
                    scene->getItem_robot_model()->inverse_list_.append(false);
                    scene->getItem_robot_model()->inverse_list_.append(false);
                    break;
                case DIFFERENTIAL_FOURWHEEL://差动四轮

                    //model切换记录属性参数
                    scene->getItem_robot_model()->robot_circle_model_data_.chasisModel.DiffFourWheel.a_axis_distance
                            = temp->roboData_.chasisModel.DiffTwoWheel.a_axis_distance;
                    scene->getItem_robot_model()->robot_circle_model_data_.chasisModel.DiffFourWheel.b_axis_distance
                            = temp->roboData_.chasisModel.DiffFourWheel.b_axis_distance;
                    //设置电机正反转及编号默认值（由于不确定电机位置引导中没有进行引导配置）
                    scene->getItem_robot_model()->robot_circle_model_data_.driverID.clear();
                    scene->getItem_robot_model()->robot_circle_model_data_.driverID.append(1);
                    scene->getItem_robot_model()->robot_circle_model_data_.driverID.append(2);
                    scene->getItem_robot_model()->robot_circle_model_data_.driverID.append(3);
                    scene->getItem_robot_model()->robot_circle_model_data_.driverID.append(4);
                    scene->getItem_robot_model()->robot_circle_model_data_.inverse_.clear();
                    scene->getItem_robot_model()->robot_circle_model_data_.inverse_.append(false);
                    scene->getItem_robot_model()->robot_circle_model_data_.inverse_.append(false);
                    scene->getItem_robot_model()->robot_circle_model_data_.inverse_.append(false);
                    scene->getItem_robot_model()->robot_circle_model_data_.inverse_.append(false);
                    scene->getItem_robot_model()->inverse_list_.clear();
                    scene->getItem_robot_model()->inverse_list_.append(false);
                    scene->getItem_robot_model()->inverse_list_.append(false);
                    scene->getItem_robot_model()->inverse_list_.append(false);
                    scene->getItem_robot_model()->inverse_list_.append(false);
                    break;
                case ALLDIRECTION_THREEWHEEL://全向三轮
                    //model切换记录属性参数
                    scene->getItem_robot_model()->robot_circle_model_data_.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle
                            = temp->roboData_.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle ;

                    scene->getItem_robot_model()->robot_circle_model_data_.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance
                            = temp->roboData_.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance;
                    //设置电机正反转及编号默认值（由于不确定电机位置引导中没有进行引导配置）
                    scene->getItem_robot_model()->robot_circle_model_data_.driverID.clear();
                    scene->getItem_robot_model()->robot_circle_model_data_.driverID.append(1);
                    scene->getItem_robot_model()->robot_circle_model_data_.driverID.append(2);
                    scene->getItem_robot_model()->robot_circle_model_data_.driverID.append(3);
                    scene->getItem_robot_model()->robot_circle_model_data_.inverse_.clear();
                    scene->getItem_robot_model()->robot_circle_model_data_.inverse_.append(false);
                    scene->getItem_robot_model()->robot_circle_model_data_.inverse_.append(false);
                    scene->getItem_robot_model()->robot_circle_model_data_.inverse_.append(false);
                    scene->getItem_robot_model()->inverse_list_.clear();
                    scene->getItem_robot_model()->inverse_list_.append(false);
                    scene->getItem_robot_model()->inverse_list_.append(false);
                    scene->getItem_robot_model()->inverse_list_.append(false);
                    break;
                default:
                    break;
                }
            }
            scene->getItem_robot_model()->update_model();
        }
            break;
        case GUID_DI:
        {
            //添加DI设备
            GuidDISetting* tempSetting =  dynamic_cast<GuidDISetting*>(widlist->at(i));

            int diNum = tempSetting->numSetting_->getNum();
            int crashDiNum = tempSetting->numSetting_crashDI_->getNum();
            ItemDI::count = 0;//静态成员变量
            ItemCrashDI::count = 0;
            for(int i = 0; i< diNum;i++)
            {
                ItemDI* tempDi = new ItemDI(scene);
                tempDi->update_item();
                scene->get_Di_list()->append(tempDi);
                scene->addItem(tempDi);
                tempDi->setPos(80,-280+15*i);//-145
                tempDi->setOriPos(QPointF(80,-280+15*i));
                tempDi->isCreatedByLoading_ = true;
                tempDi->data_di_.x_ = 80/100.;
                tempDi->data_di_.y_ = (-280+15*i)/100.;
                tempDi->data_di_.id_ = i;
                tempDi->update_item();
            }

            for(int i = 0; i < crashDiNum; i++)
            {
                ItemCrashDI* tempCrashDi = new ItemCrashDI(scene);
                scene->get_crashDi_list()->append(tempCrashDi);
                scene->addItem(tempCrashDi);
                tempCrashDi->setPos(150,-240+100*(i/*+diNum*/));
                tempCrashDi->setOriPos(QPointF(150,-240+100*(i/*+diNum*/)));
                tempCrashDi->isCreatedByLoading_ = true;
                tempCrashDi->data_crashDi_.id_ = diNum+i;

                //初始化crashPos oriPos
                for(int i = 0; i< tempCrashDi->getLine_pos_list()->size();i++)
                {
                    tempCrashDi->getLine_pos_list()->at(i)->setOriPos(
                                tempCrashDi->getLine_pos_list()->at(i)->pos());
                }
            }
        }
            break;
        case GUID_DO:
        {
            //添加DO设备
            GuidDoSetting* tempSetting =  dynamic_cast<GuidDoSetting*>(widlist->at(i));
            ItemDO::count = 0;
            int num = tempSetting->numSetting_->getNum();
            for(int i = 0; i< num;i++)
            {
                ItemDO* tempDo = new ItemDO(scene);
                scene->get_Do_list()->append(tempDo);
                scene->addItem(tempDo);
                tempDo->setPos(50,-280+15*i);
                tempDo->setOriPos(QPointF(50,-280+15*i));
                tempDo->isCreatedByLoading_ = true;
                tempDo->data_do_.x_ = 50/100.;
                tempDo->data_do_.y_ = (-280+15*i)/100.;
                tempDo->data_do_.id_ = i;
                tempDo->update_item();
            }

        }
            break;
        case GUID_LASER:
        {
            //添加激光设备
            GuidLaserSetting* tempSetting = dynamic_cast<GuidLaserSetting*>(widlist->at(i));
            int num = tempSetting->numSetting_->getNum();
            ItemLaser::count = 0;
            //从1开始遍历，0是数量设置窗口
            for(int i = 0; i< num;i++)
            {
                ItemLaser* tempLaser = new ItemLaser(scene);
                scene->get_Laserlist()->append(tempLaser);
                scene->addItem(tempLaser);
                tempLaser->setPos(QPointF(20,-280+25*i));
                tempLaser->setOriPos(QPointF(20,-280+25*i));
                tempLaser->isCreatedByLoading_ = true;
                tempLaser->data_laser_.x_  = 20/100.;
                tempLaser->data_laser_.y_ = (-280+25*i)/100.;
                tempLaser->data_laser_.id_ = i;

            }
        }
            break;
        case GUID_MAGENATICSENSOR:
        {
            //添加磁传感器设备
            GuidMagneticSensorSetting* tempSetting =  dynamic_cast<GuidMagneticSensorSetting*>(widlist->at(i));
            int num = tempSetting->numSetting_->getNum();
            ItemMagneticSensor::count = 0;
            //从1开始遍历，0是数量设置窗口
            for(int i = 0; i <num;i++)
            {
                ItemMagneticSensor* tempMagneScensor = new ItemMagneticSensor(scene);
                scene->get_magneticSensor_list()->append(tempMagneScensor);
                scene->addItem(tempMagneScensor);
                tempMagneScensor->setPos(QPointF(-10,-280+15*i));
                tempMagneScensor->setOriPos(QPointF(-10,-280+15*i));
                tempMagneScensor->isCreatedByLoading_ = true;
                tempMagneScensor->data_magnetic_.x_ = -10/100.;
                tempMagneScensor->data_magnetic_.y_ = (-280+15*i)/100.;
                tempMagneScensor->data_magnetic_.id_ = i;
                tempMagneScensor->update_item();
            }
        }
            break;
        case GUID_RFIDSENSOR:
        {
            //添加RFID传感器
            GuidRFIDSetting* temp = dynamic_cast<GuidRFIDSetting*>(widlist->at(i));
            if(temp->getConfigure())
            {
                ItemRFIDSensor* tempRFID = new ItemRFIDSensor(scene);
                scene->get_RFID_list()->append(tempRFID);
                scene->addItem(tempRFID);
                tempRFID->setPos(QPointF(-30,-280));
                tempRFID->setOriPos(QPointF(-30,-280));
                tempRFID->data_RFID_.x_ = -30/100.;
                tempRFID->data_RFID_.y_ = -280/100.;
                tempRFID->data_RFID_.z_ = 0;
            }
            widget_basic_setting_->getRFID()->blockSignals(true);
            int idx = widget_basic_setting_->getRFID()->findData(temp->basicData_.RFID_);
            widget_basic_setting_->getRFID()->setCurrentIndex(idx);
            widget_basic_setting_->getRFID()->setOri_index(idx);
            widget_basic_setting_->getRFID()->blockSignals(false);
            widget_basic_setting_->model_base_data_.RFID_ = temp->basicData_.RFID_;
            if(idx==0)
            {
                widget_basic_setting_->getMagneticSensorType()->blockSignals(true);
                widget_basic_setting_->getMagneticSensorType()->setCurrentIndex(0);
                widget_basic_setting_->getMagneticSensorType()->setOri_index(0);
                widget_basic_setting_->getMagneticSensorType()->blockSignals(false);
                widget_basic_setting_->model_base_data_.magnetic_sensor_ = widget_basic_setting_->getMagneticSensorType()->currentData().toInt();
            }
        }
            break;
        case GUID_ULTRASONIC:
        {
            //添加超声设备
            GuidUltrasonicSetting*  tempSetting = dynamic_cast<GuidUltrasonicSetting*>(widlist->at(i));
            int num = tempSetting->numSetting_->getNum();
            ItemUltrasonic::count = 0;
            //从1开始遍历，0是数量设置窗口
            for(int i = 0 ; i < num;i++)
            {
                ItemUltrasonic* tempUltrasonic = new ItemUltrasonic(scene);
                scene->get_ultrasonic_list()->append(tempUltrasonic);
                scene->addItem(tempUltrasonic);
                tempUltrasonic->setPos(QPointF(-100,-280+15*i));
                tempUltrasonic->setOriPos(QPointF(-100,-280+15*i));
                tempUltrasonic->isCreatedByLoading_ = true;
                tempUltrasonic->data_ultrasonic_.x_ = -100/100.;
                tempUltrasonic->data_ultrasonic_.y_ = (-280+15*i)/100.;
                tempUltrasonic->data_ultrasonic_.id_ = i;
                tempUltrasonic->update_item();
            }
        }
            break;
        case GUID_FINISHED:
        {

        }
            break;
        default:
            break;
        }

    }
    //初始为已保存状态
    m_act_saveModel->setEnabled(false);
}

void SCRobotFileModelWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
}
/** 更新同步地图数据提示窗口
 * @brief SCRobotFileModelWidget::updateToolTipWidget
 */
void SCRobotFileModelWidget::updateToolTipWidget()
{
    if(RobotManager::init()->isConnected()){ //已连接机器人

        //        QString keyID = PublicClass::init()->_currentKeyID;

    }
}
void SCRobotFileModelWidget::setUpandDownloadBtnEnable(bool isConnected)
{
    //1当前文件为空
    if(getCurrentFilePath().isEmpty())
    {
        //如果当前是链接状态允许下载不允许上传
        if(isConnected)
        {
            m_act_upload->setEnabled(false);
        }
        else
        {
            m_act_upload->setEnabled(false);
        }
    }
    else
    {
        //当前是连接状态
        if(isConnected)
        {
            m_act_upload->setEnabled(true);
        }
        else
        {
            m_act_upload->setEnabled(false);
        }
    }

}

void SCRobotFileModelWidget::setCurrentFilePath(QString path)
{
    m_currentFilePath = path;
}

QString SCRobotFileModelWidget::getCurrentFilePath()
{
    return m_currentFilePath;
}

/** 状态变化
 * @brief RobotControlWindow::slotRoTcpManagerStatus
 * @param ip
 * @param port
 * @param commandStatus
 *
 */
void SCRobotFileModelWidget::slotRoTcpManagerStatus(const QString &ip,RobotInterface::RobotCellStatus status)
{
    switch (status) {
    case RobotInterface::E_CellStatus_OffLine://离线
    case RobotInterface::E_CellStatus_OLDisConnection://在线未连接
    case RobotInterface::E_CellStatus_OLConnected://已连接
    {
        init();
    }
        break;
    default:
        break;
    }
}

void SCRobotFileModelWidget::slotStatusTcpReceiveAll(const QString &ip, const QString &sCD, const QVariant& jsonData, int number, bool isError)
{
    if(number != E_SCRobotFileModelWidget) return;
    if(isError){
        slotMessage(QString("%1").arg(jsonData.toString()),1);
        return;
    }
    if(sCD == "robot_status_model_req"){
        slotMessage(QString("%1").arg(tr("Download success")));
    }
}
void SCRobotFileModelWidget::slotConfigTcpReceiveAll(const QString &ip, const QString &sCD, const QVariant &jsonData, int number, bool isError)
{
    if(number != E_SCRobotFileModelWidget) return;

    if(sCD == "robot_config_model_req"){
        if(!isError){
            slotMessage(QString("%1").arg(tr("Upload success")));
        }else{
            slotMessage(QString("%1").arg(tr("Upload failed")));
        }
    }else if(isError){
        slotMessage(QString("%1").arg(jsonData.toString()),1);
    }
}

///机器人模型上传
void SCRobotFileModelWidget::slotUpload()
{

    //先保存再上传
    QString filePath = getCurrentFilePath();

    if(filePath.isEmpty())
    {
        return;
    }

    //    if(!_isSaved)//如果没有保存
    //    {

    pEditRobotModelWidget->saveFile(filePath);//进行保存

    if(pEditRobotModelWidget->getIsSaveSuccess())//如果保存 成功
    {
        SCDebug<<"save success==============";
        _isSaved = true;//已保存
        m_act_saveModel->setEnabled(false);
    }
    else
    {
        _isSaved = false;
        return;
        SCDebug<<"save failed==============";
    }

    //    }


    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        slotMessage(QString(tr("OPen model failed:[%1]")).arg(file.fileName()),1);
        return;
    }
    QByteArray b = file.readAll();
    SCDebug<<"file size is "<<b.size();
    file.close();
    slotMessage(QString(tr("Uploading[%1]")).arg(file.fileName()),-1,true);
    RobotManager::init()->sendData(RobotInterface::E_ConfigTcp,b,"robot_config_model_req","",E_SCRobotFileModelWidget);
}
///机器人模型下载
void SCRobotFileModelWidget::slotDownload()
{
    RobotManager::init()->sendData(RobotInterface::E_StatusTcp,"","robot_status_model_req","",E_SCRobotFileModelWidget);
}
///TODO ---------------------------------------------------------
void SCRobotFileModelWidget::slotNewModel()
{

    if(!_isSaved)
    {
        QStringList list;
        list<<tr("Save and Close")<<tr("Do not Save")<<tr("Cancel");
        int rb = UiClass::init()->showDialog(tr("Do you want to close the current model ?"),tr("Information"),0,list);
        SCDebug<<"rb is "<<rb;
        if(rb ==0)
        {
            slotSaveModel();
        }
        else if(rb == 2 || -1 == rb)
        {
            return ;
        }
    }
#if 0
    QStringList list;
    list<<("确定")<<("取消");
    int rb = UiClass::init()->showDialog(("是否要需要新建引导？"),("提示"),0,list);
    SCDebug<<"rb is "<<rb;
    if(rb == 0)
    {
        _guidenceWidget->ShowCurrentGuidWid(0);
        _guidenceWidget->SetGuidInstruction();
        if(_guidenceWidget->exec()==QDialog::Accepted)
        {
        }
        else
        {
        }
    }
    else
    {
        //加载模板文件列表
        _modelchoseDlg->loadMaps(DEFAULTCONFIGMODEL_PATH);

        if(_modelchoseDlg->exec()==QDialog::Accepted)
        {
            SCDebug<<"confirm";
        }
        else
        {
        }

    }
#endif
}

void SCRobotFileModelWidget::slotLoadModel()
{

    GlobalVariant::getInst()->flag = true;
    //加载模型操作
    QString modelDir = PublicClass::init()->getModelsPath();
    QString filePath = QFileDialog::getOpenFileName(this, tr("model"), modelDir, ("model files(*.model)"));
    if(filePath.isEmpty()){
        return;
    }
    //----
    if(!_isSaved)
    {
        QStringList list;
        list<<tr("Yes")<<tr("No");
        int rb = UiClass::init()->showDialog(tr("Do you want to save the current model ?"),tr("Information"),0,list);
        qDebug()<<"rb is ------------"<<rb;
        if(rb ==0)
        {
            slotSaveModel();
            if(!_isSaved) //如果没有保存成功则返回
                return;
        }
        else if(rb == 1 || rb==-1)
        {

        }

    }

    if(loadFile(filePath))
    {
        SCDebug<<"load filePath is "<<filePath;
        setCurrentFilePath(filePath);//记录当前模型路径
        set_file_existed(true);
        //对basicWidget宽度自适应
        fitPropertyWidget();
        //加载成功后设置当前文件模型类型
        _filetype = FILE_MODEL;
    }
    else
    {
        if(getCurrentFilePath().isEmpty())
        {
            set_file_existed(false);
        }
    }
    setUpandDownloadBtnEnable(RobotManager::init()->isConnected());
}

void SCRobotFileModelWidget::slotSaveModel()
{
    if(m_currentFilePath.isEmpty())
    {
        return;
    }
    if(!_isSaved)//没有保存
    {
        QString filePath = pEditRobotModelWidget->filePath();
        SCDebug<<"save ==================path is "<<filePath;
        pEditRobotModelWidget->saveFile(filePath,0,_isGeneratedByGuidence);//进行保存

        if(pEditRobotModelWidget->getIsSaveSuccess())//如果保存 成功
        {
            SCDebug<<"save success==============";
            _isSaved = true;//已保存
            m_act_saveModel->setEnabled(false);
        }
        else
        {
            _isSaved = false;
            m_act_saveModel->setEnabled(true);
            SCDebug<<"save failed==============";
        }
    }
    else
    {
        m_act_saveModel->setEnabled(false);
        _isSaved = true;
        return;
    }
}

void SCRobotFileModelWidget::slotSaveModelAs()
{
    if(!pEditRobotModelWidget)return;
    pEditRobotModelWidget->saveAsOtherFile(_isGeneratedByGuidence);
}

void SCRobotFileModelWidget::slotSaveAsTemp()
{
    if(!pEditRobotModelWidget)return;

    if(!pEditRobotModelWidget->isAllowSave(_isGeneratedByGuidence))
    {
        return ;
    }
    _saveTempWid = new DialogSaveAsTemplate;
    _saveTempWid->setWindowTitle(tr("Save Template"));
    connect(_saveTempWid,SIGNAL(sig_confirm(QString)),SLOT(slot_saveAsTempConfirm(QString)));
    connect(_saveTempWid,SIGNAL(sig_cancel()),SLOT(slot_saveASTempCancel()));
    _saveTempWid->setAttribute(Qt::WA_DeleteOnClose, true);
    _saveTempWid->setModal(true);
    _saveTempWid->show();
}

void SCRobotFileModelWidget::slotOpenTemp()
{
    if(!isAllowExec())
        return;
    //加载模板文件列表
    _modelchoseDlg->loadMaps(DEFAULTCONFIGMODEL_PATH);
    _modelchoseDlg->setModal(true);
    _modelchoseDlg->show();

    return;

    GlobalVariant::getInst()->flag = true;
    //加载模型时设置保存按钮状态
    _isSaved = false;
    //加载模型操作
    QString modelDir = PublicClass::init()->getModelsPath();
    QString filePath = QFileDialog::getOpenFileName(this, tr("temp"), modelDir, ("model files(*.tmp)"));
    if(filePath.isEmpty()){
        return;
    }

    if(loadTemplate(filePath))
    {
        SCDebug<<"load filePath is "<<filePath;
        setCurrentFilePath(filePath);//记录当前模型路径
        set_file_existed(true);
        //对basicWidget宽度自适应
        fitPropertyWidget();
        //设置当前文件类型
        _filetype = FILE_TEMP;
    }
    else
    {
        if(getCurrentFilePath().isEmpty())
        {
            set_file_existed(false);
        }
    }
    setUpandDownloadBtnEnable(RobotManager::init()->isConnected());

}

void SCRobotFileModelWidget::slotClose()
{
    if(!_isSaved)
    {
        QStringList list;
        list<<tr("Save and Close")<<tr("Do not Save")<<tr("Cancel");
        int rb = UiClass::init()->showDialog(tr("Do you want to close the current model ?"),tr("Information"),0,list);
        SCDebug<<"rb is "<<rb;
        if(rb ==0)
        {
            pEditRobotModelWidget->saveFile(pEditRobotModelWidget->filePath());
        }else if(rb == 2 || -1 == rb)
        {
            return;
        }
    }

    //判断
    if(pEditRobotModelWidget)
    {
        delete pEditRobotModelWidget;
        pEditRobotModelWidget = NULL;
    }
    //清空undostack
    if(m_undoStack)
    {
        m_undoStack->clear();
    }
    set_file_existed(false);
    //关闭模型文件时 _isSaved标志置为false
    _isSaved = false;
    setCurrentFilePath("");

    if(RobotManager::init()->isConnected()){
        //判断连接状态-是否可上传下载
        setUpandDownloadBtnEnable(true);
    }else{
        //判断连接状态-是否可上传下载
        setUpandDownloadBtnEnable(false);
    }
}

void SCRobotFileModelWidget::slotEnableSaveMode()
{
    _isSaved = false;
    m_act_saveModel->setEnabled(true);
}

void SCRobotFileModelWidget::slot_Prompt_close()
{
    _maskWid->hide();
}

bool SCRobotFileModelWidget::loadFile(const QString&filePath)
{
    QFileInfo info(filePath);

    if(!info.fileName().contains(".model")){
        slotMessage(QString(tr("Invalid Model file: %1")).arg(info.fileName()),1);
    }else{
        bool isReasonable = check_modelfile_reasonable(filePath);
        if(!isReasonable)
        {
            slotMessage(QString(tr("Model file chassis does not exist")),1);
            return false;
        }
        if(pEditRobotModelWidget)
        {
            delete pEditRobotModelWidget;//先释放当前的模型
        }
        pEditRobotModelWidget = new EditRobotModelWidget(this);
        ui->vlayout_model->addWidget(pEditRobotModelWidget);
        pEditRobotModelWidget->setFilePath(filePath);

        bool res =  pEditRobotModelWidget->loadFile(filePath);
        if(res)
        {
        }
        else
        {
        }
        int index = filePath.lastIndexOf("/");
        QString name = filePath.right(filePath.length()-index-1);
        pEditRobotModelWidget->setFileName(name);
        m_undoStack->clear();//加载模型之前清空已入栈操作
    }
    return true;
}

bool SCRobotFileModelWidget::loadTemplate(const QString &filePath)
{
    QFileInfo info(filePath);

    if(!info.fileName().contains(".tmp")){
        slotMessage(QString(tr("Invalid Model file: %1")).arg(info.fileName()),1);
    }else{
        bool isReasonable = check_modelfile_reasonable(filePath);
        if(!isReasonable)
        {
            slotMessage(QString(tr("The model file format is incorrect: %1")).arg(info.fileName()),1);
            return false;
        }
        if(pEditRobotModelWidget)
        {
            delete pEditRobotModelWidget;//先释放当前的模型
        }
        pEditRobotModelWidget = new EditRobotModelWidget(this);

        ui->vlayout_model->addWidget(pEditRobotModelWidget);
        pEditRobotModelWidget->setFilePath(filePath);

        pEditRobotModelWidget->loadFile(filePath);
        int index = filePath.lastIndexOf("/");
        QString name = filePath.right(filePath.length()-index-1);
        pEditRobotModelWidget->setFileName(name);
        //新建模型文件时将undoStack 传递到子窗口
        m_undoStack->clear();//加载模型之前清空已入栈操作
    }
    return true;
}

void SCRobotFileModelWidget::set_file_existed(const bool &is_exsit)
{
    m_act_saveModelAs->setEnabled(is_exsit);
    m_act_saveModel->setEnabled(is_exsit);
    m_act_saveAsTemplate->setEnabled(is_exsit);
}

void SCRobotFileModelWidget::setUndoStack(QUndoStack *stack)
{
    m_undoStack = stack;
}

QUndoStack *SCRobotFileModelWidget::getUndoStack()
{
    return m_undoStack;
}

EditRobotModelWidget *SCRobotFileModelWidget::getEidtRobotModelWidget()
{
    return pEditRobotModelWidget;
}

ConfigureData SCRobotFileModelWidget::readConfigure_model(const QString &filePath)
{
    QString val;

    QFile file;

    file.setFileName(filePath);

    file.open(QIODevice::ReadOnly);

    val = file.readAll();

    file.close();

    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject obj = d.object();

    QJsonValue models_value = obj.value(QString("defaultModels"));
    QJsonObject models_obj = models_value.toObject();

    QJsonValue defindx = models_obj["defaultIndex"];

    QJsonArray typeList = models_obj["typeList"].toArray();

    ConfigureData  configData ;

    configData.defaultidx = defindx.toInt();

    for(int i = 0; i < typeList.size();i++)
    {
        QJsonObject tempObj = typeList.at(i).toObject();

        QJsonValue tempIdex = tempObj["index"];
        QJsonValue tempDesc = tempObj["desc"];

        ItemData  tempData;
        tempData.index = tempIdex.toInt();
        tempData.describtion = tempDesc.toString();
        configData.dataList.push_back(tempData);
    }

    return configData;
}

bool SCRobotFileModelWidget::check_modelfile_reasonable(const QString filePath)
{
    //获取标准模板keys
    standardModelKeys_list.clear();
    getModelKeys(COMPAREMODEL_PATH,standardModelKeys_list);
    //获取当前模型keys
    currentModelKeys_list.clear();
    getLoadModelKeys(filePath,currentModelKeys_list);
    //获取多余keys
    getExtralKeys(currentModelKeys_list,standardModelKeys_list);
    _extralInforString =tr("The following are the exception fields for the current model file:\n");
    int  basicExtralLength = _extralInforString.size();
    if(extralKeys_list.size()>0)
    {
        //拼接异常字段
        for(int er = 0; er<extralKeys_list.size();er++)
        {
            _extralInforString += extralKeys_list.at(er)+"\n";
        }

    }
    else
    {
        //如果无异常字段则清空字符串
        _extralInforString = "";
    }
    model_containKeyMap.clear();

    QString val;
    QFile file;
    file.setFileName(filePath);
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    val = file.readAll();
    file.close();

    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject obj = d.object();

    model_containKeyMap.insert("obj_model",obj.contains("model"));
    model_containKeyMap.insert("chassis",obj.contains("chassis"));
    model_containKeyMap.insert("laser",obj.contains("laser"));
    model_containKeyMap.insert("DI",obj.contains("DI"));
    model_containKeyMap.insert("DO",obj.contains("DO"));
    model_containKeyMap.insert("ultrasonic",obj.contains("ultrasonic"));
    model_containKeyMap.insert("magneticSensor",obj.contains("magneticSensor"));
    model_containKeyMap.insert("RFID",obj.contains("RFID"));
    model_containKeyMap.insert("vision",obj.contains("vision"));

    QJsonObject  chassis_obj = obj["chassis"].toObject();
    model_containKeyMap.insert("chassis_shape",chassis_obj.contains("shape"));
    model_containKeyMap.insert("chassis_gyro",chassis_obj.contains("gyro"));
    model_containKeyMap.insert("chassis_brake",chassis_obj.contains("brake"));
    model_containKeyMap.insert("chassis_autoBrake",chassis_obj.contains("autoBrake"));
    model_containKeyMap.insert("chassis_batteryInfo",chassis_obj.contains("batteryInfo"));
    model_containKeyMap.insert("chassis_LED",chassis_obj.contains("LED"));
    model_containKeyMap.insert("chassis_mode",chassis_obj.contains("mode"));
    model_containKeyMap.insert("chassis_mode_param",chassis_obj.contains("mode_param"));

    QJsonObject mode_param_obj = chassis_obj["mode_param"].toObject();
    model_containKeyMap.insert("mode_param_type",mode_param_obj.contains("type"));
    model_containKeyMap.insert("mode_param_wheelRadius",mode_param_obj.contains("wheelRadius"));
    model_containKeyMap.insert("mode_param_reductionRatio",mode_param_obj.contains("reductionRatio"));
    model_containKeyMap.insert("mode_param_encoderLine",mode_param_obj.contains("encoderLine"));
    model_containKeyMap.insert("mode_param_maxMotorRpm",mode_param_obj.contains("maxMotorRpm"));
    model_containKeyMap.insert("mode_param_inverse",mode_param_obj.contains("inverse"));
    model_containKeyMap.insert("mode_param_driver",mode_param_obj.contains("driver"));
    model_containKeyMap.insert("mode_param_driverBrand",mode_param_obj.contains("driverBrand"));
    model_containKeyMap.insert("mode_param_driverID",mode_param_obj.contains("driverID"));

    QJsonValue shape_value = chassis_obj["shape"];
    int shape = shape_value.toInt();
    if(1 == shape)//矩形轮廓
    {
        model_containKeyMap.insert("chassis_width",chassis_obj.contains("width"));
        model_containKeyMap.insert("chassis_head",chassis_obj.contains("head"));
        model_containKeyMap.insert("chassis_tail",chassis_obj.contains("tail"));
        QJsonValue type_value = mode_param_obj["type"];
        int type = type_value.toInt();

        if(10 == type)//差动两轮
        {
            model_containKeyMap.insert("mode_param_A",mode_param_obj.contains("A"));
        }
        else if(11 == type)//差动四轮
        {
            model_containKeyMap.insert("mode_param_A",mode_param_obj.contains("A"));
            model_containKeyMap.insert("mode_param_B",mode_param_obj.contains("B"));
        }
        else if(20 == type)//全向三轮
        {
            model_containKeyMap.insert("mode_param_A",mode_param_obj.contains("A"));
            model_containKeyMap.insert("mode_param_theta",mode_param_obj.contains("theta"));
        }
        else if(30 == type)//麦克纳姆四轮
        {
            model_containKeyMap.insert("mode_param_A",mode_param_obj.contains("A"));
            model_containKeyMap.insert("mode_param_B",mode_param_obj.contains("B"));
        }
        else //单舵轮
        {
            model_containKeyMap.insert("mode_param_A",mode_param_obj.contains("A"));
            model_containKeyMap.insert("mode_param_B",mode_param_obj.contains("B"));
            model_containKeyMap.insert("mode_param_absEncoder",mode_param_obj.contains("absEncoder"));
            model_containKeyMap.insert("mode_param_absEncoderRange",mode_param_obj.contains("absEncoderRange"));
            model_containKeyMap.insert("mode_param_steerAngle",mode_param_obj.contains("steerAngle"));
        }
    }
    else if(2 == shape)//圆形轮廓
    {
        model_containKeyMap.insert("chassis_radius",chassis_obj.contains("radius"));

        QJsonValue type_value = mode_param_obj["type"];
        int type = type_value.toInt();
        if(10 == type)//差动两轮
        {
            model_containKeyMap.insert("mode_param_A",mode_param_obj.contains("A"));
        }
        else if(11 == type)//差动四轮
        {
            model_containKeyMap.insert("mode_param_A",mode_param_obj.contains("A"));
            model_containKeyMap.insert("mode_param_B",mode_param_obj.contains("B"));
        }
        else if(20 == type)//全向三轮
        {
            model_containKeyMap.insert("mode_param_A",mode_param_obj.contains("A"));
            model_containKeyMap.insert("mode_param_theta",mode_param_obj.contains("theta"));
        }
    }
#if 0  //多边形已去除
    // 减速多边形
    QJsonObject decelerateArea_obj = obj["decelerateArea"].toObject();
    model_containKeyMap.insert("decelerateArea_num",decelerateArea_obj.contains("num"));

    QJsonArray decelerateArea_index_array = decelerateArea_obj["index"].toArray();
    model_containKeyMap.insert("decelerateArea_index",decelerateArea_obj.contains("index"));
    if(decelerateArea_obj.contains("index"))
    {
        for(int j = 0; j < decelerateArea_index_array.size();j++)
        {
            QJsonObject indexObj = decelerateArea_index_array.at(j).toObject();
            model_containKeyMap.insert("decelerateArea_verteNum",indexObj.contains("vertexNum"));
            model_containKeyMap.insert("decelerateArea_maxSpeed",indexObj.contains("maxSpeed"));

            QJsonArray vertex_array = indexObj["vertex"].toArray();
            model_containKeyMap.insert("decelerateArea_vertex",indexObj.contains("vertex"));

            if(indexObj.contains("vertex"))
            {
                for(int i = 0; i<vertex_array.size();i++)
                {
                    QJsonObject tempObj = vertex_array.at(i).toObject();
                    //为防止map相同的key 会发生覆盖在此将每个对象的x,y 加上编号
                    model_containKeyMap.insert("decelerateArea_x"+QString("%1").arg(i),tempObj.contains("x"));
                    model_containKeyMap.insert("decelerateArea_y"+QString("%1").arg(i),tempObj.contains("y"));
                }
            }
        }
    }

    //停障多边形
    QJsonObject stopArea_obj  = obj["stopArea"].toObject();

    model_containKeyMap.insert("stopArea_num",stopArea_obj.contains("num"));
    model_containKeyMap.insert("stopArea_index",stopArea_obj.contains("index"));
    QJsonArray stopArea_index_array = stopArea_obj["index"].toArray();

    model_containKeyMap.insert("stopArea_index",stopArea_obj.contains("index"));
    if(stopArea_obj.contains("index"))
    {
        for(int j = 0; j < stopArea_index_array.size();j++)
        {
            QJsonObject indexObj = stopArea_index_array.at(j).toObject();
            model_containKeyMap.insert("stopArea_vertexNum",indexObj.contains("vertexNum"));
            model_containKeyMap.insert("stopArea_vertex",indexObj.contains("vertex"));
            if(indexObj.contains("vertex"))
            {
                QJsonArray stopAreavertex_array = indexObj["vertex"].toArray();
                for(int i = 0; i<stopAreavertex_array.size();i++)
                {
                    QJsonObject tempObj = stopAreavertex_array.at(i).toObject();
                    model_containKeyMap.insert("stopAreaArea_x"+QString("%1").arg(i),tempObj.contains("x"));
                    model_containKeyMap.insert("stopAreaArea_y"+QString("%1").arg(i),tempObj.contains("y"));
                }
            }
        }

    }
    //安全多边形
    QJsonObject safeArea_obj = obj["safeArea"].toObject();
    model_containKeyMap.insert("safeArea_num",stopArea_obj.contains("num"));
    model_containKeyMap.insert("safeArea_index",stopArea_obj.contains("index"));

    QJsonArray safeArea_index_array = safeArea_obj["index"].toArray();
    model_containKeyMap.insert("safeArea_index",safeArea_obj.contains("index"));
    if(safeArea_obj.contains("index"))
    {
        for(int j = 0; j < safeArea_index_array.size();j++)
        {
            QJsonObject indexObj = safeArea_index_array.at(j).toObject();
            model_containKeyMap.insert("stopArea_vertexNum",indexObj.contains("vertexNum"));
            model_containKeyMap.insert("stopArea_vertex",indexObj.contains("vertex"));
            if(indexObj.contains("vertex"))
            {
                QJsonArray safeAreavertex_array = indexObj["vertex"].toArray();
                for(int i = 0; i<safeAreavertex_array.size();i++)
                {
                    QJsonObject tempObj = safeAreavertex_array.at(i).toObject();
                    model_containKeyMap.insert("stopAreaArea_x"+QString("%1").arg(i),tempObj.contains("x"));
                    model_containKeyMap.insert("stopAreaArea_y"+QString("%1").arg(i),tempObj.contains("y"));
                }
            }
        }

    }

#endif
    //激光

    QJsonObject laser_obj = obj["laser"].toObject();
    model_containKeyMap.insert("laser_num",laser_obj.contains("num"));
    model_containKeyMap.insert("laser_index",laser_obj.contains("index"));

    if(laser_obj.contains("index"))
    {
        QJsonArray laser_indexArray = laser_obj["index"].toArray();
        for(int j = 0; j < laser_indexArray.size();j++)
        {
            QJsonObject tempObj = laser_indexArray.at(j).toObject();
            model_containKeyMap.insert("laser_ip",tempObj.contains("ip"));
            model_containKeyMap.insert("laser_maxAngle",tempObj.contains("maxAngle"));
            model_containKeyMap.insert("laser_minAngle",tempObj.contains("minAngle"));
            model_containKeyMap.insert("laser_r",tempObj.contains("r"));
            model_containKeyMap.insert("laser_upside",tempObj.contains("upside"));
            model_containKeyMap.insert("laser_useForLocalization",tempObj.contains("useForLocalization"));
            model_containKeyMap.insert("laser_x",tempObj.contains("x"));
            model_containKeyMap.insert("laser_y",tempObj.contains("y"));
            model_containKeyMap.insert("laser_z",tempObj.contains("z"));
        }
    }
    //DI
    QJsonObject DI_obj = obj["DI"].toObject();

    model_containKeyMap.insert("DI_maxNum",DI_obj.contains("maxDINum"));
    model_containKeyMap.insert("DI_Num",DI_obj.contains("num"));
    model_containKeyMap.insert("DI_index",DI_obj.contains("index"));
    //清空id容器
    DI_id_list.clear();
    //清空提示
    _repeatPromtList.clear();
    if(DI_obj.contains("index"))
    {
        QJsonArray di_index_Array = DI_obj["index"].toArray();
        for(int j = 0;j <di_index_Array.size();j++)
        {
            QJsonObject tempDI_obj = di_index_Array.at(j).toObject();

            if(DI_id_list.contains(tempDI_obj["id"].toInt()))
            {
                if(!_repeatPromtList.contains(QString(tr("DI has a duplicate id%1")).arg(tempDI_obj["id"].toInt())))
                    _repeatPromtList.append(QString(tr("DI has a duplicate id%1")).arg(tempDI_obj["id"].toInt()));
            }
            else
            {
                DI_id_list.append(tempDI_obj["id"].toInt());//将id存储到容器中
            }

            model_containKeyMap.insert("DI_id",tempDI_obj.contains("id"));
            model_containKeyMap.insert("DI_func",tempDI_obj.contains("func"));
            model_containKeyMap.insert("DI_type",tempDI_obj.contains("type"));
            model_containKeyMap.insert("DI_inverse",tempDI_obj.contains("inverse"));

            QJsonValue di_type_value = tempDI_obj["type"];
            int diType = di_type_value.toInt();
            if(3 == diType)
            {
                model_containKeyMap.insert("DI_vertex",tempDI_obj.contains("vertex"));
                if(tempDI_obj.contains("vertex"))
                {
                    QJsonArray di_vertex_Array = tempDI_obj["vertex"].toArray();
                    for(int i = 0; i<di_vertex_Array.size();i++)
                    {
                        QJsonObject di_vertex_obj = di_vertex_Array.at(i).toObject();
                        model_containKeyMap.insert("DI_vertex_x"+QString("%1").arg(i),di_vertex_obj.contains("x"));
                        model_containKeyMap.insert("DI_vertex_y"+QString("%1").arg(i),di_vertex_obj.contains("y"));
                    }
                }
            }
            else
            {
                model_containKeyMap.insert("DI_x",tempDI_obj.contains("x"));
                model_containKeyMap.insert("DI_y",tempDI_obj.contains("y"));
                model_containKeyMap.insert("DI_z",tempDI_obj.contains("z"));
                model_containKeyMap.insert("DI_r",tempDI_obj.contains("r"));
                model_containKeyMap.insert("DI_range",tempDI_obj.contains("range"));
            }
        }

    }

    //DO
    QJsonObject DO_obj = obj["DO"].toObject();

    model_containKeyMap.insert("DO_defaultValue",DO_obj.contains("defaultValue"));
    model_containKeyMap.insert("DO_maxNum",DO_obj.contains("maxDONum"));
    model_containKeyMap.insert("DO_num",DO_obj.contains("num"));
    model_containKeyMap.insert("DO_index",DO_obj.contains("index"));
    //清空id容器
    DO_id_list.clear();
    if(DO_obj.contains("index"))
    {
        QJsonArray do_index_Array = DO_obj["index"].toArray();
        for(int j = 0;j <do_index_Array.size();j++)
        {
            QJsonObject tempDO_obj = do_index_Array.at(j).toObject();
            if(DO_id_list.contains(tempDO_obj["id"].toInt()))
            {
                if(!_repeatPromtList.contains(QString(tr("DO has a duplicate id%1")).arg(tempDO_obj["id"].toInt())))
                    _repeatPromtList.append(QString(tr("DO has a duplicate id%1")).arg(tempDO_obj["id"].toInt()));
            }
            else
            {
                DO_id_list.append(tempDO_obj["id"].toInt());//将id存储到容器中
            }

            model_containKeyMap.insert("DO_func",tempDO_obj.contains("func"));
            model_containKeyMap.insert("DO_id",tempDO_obj.contains("id"));
            model_containKeyMap.insert("DO_type",tempDO_obj.contains("type"));
            model_containKeyMap.insert("DO_x",tempDO_obj.contains("x"));
            model_containKeyMap.insert("DO_y",tempDO_obj.contains("y"));
            model_containKeyMap.insert("DO_z",tempDO_obj.contains("z"));
        }

    }
    //超声
    QJsonObject ultrasonic_obj = obj["ultrasonic"].toObject();

    model_containKeyMap.insert("ultrasonic_maxNum",ultrasonic_obj.contains("maxUltrasonicNum"));
    model_containKeyMap.insert("ultrasonic_num",ultrasonic_obj.contains("num"));
    model_containKeyMap.insert("ultrasonic_type",ultrasonic_obj.contains("type"));
    model_containKeyMap.insert("ultrasonic_index",ultrasonic_obj.contains("index"));
    //清空id容器
    Ultrasonic_id_list.clear();
    if(ultrasonic_obj.contains("index"))
    {
        QJsonArray ultrasonic_index_Array = ultrasonic_obj["index"].toArray();
        for(int j = 0;j <ultrasonic_index_Array.size();j++)
        {
            QJsonObject temp_ultrasonic_obj = ultrasonic_index_Array.at(j).toObject();

            if(Ultrasonic_id_list.contains(temp_ultrasonic_obj["id"].toInt()))
            {
                if(!_repeatPromtList.contains(QString(tr("Ultrasonic has a duplicate id%1")).arg(temp_ultrasonic_obj["id"].toInt())))
                    _repeatPromtList.append(QString(tr("Ultrasonic has a duplicate id%1")).arg(temp_ultrasonic_obj["id"].toInt()));
            }
            else
            {
                Ultrasonic_id_list.append(temp_ultrasonic_obj["id"].toInt());
            }

            model_containKeyMap.insert("ultrasonic_func",temp_ultrasonic_obj.contains("func"));
            model_containKeyMap.insert("ultrasonic_id",temp_ultrasonic_obj.contains("id"));//将id存储到容器中
            model_containKeyMap.insert("ultrasonic_x",temp_ultrasonic_obj.contains("x"));
            model_containKeyMap.insert("ultrasonic_maxDist",temp_ultrasonic_obj.contains("maxDist"));
            model_containKeyMap.insert("ultrasonic_minDist",temp_ultrasonic_obj.contains("minDist"));
            model_containKeyMap.insert("ultrasonic_r",temp_ultrasonic_obj.contains("r"));
            model_containKeyMap.insert("ultrasonic_y",temp_ultrasonic_obj.contains("y"));
            model_containKeyMap.insert("ultrasonic_z",temp_ultrasonic_obj.contains("z"));
            model_containKeyMap.insert("ultrasonic_range",temp_ultrasonic_obj.contains("range"));
        }
    }
    //磁传感器
    QJsonObject magneticSensor_obj = obj["magneticSensor"].toObject();
    model_containKeyMap.insert("magneticSensor_type",magneticSensor_obj.contains("type"));
    model_containKeyMap.insert("magneticSensor_num",magneticSensor_obj.contains("num"));
    model_containKeyMap.insert("magneticSensor_index",magneticSensor_obj.contains("index"));
    //清空id容器
    Magnetic_id_list.clear();
    if(magneticSensor_obj.contains("index"))
    {
        QJsonArray magneticSensor_index_Array = magneticSensor_obj["index"].toArray();
        for(int k = 0; k<magneticSensor_index_Array.size();k++)
        {
            QJsonObject magneticSensor_index_obj = magneticSensor_index_Array.at(k).toObject();

            if(Magnetic_id_list.contains(magneticSensor_index_obj["id"].toInt()))
            {
                if(!_repeatPromtList.contains(QString(tr("MagneticSensor has a duplicate id%1")).arg(magneticSensor_index_obj["id"].toInt())))
                    _repeatPromtList.append(QString(tr("MagneticSensor has a duplicate id%1")).arg(magneticSensor_index_obj["id"].toInt()));
            }
            else
            {
                Magnetic_id_list.append(magneticSensor_index_obj["id"].toInt());//将id存储到容器中
            }
            model_containKeyMap.insert("magneticSensor_id",magneticSensor_index_obj.contains("id"));
            model_containKeyMap.insert("magneticSensor_r",magneticSensor_index_obj.contains("r"));
            model_containKeyMap.insert("magneticSensor_resolution",magneticSensor_index_obj.contains("resolution"));
            model_containKeyMap.insert("magneticSensor_step",magneticSensor_index_obj.contains("step"));
            model_containKeyMap.insert("magneticSensor_x",magneticSensor_index_obj.contains("x"));
            model_containKeyMap.insert("magneticSensor_y",magneticSensor_index_obj.contains("y"));
            model_containKeyMap.insert("magneticSensor_z",magneticSensor_index_obj.contains("z"));
        }

    }
    //RFID
    QJsonObject RFID_obj = obj["RFID"].toObject();
    model_containKeyMap.insert("RFID_type",RFID_obj.contains("type"));
    model_containKeyMap.insert("RFID_num",RFID_obj.contains("num"));
    model_containKeyMap.insert("RFID_index",RFID_obj.contains("index"));
    if(RFID_obj.contains("index"))
    {
        QJsonArray RFID_index_array = RFID_obj["index"].toArray();
        for(int k = 0; k<RFID_index_array.size();k++ )
        {
            QJsonObject RFID_index_obj = RFID_index_array.at(k).toObject();
            model_containKeyMap.insert("RFID_x",RFID_index_obj.contains("x"));
            model_containKeyMap.insert("RFID_y",RFID_index_obj.contains("y"));
            model_containKeyMap.insert("RFID_z",RFID_index_obj.contains("z"));
        }
    }
    //vision
    QJsonObject vision_obj = obj["vision"].toObject();
    model_containKeyMap.insert("vision_num",vision_obj.contains("num"));
    model_containKeyMap.insert("vision_index",vision_obj.contains("index"));
    if(vision_obj.contains("index"))
    {
        QJsonArray  vision_array = vision_obj["index"].toArray();
        for(int k = 0; k<vision_array.size();k++)
        {
            QJsonObject vision_index_obj = vision_array.at(k).toObject();
            model_containKeyMap.insert("vision_type",vision_index_obj.contains("type"));
            model_containKeyMap.insert("vision_x",vision_index_obj.contains("x"));
            model_containKeyMap.insert("vision_y",vision_index_obj.contains("y"));
            model_containKeyMap.insert("vision_z",vision_index_obj.contains("z"));
            model_containKeyMap.insert("vision_pitch",vision_index_obj.contains("pitch"));
            model_containKeyMap.insert("vision_roll",vision_index_obj.contains("roll"));
            model_containKeyMap.insert("vision_yaw",vision_index_obj.contains("yaw"));
        }
    }
    //叉车
    if(obj.contains("fork"))
    {
        model_containKeyMap.insert("fork",obj.contains("fork"));
        QJsonObject fork_obj = obj["fork"].toObject();
        model_containKeyMap.insert("fork_blockLaserDist",fork_obj.contains("blockLaserDist"));
        model_containKeyMap.insert("fork_encoderSteps",fork_obj.contains("encoderSteps"));
        model_containKeyMap.insert("fork_lengthFactor",fork_obj.contains("lengthFactor"));
        model_containKeyMap.insert("fork_lengthPerTurn",fork_obj.contains("lengthPerTurn"));
        model_containKeyMap.insert("fork_minHeight",fork_obj.contains("minHeight"));
        model_containKeyMap.insert("fork_maxHeight",fork_obj.contains("maxHeight"));
        model_containKeyMap.insert("fork_pressureSensor",fork_obj.contains("pressureSensor"));
        model_containKeyMap.insert("fork_pump",fork_obj.contains("pump"));
        model_containKeyMap.insert("fork_type",fork_obj.contains("type"));
        model_containKeyMap.insert("fork_downBuffer",fork_obj.contains("downBuffer"));
        model_containKeyMap.insert("fork_upBuffer",fork_obj.contains("upBuffer"));
    }
    //Arm
    if(obj.contains("Arm"))
    {
        model_containKeyMap.insert("Arm",obj.contains("Arm"));
    }
    //Jack
    if(obj.contains("Jack"))
    {
        model_containKeyMap.insert("Jack",obj.contains("Jack"));
    }
    //Roller
    if(obj.contains("Roller"))
    {
        model_containKeyMap.insert("Roller",obj.contains("Roller"));
    }
    missKeys_list.clear();

    QMap<QString,bool>::iterator iter3 = model_containKeyMap.begin();
    for(; iter3 != model_containKeyMap.end();iter3++)
    {
        if(!iter3.value())
        {
            SCDebug<<"QString is "<<iter3.key()<<"do not contained "<<iter3.value() ;
            missKeys_list.append(iter3.key());
            m_isModelFileReasonable = false;
        }
        else
        {
            m_isModelFileReasonable = true;
        }
    }

    _missInforString =tr("The following are some fields of missing devices or corresponding devices:\n");
    int basicMissNum = _missInforString.size();
    if(missKeys_list.size()<1)
    {
        //若无多余字段清空字符串
        _missInforString = "";
    }
    //过滤不必要提示的字段
    filterDevice(missKeys_list);

    for(int m = 0; m < missKeys_list.size();m++)
    {
        _missInforString =_missInforString + missKeys_list.at(m) +"\n";
    }

    //如果错误_missInforString大小不为0说明存在缺失字段
    if(_missInforString.size()>basicMissNum||_extralInforString.size()>basicExtralLength||_repeatPromtList.size()>0)
    {
        _maskWid->move(0,0);
        QRect rect = this->rect();
        _maskWid->setGeometry(rect);
        _maskWid->show();
        _promptWid->setPromptInfor(_missInforString,_extralInforString,_repeatPromtList);
        _promptWid->show();
    }
    //如果不包含机器人则直接退出
    for(int i = 0; i< missKeys_list.size();i++)
    {
        if(missKeys_list.at(i)=="chassis")
            return false;
    }

    return  true;
}
void SCRobotFileModelWidget::filterDevice(QList<QString> &list)
{
    for(int i = 0; i < list.size();i++)
    {
        if(list.at(i)=="DI")
        {
            for(int j = 0; j < list.size();j++)
            {
                if(list.at(j).contains("DI_")&&list.at(j).size()>2)
                {
                    list.removeAt(j);
                    --j;
                }
            }
        }
        if(list.at(i)=="DO")
        {
            for(int j = 0; j < list.size();j++)
            {
                if(list.at(j).contains("DO_")&&list.at(j).size()>2)
                {
                    list.removeAt(j);
                    --j;
                }
            }
        }
        if(list.at(i)=="laser")
        {
            for(int j = 0; j < list.size();j++)
            {
                if(list.at(j).contains("laser_")&&list.at(j).size()>5)
                {
                    list.removeAt(j);
                    --j;
                }
            }
        }
        if(list.at(i)=="RFID")
        {
            for(int j = 0; j < list.size();j++)
            {
                if(list.at(j).contains("RFID_")&&list.at(j).size()>4)
                {
                    list.removeAt(j);
                    --j;
                }
            }
        }

        if(list.at(i)=="chassis")
        {
            for(int j = 0; j < list.size();j++)
            {
                if(list.at(j).contains("chassis_")&&list.at(j).size()>7)
                {
                    SCDebug<<"list.removeAt(j)"<<list.at(j);
                    list.removeAt(j);

                    --j;
                }

                if(list.at(j).contains("mode_param")&&list.at(j).size()>10)
                {
                    list.removeAt(j);
                    --j;
                }

            }
        }

        if(list.at(i)=="chassis_mode_param")//mode_param比较特殊不是一个设备，只是json结构中的一个中间元素
        {
            for(int j = 0; j < list.size();j++)
            {
                if(list.at(j).contains("mode_param_")&&list.at(j).size()>10)
                {
                    SCDebug<<"list.removeAt(j)"<<list.at(j);
                    list.removeAt(j);
                    --j;
                }
            }
        }

        if(list.at(i)=="ultrasonic")
        {
            for(int j = 0; j < list.size();j++)
            {
                if(list.at(j).contains("ultrasonic_")&&list.at(j).size()>10)
                {
                    list.removeAt(j);
                    --j;
                }
            }
        }
        if(list.at(i)=="vision")
        {
            for(int j = 0; j < list.size();j++)
            {
                if(list.at(j).contains("vision_")&&list.at(j).size()>6)
                {
                    list.removeAt(j);
                    --j;
                }
            }
        }
        if(list.at(i)=="magneticSensor")
        {
            for(int j = 0; j < list.size();j++)
            {
                if(list.at(j).contains("magneticSensor_")&&list.at(j).size()>14)
                {
                    list.removeAt(j);
                    --j;
                }
            }
        }


    }
}

void SCRobotFileModelWidget::getModelKeys(const QString filePath, QList<QString> &list)
{

    QFile loadFile(filePath);

    if(!loadFile.open(QIODevice::ReadOnly))
    {
        SCDebug << "could't open projects json";
    }

    QByteArray allData = loadFile.readAll();
    loadFile.close();

    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    if(json_error.error != QJsonParseError::NoError)
    {
        SCDebug << "json error!"<<json_error.errorString()<<allData.size() ;
        return;
    }
    QJsonObject rootObj = jsonDoc.object();
    getKeys(rootObj,"",list);
    //输出所有key，这一步是非必须的，放最后的话，你可能读不到任何key
    //    QStringList keys = rootObj.keys();
    //    for(int i = 0; i < keys.size(); i++)
    //    {
    //        keyList.append(keys.at(i));
    //        QJsonObject cObj = rootObj.value(keys.at(i)).isObject();
    //    }

}


QList<QString> SCRobotFileModelWidget::getExtralKeys(QList<QString> &current, QList<QString> &standard)
{
    extralKeys_list.clear();

    for(int i = 0; i< current.size();i++)
    {
        if(!standard.contains(current.at(i)))
        {
            if(!extralKeys_list.contains(current.at(i)))
            {
                extralKeys_list.append(current.at(i));
                SCDebug<<"extral key is "<<current.at(i);
            }
        }
    }
    return extralKeys_list;
}

void SCRobotFileModelWidget::getLoadModelKeys(const QString filePath,QList<QString>& list)
{
    QFile loadFile(filePath);

    if(!loadFile.open(QIODevice::ReadOnly))
    {
        SCDebug << "could't open projects json";
    }

    QByteArray allData = loadFile.readAll();
    loadFile.close();

    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    if(json_error.error != QJsonParseError::NoError)
    {
        SCDebug << "json error!"<<json_error.errorString()<<allData.size() ;
        return;
    }
    QJsonObject rootObj = jsonDoc.object();
    getLoadKeysFunc(rootObj,"",list);
}

void SCRobotFileModelWidget::getLoadKeysFunc(QJsonObject& rootObj,QString parent,QList<QString>& list)
{
    QStringList keys = rootObj.keys();//获取根元素key

    for(int i = 0; i < keys.size(); i++)
    {
        //如果标准匹配文件中没有此Key则不往下执行
        if(parent!="")
        {
            list.append(parent+"_"+keys.at(i));
        }
        else
        {
            list.append(keys.at(i));
        }
        if(!standardModelKeys_list.contains(keys.at(i))) continue;

        if(rootObj.value(keys.at(i)).isArray())
        {
            QJsonArray array = rootObj.value(keys.at(i)).toArray();
            for(int j = 0; j<array.size();j++)
            {
                QJsonObject tempObj = array.at(i).toObject();
                getKeys(tempObj,keys.at(i),list);
            }
        }
        else if(rootObj.value(keys.at(i)).isObject())
        {
            getLoadKeysFunc(rootObj.value(keys.at(i)).toObject(),keys.at(i),list);
        }
    }
}

void SCRobotFileModelWidget::getKeys(QJsonObject &rootObj, QString parent, QList<QString> &list)
{

    QStringList keys = rootObj.keys();//获取根元素key

    for(int i = 0; i < keys.size(); i++)
    {
        if(parent!="")
        {
            list.append(parent+"_"+keys.at(i));
        }
        else
        {
            list.append(keys.at(i));
        }

        if(rootObj.value(keys.at(i)).isArray())
        {
            QJsonArray array = rootObj.value(keys.at(i)).toArray();
            for(int j = 0; j<array.size();j++)
            {
                QJsonObject tempObj = array.at(i).toObject();
                getKeys(tempObj,keys.at(i),list);
            }
        }
        else if(rootObj.value(keys.at(i)).isObject())
        {
            getKeys(rootObj.value(keys.at(i)).toObject(),keys.at(i),list);
        }
    }
}

QGroupBox *SCRobotFileModelWidget::getGroupBox() const
{
    return GroupBox;
}

void SCRobotFileModelWidget::setGroupBox(QGroupBox *value)
{
    GroupBox = value;
}
void SCRobotFileModelWidget::setToolButtonStyleBy(Qt::ToolButtonStyle m_style)
{
    foreach(auto btn,ButtonGroup->buttons()){
        QToolButton*b = qobject_cast<QToolButton*>(btn);
        b->setToolButtonStyle(m_style);
    }
}
QButtonGroup *SCRobotFileModelWidget::getButtonGroup() const
{
    return ButtonGroup;
}

void SCRobotFileModelWidget::setButtonGroup(QButtonGroup *value)
{
    ButtonGroup = value;
}
/** 同步模型文件数据
 * @brief SCRobotFileModelWidget::slotSynchronizeData
 */
void SCRobotFileModelWidget::slotSynchronizeData()
{
    UiClass::init()->slotDeleteToolTipWidget(_toolTipWidgetKey);
    SendGlobalMsg::init()->sendGlobalMsg(SendGlobalMsg::E_GS_SynchronizeRbkModel);
}
/** 同步模型文件数据完成
 * @brief SCRobotFileModelWidget::slotSendGlobalMsgResult
 * @param sgs
 * @param var
 * @param isError
 */
void SCRobotFileModelWidget::slotSendGlobalMsgResult(int sgs,const QVariant& var,bool isError)
{
    switch (sgs) {
    case SendGlobalMsg::E_GS_SynchronizeRbkModel:
    {
        if(isError){
            slotMessage(var.toString(),2);
            return;
        }
    }
        break;
    case SendGlobalMsg::E_GS_keyPress:
    {
        qDebug()<<"_is_Ctrl_Key_pressed is-------------- "<<_is_Ctrl_Key_pressed<<" "<<var.toString();
        //        if(var.toString()=="S")//判断s键是否被按下
        //        {
        //            if(_is_Ctrl_Key_pressed&&isVisible())
        //            {
        //                slotSaveModel();
        //            }
        //        }
    }
        break;
    default:
        break;
    }
}

//void SCRobotFileModelWidget::on_comboBox_models_currentIndexChanged(int index)
//{
//    GlobalVariant::getInst()->flag = true;
//    if(index==0)//默认选项
//    {
//        newModel();
//        return;
//    }
//    QString modelPath = DEFAULTCONFIGMODEL_PATH + _modelchoseDlg->getCombox()->currentText()+".model";
//    if(loadFile(modelPath))
//    {
//        SCDebug<<"load filePath is "<<modelPath;
//        setCurrentFilePath(modelPath);//记录当前模型路径
//        set_file_existed(true);
//    }
//    else
//    {
//        if(getCurrentFilePath().isEmpty())
//        {
//            set_file_existed(false);
//        }
//    }
//}

void SCRobotFileModelWidget::slot_choseConfirm()
{
    if(_modelchoseDlg->isVisible())
    {
        _modelchoseDlg->hide();
    }

    GlobalVariant::getInst()->flag = true;
    if(_modelchoseDlg->getCombox()->currentIndex() ==0 )//默认选项
    {
        newModel();
        return;
    }
    QString modelPath = DEFAULTCONFIGMODEL_PATH + _modelchoseDlg->getCombox()->currentText()+".tmp";
    if(loadTemplate(modelPath))
    {
        fitPropertyWidget();
        SCDebug<<"load filePath is "<<modelPath;
        setCurrentFilePath(modelPath);//记录当前模型路径
        set_file_existed(true);
    }
    else
    {
        if(getCurrentFilePath().isEmpty())
        {
            set_file_existed(false);
        }
    }
}

void SCRobotFileModelWidget::slot_redo_clicked()
{
    GlobalVariant::getInst()->isRedo = true;
    SCDebug<<"redo clicked ================00000";
}

void SCRobotFileModelWidget::slot_undo_clicked()
{
    SCDebug<<"undo clicked ================1111";
}

void SCRobotFileModelWidget::slotToolBarRobotControlMenu(bool trigger)
{
    QAction* a = (QAction*)sender();
    updateToolButtonStyle(a->data().toInt());
}

void SCRobotFileModelWidget::slotRoboModelContextMenuRequested(const QPoint &point)
{
    Q_UNUSED(point);
    SCDebug;
    //    _pToolBarModelMenu = new QMenu(this);
    //    QStringList list;
    //    list<<("只显示图标")<<("只显示文字") <<("显示图标和文字");
    //    QActionGroup* group = new QActionGroup(this);
    //    group->setExclusive(true);

    //    for(int i = 0; i < list.size();i++)
    //    {
    //        QAction* a = new QAction(list.at(i),_pToolBarModelMenu);
    //        a->setCheckable(true);
    //        a->setData(i);
    //        if(_currentToolButtonStyleIndex == i)
    //        {
    //            a->setCheckable(true);
    //        }
    //        connect(a,SIGNAL(triggered(bool)),this,SLOT(slotToolBarRobotControlMenu(bool)));
    //        _pToolBarModelMenu->addAction(a);
    //        group->addAction(a);
    //    }
    //    _pToolBarModelMenu->move(this->cursor().pos());
    //    _pToolBarModelMenu->show();
}

void SCRobotFileModelWidget::slotNewModelbyDefault()
{
    if(!isAllowExec())
        return;
    newModel();
    setIsGeneratbyGuid(false);
}

void SCRobotFileModelWidget::slotNewModelbyGudience()
{
    if(!isAllowExec())
        return;
    _guidenceWidget = new WidgetGuidance;
    _guidenceWidget->setAttribute(Qt::WA_DeleteOnClose, true);
    connect(_guidenceWidget,SIGNAL(sig_finished()),this,SLOT(slotGuidNewModel()));//完成时新建模型
    _guidenceWidget->setfinished(false);
    _guidenceWidget->ShowCurrentGuidWid(0);
    _guidenceWidget->SetGuidInstruction();
    _guidenceWidget->setModal(true);
    _guidenceWidget->show();
    setIsGeneratbyGuid(true);

}

void SCRobotFileModelWidget::slotNewModelbyTemplate()
{
    if(!isAllowExec())
        return;
    //加载模板文件列表
    _modelchoseDlg->loadMaps(DEFAULTCONFIGMODEL_PATH);
    _modelchoseDlg->setModal(true);
    _modelchoseDlg->show();
    setIsGeneratbyGuid(false);
}

void SCRobotFileModelWidget::slotOpenTempLocation()
{
    QString filepath = "./appInfo/model/template";
    QDesktopServices::openUrl(QUrl::fromLocalFile(filepath));
}

void SCRobotFileModelWidget::slot_saveAsTempConfirm(QString name)
{
    //保存模板
    pEditRobotModelWidget->saveAsTemp(name,_isGeneratedByGuidence);
    if(_saveTempWid->isVisible())
    {
        _saveTempWid->hide();
    }
}

void SCRobotFileModelWidget::slot_saveASTempCancel()
{
    _saveTempWid->hide();
}

void SCRobotFileModelWidget::slotMessage(const QString&msg,int type,bool isMask)
{
    UiClass::init()->showToastr(msg,(SCToastr::SCToastrType)type,this,isMask);
}
