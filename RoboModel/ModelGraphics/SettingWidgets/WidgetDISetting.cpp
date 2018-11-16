#include "WidgetDISetting.h"
#include "ui_WidgetDISetting.h"
#include "ItemDI.h"
#include "ModelGraphicsScene.h"
#include "ModelGraphicsView.h"
#include "ModelCommon.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include "PublicClass.h"
#include"ItemAngelCtroller.h"
#include"ItemScanArea.h"

WidgetDISetting::WidgetDISetting(ItemDI* itemdi,QWidget *parent) :
    WidgetSetting(parent),
    item_di_(itemdi),
    ui(new Ui::WidgetDISetting)
{
    ui->setupUi(this);
    setType(WIDGET_DI);
    initWidgetData();
    ui->doubleSpinBox_r->setDecimals(3);
    ui->doubleSpinBox_range->setDecimals(3);
    ui->doubleSpinBox_range->setSingleStep(1);
    ui->doubleSpinBox_range->setMaximum(360);
    ui->doubleSpinBox_minDist->setMaximum(100);
    ui->doubleSpinBox_minDist->setMinimum(0);
    ui->doubleSpinBox_maxDist->setMaximum(100);
    ui->doubleSpinBox_maxDist->setSingleStep(0.5);
    ui->doubleSpinBox_maxDist->setMinimum(0);
    ui->doubleSpinBox_minDist->setSingleStep(0.5);
//    ui->spinBox_id->setMaximum(item_di_->model_scene()->model_view()->basic_widget()->model_base_data_.max_di_num_-1);
    connect(item_di_->model_scene()->model_view()->basic_widget(),SIGNAL(sig_max_num_changed(quint8,quint8)),this,SLOT(slot_max_mum_changed(quint8,quint8)));
    connect(&item_di_->timer,SIGNAL(timeout()),this,SLOT(on_timer_out()));

}

WidgetDISetting::~WidgetDISetting()
{
    delete ui;
}

void WidgetDISetting::update_value ()
{
    ui->spinBox_id->blockSignals(true);
    ui->spinBox_id->setValue (item_di_->data_di_.id_);
    ui->spinBox_id->blockSignals(false);
    ui->doubleSpinBox_x->blockSignals(true);
    ui->doubleSpinBox_x->setValue (item_di_->data_di_.x_);
    ui->doubleSpinBox_x->blockSignals(false);
    ui->doubleSpinBox_y->blockSignals(true);
    ui->doubleSpinBox_y->setValue (item_di_->data_di_.y_);
    ui->doubleSpinBox_y->blockSignals(false);
    ui->doubleSpinBox_z->blockSignals(true);
    ui->doubleSpinBox_z->setValue (item_di_->data_di_.z_);
    ui->doubleSpinBox_z->blockSignals(false);
    ui->doubleSpinBox_r->blockSignals(true);
    ui->doubleSpinBox_r->setValue (item_di_->data_di_.r_);
    ui->doubleSpinBox_r->blockSignals(false);
    ui->doubleSpinBox_range->blockSignals(true);
    ui->doubleSpinBox_range->setValue (item_di_->data_di_.rang_);
    ui->doubleSpinBox_range->blockSignals(false);
    ui->comboBox_func->blockSignals(true);
    int idx = ui->comboBox_func->findData(item_di_->data_di_.func_);
    ui->comboBox_func->setCurrentIndex (idx);
    ui->comboBox_func->setOri_index(item_di_->data_di_.func_);
    ui->comboBox_func->blockSignals(false);
    ui->comboBox_type->blockSignals(true);
    idx = ui->comboBox_type->findData(item_di_->data_di_.type_);
    ui->comboBox_type->setCurrentIndex (idx);
    ui->comboBox_type->setOri_index(item_di_->data_di_.type_);
    ui->comboBox_type->blockSignals(false);
    ui->checkBox_inverse->blockSignals(true);
    ui->checkBox_inverse->setChecked (item_di_->data_di_.inverse_);
    ui->checkBox_inverse->blockSignals(false);

    ui->doubleSpinBox_minDist->blockSignals(true);
    ui->doubleSpinBox_minDist->setValue(item_di_->data_di_.minDist_);
    ui->doubleSpinBox_minDist->blockSignals(false);
    ui->doubleSpinBox_maxDist->blockSignals(true);
    ui->doubleSpinBox_maxDist->setValue(item_di_->data_di_.maxDist_);
    ui->doubleSpinBox_maxDist->blockSignals(false);


}

void WidgetDISetting::set_r(const qreal &angel)
{
    //非鼠标点击改变编辑框改变值，阻塞改变消息
    ui->doubleSpinBox_r->blockSignals(true);
    ui->doubleSpinBox_r->setValue(angel);
    ui->doubleSpinBox_r->blockSignals(false);
    item_di_->data_di_.r_=angel;
    item_di_->model_scene()->model_view()->update();
}

void WidgetDISetting::initWidgetData()
{
    ui->spinBox_id->blockSignals(true);
    ui->spinBox_id->setValue (item_di_->data_di_.id_);
    ui->spinBox_id->blockSignals(false);
    ui->spinBox_id->setOriValue(item_di_->data_di_.id_);
    ui->doubleSpinBox_x->blockSignals(true);
    ui->doubleSpinBox_x->setValue (item_di_->data_di_.x_);
    ui->doubleSpinBox_x->blockSignals(false);
    ui->doubleSpinBox_x->setOriValue(item_di_->data_di_.x_);
    ui->doubleSpinBox_y->blockSignals(true);
    ui->doubleSpinBox_y->setValue (item_di_->data_di_.y_);
    ui->doubleSpinBox_y->blockSignals(false);
    ui->doubleSpinBox_y->setOriValue(item_di_->data_di_.y_);
    ui->doubleSpinBox_z->blockSignals(true);
    ui->doubleSpinBox_z->setValue (item_di_->data_di_.z_);
    ui->doubleSpinBox_z->blockSignals(false);
    ui->doubleSpinBox_z->setOriValue(item_di_->data_di_.z_);
    ui->doubleSpinBox_r->blockSignals(true);
    ui->doubleSpinBox_r->setValue (item_di_->data_di_.r_);
    ui->doubleSpinBox_r->blockSignals(false);
    ui->doubleSpinBox_r->setOriValue(item_di_->data_di_.r_);
    item_di_->getAngelCtroller()->setOriAngel(item_di_->data_di_.r_);
    ui->doubleSpinBox_range->blockSignals(true);
    ui->doubleSpinBox_range->setValue (item_di_->data_di_.rang_);
    ui->doubleSpinBox_range->blockSignals(false);
    ui->doubleSpinBox_range->setOriValue(item_di_->data_di_.rang_);

//    ui->comboBox_func->blockSignals(true);
//    ui->comboBox_func->setCurrentIndex (item_di_->data_di_.func_);
//    ui->comboBox_func->blockSignals(false);
//    ui->comboBox_func->setOri_index(item_di_->data_di_.func_);
    ui->checkBox_inverse->blockSignals(true);
    ui->checkBox_inverse->setChecked (item_di_->data_di_.inverse_);
    ui->checkBox_inverse->blockSignals(false);
    ui->checkBox_inverse->setOriValue(item_di_->data_di_.inverse_);


    ui->doubleSpinBox_minDist->blockSignals(true);
    ui->doubleSpinBox_minDist->setValue(item_di_->data_di_.minDist_);
    ui->doubleSpinBox_minDist->blockSignals(false);
    ui->doubleSpinBox_minDist->setOriValue(item_di_->data_di_.minDist_);

    ui->doubleSpinBox_maxDist->blockSignals(true);
    ui->doubleSpinBox_maxDist->setValue(item_di_->data_di_.maxDist_);
    ui->doubleSpinBox_maxDist->blockSignals(false);
    ui->doubleSpinBox_maxDist->setOriValue(item_di_->data_di_.maxDist_);


    ConfigureData funcData = read_funcConfigFile();

    QList<ItemData>::iterator iter_func = funcData.dataList.begin();
    ui->comboBox_func->blockSignals(true);
    for(;iter_func!= funcData.dataList.end();iter_func++)
    {
        ui->comboBox_func->addItem((*iter_func).describtion,(*iter_func).index);
    }

    if(item_di_->isCreatedByLoading_)//加载模型文件创建
    {
        int idx = ui->comboBox_func->findData(item_di_->data_di_.func_);
        ui->comboBox_func->setCurrentIndex(idx);
    }
    else //拖拽到场景中创建
    {
        int idx = ui->comboBox_func->findData(funcData.defaultidx);
        ui->comboBox_func->setCurrentIndex(idx);
    }
    ui->comboBox_func->setOri_index(ui->comboBox_func->currentIndex());
    ui->comboBox_func->blockSignals(false);

    ConfigureData typeData = read_typeConfigFile();

    QList<ItemData>::iterator iter_type = typeData.dataList.begin();
    ui->comboBox_type->blockSignals(true);
    for(;iter_type!= typeData.dataList.end();iter_type++)
    {
        ui->comboBox_type->addItem((*iter_type).describtion,(*iter_type).index);
    }
    if(item_di_->isCreatedByLoading_)
    {
        int idx = ui->comboBox_type->findData(item_di_->data_di_.type_);
        qDebug()<<"di type is ---"<<item_di_->data_di_.type_;
        qDebug()<<"di type index is ---"<<idx;
        ui->comboBox_type->setCurrentIndex (idx);
    }
    else
    {
        int idx = ui->comboBox_type->findData(typeData.defaultidx);
        ui->comboBox_type->setCurrentIndex(idx);
    }
    ui->comboBox_type->setOri_index(ui->comboBox_type->currentIndex());
    ui->comboBox_type->blockSignals(false);
}

ConfigureData WidgetDISetting::read_funcConfigFile()
{
    return  readConfigData(CONFIGFILE_PATH,"func");
}

ConfigureData WidgetDISetting::read_typeConfigFile()
{
    return  readConfigData(CONFIGFILE_PATH,"type");
}


ConfigureData WidgetDISetting::readConfigData(const QString &filePath,const QString &argu)
{
    QString val;

    QFile file;

    file.setFileName(filePath);

    file.open(QIODevice::ReadOnly|QIODevice::Text);

    val = file.readAll();

    file.close();

    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject obj = d.object();

    QJsonValue chassis_value = obj.value(QString("chassis"));
    QJsonObject chassis_obj = chassis_value.toObject();

    QJsonValue  DI_value= chassis_obj["DI"];
    QJsonObject DI_obj = DI_value.toObject();

    QJsonValue  argu_value = DI_obj.value(argu);
    QJsonObject  argu_obj  = argu_value.toObject();


    ConfigureData  cfData;

    QJsonValue defaultIdex = argu_obj["defaultIndex"];

    cfData.defaultidx = defaultIdex.toInt();
    QJsonArray typeList = argu_obj["typeList"].toArray();

    for(int i = 0;i < typeList.size();i++)
    {
        ItemData tempData;
        QJsonObject tempObj = typeList.at(i).toObject();
        QJsonValue tempValue = tempObj["index"];
        QJsonValue tempDesc  = tempObj["desc"];

        tempData.index = tempValue.toInt();
        tempData.describtion = tempDesc.toString();

        cfData.dataList.push_back(tempData);
    }

    return cfData;
}

void WidgetDISetting::on_spinBox_id_valueChanged(int arg1)
{
    if(item_di_->model_scene()->get_id_list(0).contains(QString::number(arg1)))
    {
        if(arg1 != item_di_->data_di_.id_)
        {
            ui->label_prompt->setText(tr("The Id already exists"));
            ui->label_prompt->setStyleSheet("color:red;");
        }
        else
        {
            item_di_->data_di_.id_=arg1;
            item_di_->update_item ();
            ui->label_prompt->setText(tr("Setup success"));
            ui->label_prompt->setStyleSheet("color::green;");
        }
        return;
    }
    else
    {
        item_di_->data_di_.id_=arg1;
        item_di_->update_item ();
        ui->label_prompt->setText(tr("Setup success"));
        ui->label_prompt->setStyleSheet("color::green;");
    }


    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->spinBox_id->getIsClickedTrigger())
    {
        ui->spinBox_id->setNewValue(arg1);

        SpinBoxCommand* sp_command = new SpinBoxCommand(
                    ui->spinBox_id,ui->spinBox_id->getOriValue(),
                    ui->spinBox_id->getNewValue(),this,NULL);
        m_unStack->push(sp_command);
        ui->spinBox_id->setOriValue(arg1);
    }
    ui->spinBox_id->setIsClickedTrigger(true);
}

void WidgetDISetting::on_doubleSpinBox_x_valueChanged(double arg1)
{
    item_di_->data_di_.x_=arg1;

    item_di_->update_item ();

    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_x->getIsClickedTrigger())
    {
        ui->doubleSpinBox_x->setNewValue(arg1);
        doubleSpinBoxCommand* dbspinbox_command = new doubleSpinBoxCommand(ui->doubleSpinBox_x,item_di_,ui->doubleSpinBox_x->getOriValue(),
                                                                           ui->doubleSpinBox_x->getNewValue(),this,NULL);
        m_unStack->push(dbspinbox_command);

        ui->doubleSpinBox_x->setOriValue(arg1);
    }
    ui->doubleSpinBox_x->setIsClickedTrigger(true);
}

void WidgetDISetting::on_doubleSpinBox_y_valueChanged(double arg1)
{
    item_di_->data_di_.y_=arg1;

    item_di_->update_item ();

    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_y->getIsClickedTrigger())
    {
        ui->doubleSpinBox_y->setNewValue(arg1);
        doubleSpinBoxCommand* dbspinbox_command = new doubleSpinBoxCommand(ui->doubleSpinBox_y,item_di_,ui->doubleSpinBox_y->getOriValue(),
                                                                           ui->doubleSpinBox_y->getNewValue(),this,NULL);
        m_unStack->push(dbspinbox_command);

        ui->doubleSpinBox_y->setOriValue(arg1);
    }
    ui->doubleSpinBox_y->setIsClickedTrigger(true);
}

void WidgetDISetting::on_doubleSpinBox_z_valueChanged(double arg1)
{
    item_di_->data_di_.z_=arg1;

    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_z->getIsClickedTrigger())
    {
        ui->doubleSpinBox_z->setNewValue(arg1);
        doubleSpinBoxCommand* dbspinbox_command = new doubleSpinBoxCommand(ui->doubleSpinBox_z,item_di_,ui->doubleSpinBox_z->getOriValue(),
                                                                           ui->doubleSpinBox_z->getNewValue(),this,NULL);
        m_unStack->push(dbspinbox_command);

        ui->doubleSpinBox_z->setOriValue(arg1);
    }
    ui->doubleSpinBox_z->setIsClickedTrigger(true);
}

void WidgetDISetting::on_doubleSpinBox_r_valueChanged(double arg1)
{
    item_di_->data_di_.r_=arg1;

    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_r->getIsClickedTrigger())
    {
        ui->doubleSpinBox_r->setNewValue(arg1);
        doubleSpinBoxCommand* dbspinbox_command = new doubleSpinBoxCommand(ui->doubleSpinBox_r,item_di_,ui->doubleSpinBox_r->getOriValue(),
                                                                           ui->doubleSpinBox_r->getNewValue(),this,NULL);
        m_unStack->push(dbspinbox_command);

        ui->doubleSpinBox_r->setOriValue(arg1);
    }
    ui->doubleSpinBox_r->setIsClickedTrigger(true);

    //通过属性框反向控制设备角度
    item_di_->getScanArea()->setRotation(90+arg1);//扫描区域与DI设备相差90度，在外部旋转item用rotation，在item内部旋转旋转绘画设备painter
    qreal rotationRad = item_di_->getScanArea()->get_rotateRadius();
    item_di_->getScanArea()->setPos(rotationRad*qCos(arg1*M_PI/180.),rotationRad*qSin(arg1*M_PI/180.));
    item_di_->getScanArea()->update();
    item_di_->set_d_r(arg1);//重画di设备
    item_di_->getAngelCtroller()->set_rotation(arg1);//对角标旋转
}

void WidgetDISetting::on_comboBox_func_currentIndexChanged(int index)
{
    item_di_->data_di_.func_ = ui->comboBox_func->currentData().toInt();
    if(m_unStack==NULL)
    {
        return;
    }
    if(ui->comboBox_func->getIsClickedTrigger())
    {
        ui->comboBox_func->setNew_index(index);

        customboxCommand* combx_command = new customboxCommand(ui->comboBox_func,ui->comboBox_func->getOri_index(),
                                                               ui->comboBox_func->getNew_index(),this,NULL);
        m_unStack->push(combx_command);

        ui->comboBox_func->setOri_index(index);
    }
    ui->comboBox_func->setIsClickedTrigger(true);
}

void WidgetDISetting::on_comboBox_type_currentIndexChanged(int index)
{

    item_di_->data_di_.type_ = ui->comboBox_type->currentData().toInt();
    qDebug()<<"item_di_->data_di_.type_ is --------"<<item_di_->data_di_.type_;
    if(m_unStack==NULL)
    {
        return;
    }
    if(ui->comboBox_type->getIsClickedTrigger())
    {
        ui->comboBox_type->setNew_index(index);

        customboxCommand* combx_command = new customboxCommand(ui->comboBox_type,ui->comboBox_type->getOri_index(),
                                                               ui->comboBox_type->getNew_index(),this,NULL);
        m_unStack->push(combx_command);

        ui->comboBox_type->setOri_index(index);
    }
    ui->comboBox_type->setIsClickedTrigger(true);
}

void WidgetDISetting::on_checkBox_inverse_toggled(bool checked)
{
    item_di_->data_di_.inverse_=checked;

    if(m_unStack==NULL)
    {
        return;
    }
    ui->checkBox_inverse->setNewValue(checked);
    checkCommand* chk_command = new checkCommand(ui->checkBox_inverse,ui->checkBox_inverse->getOriValue()
                                                 ,ui->checkBox_inverse->getNewValue(),this,NULL);
    m_unStack->push(chk_command);
    ui->checkBox_inverse->setOriValue(checked);

}


void WidgetDISetting::slot_max_mum_changed(quint8 type, quint8 max_value)
{
    if(type==0)
    {
        ui->spinBox_id->setMaximum(max_value-1);
    }
}

void WidgetDISetting::on_spinBox_id_editingFinished()
{
    int arg1=ui->spinBox_id->value();

    if(! item_di_->model_scene()->get_id_list(0).contains(QString::number(arg1)))
    {
        item_di_->data_di_.id_=arg1;
        item_di_->update_item ();
        ui->label_prompt->setText(tr("Setup success"));
        ui->label_prompt->setStyleSheet("color::green;");
    }
    else
    {

        if(arg1 != item_di_->data_di_.id_)
        {
            ui->spinBox_id->setValue(item_di_->data_di_.id_);
            ui->label_prompt->setText(tr("The Id already exists"));
            ui->label_prompt->setStyleSheet("color:red;");
        }
        else
        {
            ui->label_prompt->setText(tr("Setup success"));
            ui->label_prompt->setStyleSheet("color::green;");
        }
    }

    //500毫秒后隐藏
    item_di_->timer.setInterval(500);
    item_di_->timer.setSingleShot(true);
    item_di_->timer.start();
}

void WidgetDISetting::on_doubleSpinBox_maxDist_valueChanged(double arg1)
{
    ui->doubleSpinBox_minDist->setMaximum(arg1);
    item_di_->data_di_.maxDist_ = arg1;

    if(m_unStack == NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_maxDist->getIsClickedTrigger())
    {
        ui->doubleSpinBox_maxDist->setNewValue(arg1);

        doubleSpinBoxCommand* dbspinbox_command = new doubleSpinBoxCommand(ui->doubleSpinBox_maxDist,item_di_,
                                                                           ui->doubleSpinBox_maxDist->getOriValue(),
                                                                           ui->doubleSpinBox_maxDist->getNewValue(),
                                                                           this,NULL);
        m_unStack->push(dbspinbox_command);
        ui->doubleSpinBox_maxDist->setOriValue(arg1);
        ui->doubleSpinBox_maxDist->setIsClickedTrigger(true);
    }

    item_di_->getScanArea()->setMaxDist(arg1);
    item_di_->getScanArea()->update();
    item_di_->update_item();
}

void WidgetDISetting::on_doubleSpinBox_minDist_valueChanged(double arg1)
{
    ui->doubleSpinBox_maxDist->setMinimum(arg1);
    item_di_->data_di_.minDist_ = arg1;

    if(m_unStack == NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_minDist->getIsClickedTrigger())
    {
        ui->doubleSpinBox_minDist->setNewValue(arg1);

        doubleSpinBoxCommand* dbspinbox_command = new doubleSpinBoxCommand(ui->doubleSpinBox_minDist,item_di_,
                                                                           ui->doubleSpinBox_minDist->getOriValue(),
                                                                           ui->doubleSpinBox_minDist->getNewValue(),
                                                                           this,NULL);
        m_unStack->push(dbspinbox_command);
        ui->doubleSpinBox_minDist->setOriValue(arg1);
    }

    ui->doubleSpinBox_minDist->setIsClickedTrigger(true);
}

void WidgetDISetting::on_timer_out()
{
    ui->label_prompt->setText("");
}

void WidgetDISetting::set_func_byConfigure(int index)
{
    ui->comboBox_func->blockSignals(true);
    ui->comboBox_func->setCurrentIndex(index);
    ui->comboBox_func->setOri_index(index);
    ui->comboBox_func->blockSignals(false);
}

void WidgetDISetting::set_type_byConfigure(int index)
{
    ui->comboBox_type->blockSignals(true);
    ui->comboBox_type->setCurrentIndex(index);
    ui->comboBox_type->setOri_index(index);
    ui->comboBox_type->blockSignals(false);
}

void WidgetDISetting::set_maxDist_byConfigure(double arg)
{
    ui->doubleSpinBox_maxDist->blockSignals(true);
    ui->doubleSpinBox_maxDist->setValue(arg);
    ui->doubleSpinBox_maxDist->setOriValue(arg);
    ui->doubleSpinBox_maxDist->blockSignals(false);
}

void WidgetDISetting::set_minDist_byConfigure(double arg)
{
    ui->doubleSpinBox_minDist->blockSignals(true);
    ui->doubleSpinBox_minDist->setValue(arg);
    ui->doubleSpinBox_minDist->setOriValue(arg);
    ui->doubleSpinBox_minDist->blockSignals(false);
}

void WidgetDISetting::set_rang_byConfigure(double arg)
{
    ui->doubleSpinBox_range->blockSignals(true);
    ui->doubleSpinBox_range->setValue(arg);
    ui->doubleSpinBox_range->setOriValue(arg);
    ui->doubleSpinBox_range->blockSignals(false);
}

void WidgetDISetting::set_inverse_byConfigure(bool arg)
{
    ui->checkBox_inverse->blockSignals(true);
    ui->checkBox_inverse->setChecked(arg);
    ui->checkBox_inverse->setOriValue(arg);
    ui->checkBox_inverse->blockSignals(false);
}

ItemBase *WidgetDISetting::getRelateItem()
{
    return item_di_;
}

void WidgetDISetting::on_doubleSpinBox_range_valueChanged(double arg1)
{
    item_di_->data_di_.rang_ = arg1;

    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_range ->getIsClickedTrigger())
    {
        ui->doubleSpinBox_range->setNewValue(arg1);

        doubleSpinBoxCommand* dbspinbox_command = new doubleSpinBoxCommand(ui->doubleSpinBox_range,item_di_,
                                                                           ui->doubleSpinBox_range->getOriValue(),
                                                                           ui->doubleSpinBox_range->getNewValue(),
                                                                           this,NULL);
        m_unStack->push(dbspinbox_command);

        ui->doubleSpinBox_range->setOriValue(arg1);
        ui->doubleSpinBox_range->setIsClickedTrigger(true);
    }
    //更新扫描区域
    item_di_->getScanArea()->setRange(arg1);
    item_di_->getScanArea()->update();
    item_di_->update_item();
}
