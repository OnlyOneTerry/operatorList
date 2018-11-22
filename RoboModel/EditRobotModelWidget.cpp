#include "EditRobotModelWidget.h"
#include "ui_EditRobotModelWidget.h"
#include "RoLog.h"
#include <QFile>
#include <QJsonDocument>
#include <QFileDialog>
#include "PublicClass.h"
#include "../SCBaseGraphics/SCGraphicsHelper/GraphicsHelper.h"
#include "ModelHelper.h"
#include"QJson/parser.h"
#include"ModelBasicSettingWidget.h"
#include<QScrollBar>
#include"UiClass.h"
#include "RoFileManager.h"

EditRobotModelWidget::EditRobotModelWidget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::EditRobotModelWidget),
    model_view_(new ModelGraphicsView(this)),
    m_undoStack(NULL),
    _isSaveSuccess(true)
{
    ui->setupUi(this);
    init_model_graphics ();
    m_undoStack  = UndoStack::InitStack();
}

EditRobotModelWidget::~EditRobotModelWidget()
{
    delete model_view_;
    delete ui;
}

/*******************************************************
 * Description: 另存为
 *
 * @para
 * @author
 * @date
 *******************************************************/
bool EditRobotModelWidget::saveAsOtherFile(bool isGuid = false)
{
    //存在激光且保证只有一个激光用于定位
    if(!isAllowSave(isGuid))
    {
        _isSaveSuccess = false;
        return false;
    }
    QString modelDir = PublicClass::init()->getModelsPath();
    QString path = QFileDialog::getSaveFileName(this,
                                                tr("model"),
                                                QString(modelDir+"/%1").arg (model_view_->get_model_variant_map()["model"].toString()),
            tr("model(*.model)"));

    SCDebug<<"path is "<<path;

    if (path.isEmpty()){
        _isSaveSuccess = false;//取消保存情况
        return true;
    }
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly))
    {
        setErrorString(QString(tr("failed to open %1")).arg(path));
        return false;
    }else{
        if(this->filePath() == "new")
            this->setFilePath(path);
    }
    QTextStream ofs(&file);
    QVariantMap model_map=model_view_->get_model_variant_map ();
    ofs<<optimizationVariantMap (model_map);
    file.flush();
    file.close();
    _isSaveSuccess = true;//保存成功

    //-----------------------提示设置DO状态
    UiClass::init()->showToastr(tr("Save successfuly, please take care about the status of DO!"),(SCToastr::E_Info));
    return true;
}

bool EditRobotModelWidget::saveAsTemp(bool isGuid)
{
    if(!isAllowSave(isGuid))
    {
        _isSaveSuccess = false;
        return false;
    }
    //若不存在默认文件夹则先创建
    QDir dir;
    dir.cd("./appInfo/model");
    if(!dir.exists("template"))
    {
        bool ok = dir.mkdir("template");//只创建一级子目录，即必须保证上级目录存在
    }
    QString path = QFileDialog::getSaveFileName(this,
                                                tr("tmp"),
                                                QString("./appInfo/model/template/%1").arg (model_view_->get_model_variant_map()["model"].toString()),
            ("tmp(*.tmp)"));
    SCDebug<<"path is "<<path;
    if (path.isEmpty()){
        _isSaveSuccess = false;//取消保存情况
        return true;
    }
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly))
    {
        setErrorString(QString(tr("Failt to open %1")).arg(path));
        return false;
    }else{
        if(this->filePath() == "new")
            this->setFilePath(path);
    }
    QTextStream ofs(&file);
    QVariantMap model_map=model_view_->get_model_variant_map ();
    ofs<<optimizationVariantMap (model_map);
    file.flush();
    file.close();
    _isSaveSuccess = true;//保存成功

    //-----------------------提示设置DO状态
    UiClass::init()->showToastr(tr("Save successfuly, please take care about the status of DO!"),SCToastr::E_Info);
    return true;
}

bool EditRobotModelWidget::saveAsTemp(QString name,bool isGuid)
{
    //若不存在默认文件夹则先创建
    QDir dir;
    dir.cd("./appInfo/model");
    if(!dir.exists("template"))
    {
        bool ok = dir.mkdir("template");//只创建一级子目录，即必须保证上级目录存在
    }

    if (name.isEmpty()){
        _isSaveSuccess = false;//取消保存情况
        return true;
    }

    QString path = "./appInfo/model/template/"+name+".tmp";
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly))
    {
        setErrorString(QString(tr("failed to open %1")).arg(path));
        return false;
    }else{
        if(this->filePath() == "new")
            this->setFilePath(path);
    }
    QTextStream ofs(&file);
    QVariantMap model_map=model_view_->get_model_variant_map ();
    ofs<<optimizationVariantMap (model_map);
    file.flush();
    file.close();
    _isSaveSuccess = true;//保存成功

    //-----------------------提示设置DO状态
    UiClass::init()->showToastr(tr("Save successfuly, please take care about the status of DO!"),SCToastr::E_Info);
    return true;
}

void EditRobotModelWidget::init_model_graphics()
{
    ui->vlayout_graphics->addWidget (model_view_);
}

/*******************************************************
 * Description: 加载模型文件
 *
 * @para filePath- 文件路径
 * @author
 * @date
 *******************************************************/
bool EditRobotModelWidget::loadFile(QString filePath)
{
    if (filePath.isEmpty())
    {
        setErrorString(QString(tr("EditRobotModelWidget::loadModelFile Read empty file")));
        return false;
    }
    else
    {
        filePath = this->filePath();
    }

    //QByteArray robot_model_json_data = readFile(filePath);
    QJson::Parser jparse;
    QByteArray robot_model_json_data = jparse.s_parse(readFile(filePath));

    if(robot_model_json_data.isEmpty())
    {
        setErrorString (QString(tr("Model file data is empty !")));
        return false;
    }

    QJsonObject robot_model_json_object = QJsonDocument::fromJson(robot_model_json_data).object();

    if(robot_model_json_object.empty ())
    {
        setErrorString(tr("<font color=\"red\">Json Wrong format ！</font>"));
        return false;
    }

    model_view_->set_model_jobj (robot_model_json_object);

    return true;
}

bool EditRobotModelWidget::load_from_byteArray(const QByteArray &byte_array)
{
    QByteArray robot_model_json_data = byte_array;

    if(robot_model_json_data.isEmpty())
    {
        setErrorString (QString(tr("Model file data is empty !")));
        return false;
    }
    QJsonParseError *error = Q_NULLPTR;
    QJsonObject robot_model_json_object = QJsonDocument::fromJson(robot_model_json_data,error).object();

    SCDebug<<"error is "<<errorString();
    if(robot_model_json_object.empty ())
    {
        setErrorString(tr("<font color=\"red\">Json Wrong format!</font>"));
        return false;
    }

    model_view_->set_model_jobj (robot_model_json_object);

    return true;
}

void EditRobotModelWidget::init_new()
{
    //    model_view_->model_scene()->init_new();
}

bool EditRobotModelWidget::isAllowSave(bool isGuid)
{
//    if(!model_view_->model_scene()->isAlldevsOnChassis())
//    {
//        QStringList list;
//        list<<tr("Cancel");
//        UiClass::init()->showDialog(tr("Operation failed, please make sure all the devices is on the chassis!"),tr("Information"),0,list);
//        return false;
//    }
    if(model_view_->model_scene()->get_Laserlist()->size()>0 && model_view_->model_scene()->laserForLocationNum()!=1)
    {
        QStringList list;
        list<<tr("Cancel");
        UiClass::init()->showDialog(tr("Operation failed, please make sure there is only one laser for loaction!"),tr("Information"),0,list);
        return false;
    }

     //引导创建并且没有完成引导配置
    if(!model_view_->model_scene()->isGuidenceFinished()&&isGuid)
    {
        QStringList list;
        list<<tr("Cancel");
        UiClass::init()->showDialog(tr("Operation failed, please continue to click right graphics to finish guidence,RFID except,attention you just need to click one of them if there are several same items like three lasers!"),tr("Information"),0,list);
        return false;
    }
    return true;
}

void EditRobotModelWidget::setUndoStack(QUndoStack *stack)
{
    m_undoStack = stack;
}

QUndoStack *EditRobotModelWidget::getUndoStack()
{
    return m_undoStack;
}

ModelGraphicsView *EditRobotModelWidget::getGraphicView()
{
    return model_view_;
}

QByteArray EditRobotModelWidget::readFile(const QString &filePath)
{
    QByteArray b;
    _filePath = filePath;

    if(!_filePath.isEmpty()){

        if(_file.isOpen()){
            _file.close();
        }
        _file.setFileName(_filePath);
        if(_file.open(QIODevice::ReadOnly)){
            b = _file.readAll();
            _file.close();
        }else{
            _errorString = QString(tr("Warning:CustomTabChildWidget:[%1]:%2"))
                    .arg(_filePath)
                    .arg(_file.errorString());
        }
    }else{
        _errorString = QString(tr("Warning:CustomTabChildWidget:The file name is empty.")).arg(_filePath);
    }
    if(b.isEmpty()){
        _errorString = QString(tr("Warning:CustomTabChildWidget:[%1] is empty.")).arg(_filePath);
    }
    return b;
}

QString EditRobotModelWidget::filePath() const
{
    return _filePath;
}

void EditRobotModelWidget::setFilePath(const QString &filePath)
{
    _filePath = filePath;
}

QString EditRobotModelWidget::fileName() const
{
    return _fileName;
}

void EditRobotModelWidget::setFileName(QString &fileName)
{
    _fileName = fileName;

    ui->label_errorInfor->setText(fileName);
    ui->label_errorInfor->setAlignment(Qt::AlignCenter);
}

QString EditRobotModelWidget::errorString() const
{
    return _errorString;
}

void EditRobotModelWidget::setErrorString(const QString &errorString)
{
    _errorString = errorString;
}

bool EditRobotModelWidget::isChanged() const
{
    return _isChanged;
}

void EditRobotModelWidget::setIsChanged(bool isChanged)
{
    _isChanged = isChanged;
}

bool EditRobotModelWidget::getIsSaveSuccess()
{
    return _isSaveSuccess;
}


/*******************************************************
 * Description: 保存模型文件
 *
 * @para filePath- 文件路径
 * @author
 * @date
 *******************************************************/
bool EditRobotModelWidget::saveFile(QString filePath, int index, bool isGuid)
{
    if(!isAllowSave(isGuid))
    {
        _isSaveSuccess = false;
        return false;
    }
    if (this->filePath().isEmpty()||this->filePath()=="new")
    {
        return saveAsOtherFile(isGuid);
    }
    else
    {
        QFile file(this->filePath());
        if (!file.open(QIODevice::WriteOnly))
        {
            setErrorString(file.errorString());
            SCWarning<<file.fileName()<<" : "<<file.errorString();
            return false;
        }
        QTextStream ofs(&file);
        QVariantMap model_map=model_view_->get_model_variant_map ();
        ofs<<optimizationVariantMap (model_map);
        file.flush();
        file.close();
        QFileInfo fileInfo(file.fileName());
        if(fileInfo.completeBaseName() == "robot"
                && PublicClass::init()->existsRobotFileMd5(PublicClass::init()->_currentKeyID)){
            //---计算模型文件md5
            QString md5 = RoFileManager::init()->md5ReadAllFile(this->filePath());
            //            SCDebug<<"md5:"<<md5;
            PublicClass::init()->RobotFileMd5ClassMap.value(PublicClass::init()->_currentKeyID)->sMap[RobotFileMd5Class::E_SM_Model]->localMd5 = md5;
        }
    }
    _isSaveSuccess = true;
    //-----------------------提示设置DO状态
    UiClass::init()->showToastr(tr("Save successfuly, please take care about the status of DO!"),SCToastr::E_Info);
    return true;
}


