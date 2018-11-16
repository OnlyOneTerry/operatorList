#include "WidgetUltrasonicSetting.h"
#include "ui_WidgetUltrasonicSetting.h"
#include "ItemUltrasonic.h"
#include "ModelGraphicsScene.h"
#include "ModelGraphicsView.h"
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include "PublicClass.h"
#include"ItemAngelCtroller.h"
#include "ModelGraphicsView.h"
#include"ItemScanArea.h"

WidgetUltrasonicSetting::WidgetUltrasonicSetting(ItemUltrasonic* iu,QWidget *parent) :
    WidgetSetting(parent),
    item_ultrasonic_(iu),
    ui(new Ui::WidgetUltrasonicSetting)
{
    ui->setupUi(this);
    setType(WIDGET_ULTRASIONIC);
    initWigetData();
    ui->doubleSpinBox_r->setDecimals(3);
    ui->doubleSpinBox_range->setDecimals(3);
//    ui->spinBox_id->setMaximum(item_ultrasonic_->model_scene()->model_view()->basic_widget()->model_base_data_.max_ultrasonic_num_-1);
    connect(item_ultrasonic_->model_scene()->model_view()->basic_widget(),SIGNAL(sig_max_num_changed(quint8,quint8)),this,SLOT(slot_max_mum_changed(quint8,quint8)));
    connect(&item_ultrasonic_->timer,SIGNAL(timeout()),this,SLOT(on_timer_out()));
}

WidgetUltrasonicSetting::~WidgetUltrasonicSetting()
{
    delete ui;
}

void WidgetUltrasonicSetting::update_value ()
{
    ui->spinBox_id->blockSignals(true);
    ui->spinBox_id->setValue (item_ultrasonic_->data_ultrasonic_.id_);
    ui->spinBox_id->blockSignals(false);
    ui->doubleSpinBox_x->blockSignals(true);
    ui->doubleSpinBox_x->setValue (item_ultrasonic_->data_ultrasonic_.x_);
    ui->doubleSpinBox_x->blockSignals(false);
    ui->doubleSpinBox_y->blockSignals(true);
    ui->doubleSpinBox_y->setValue (item_ultrasonic_->data_ultrasonic_.y_);
    ui->doubleSpinBox_y->blockSignals(false);
    ui->doubleSpinBox_z->blockSignals(true);
    ui->doubleSpinBox_z->setValue (item_ultrasonic_->data_ultrasonic_.z_);
    ui->doubleSpinBox_z->blockSignals(false);
    ui->doubleSpinBox_r->blockSignals(true);
    ui->doubleSpinBox_r->setValue (item_ultrasonic_->data_ultrasonic_.r_);
    ui->doubleSpinBox_r->blockSignals(false);
    ui->comboBox_func->blockSignals(true);
    ui->comboBox_func->setCurrentIndex (item_ultrasonic_->data_ultrasonic_.func_);
    ui->comboBox_func->blockSignals(false);
    ui->comboBox_func->setOri_index(item_ultrasonic_->data_ultrasonic_.func_);
    ui->doubleSpinBox_minDist->blockSignals(true);
    ui->doubleSpinBox_minDist->setValue (item_ultrasonic_->data_ultrasonic_.min_dist_);
    ui->doubleSpinBox_minDist->blockSignals(false);
    ui->doubleSpinBox_maxDist->blockSignals(true);
    ui->doubleSpinBox_maxDist->setValue (item_ultrasonic_->data_ultrasonic_.max_dist_);
    ui->doubleSpinBox_maxDist->blockSignals(false);
    ui->doubleSpinBox_range->blockSignals(true);
    ui->doubleSpinBox_range->setValue (item_ultrasonic_->data_ultrasonic_.range_);
    ui->doubleSpinBox_range->blockSignals(false);
}

void WidgetUltrasonicSetting::set_r(const qreal &angel)
{
    //非鼠标点击改变编辑框改变值，阻塞改变消息
    ui->doubleSpinBox_r->blockSignals(true);
    ui->doubleSpinBox_r->setValue(angel);
    ui->doubleSpinBox_r->blockSignals(false);
    //更新ultrasonic图标角度
    item_ultrasonic_->data_ultrasonic_.r_=angel;
    item_ultrasonic_->model_scene()->model_view()->update();
}

ConfigureData WidgetUltrasonicSetting::readFuncTypeFile(const QString filePath)
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

    QJsonValue  ultrasonic_value= chassis_obj["ultrasonic"];
    QJsonObject ultrasonic_obj = ultrasonic_value.toObject();

    QJsonValue  func_value = ultrasonic_obj.value("func");
    QJsonObject  func_obj  = func_value.toObject();


    ConfigureData  cfData;

    QJsonValue defaultIdex = func_obj["defaultIndex"];

    cfData.defaultidx = defaultIdex.toInt();
    QJsonArray typeList = func_obj["typeList"].toArray();


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

void WidgetUltrasonicSetting::initWigetData()
{

    ConfigureData funcItemData = readFuncTypeFile(CONFIGFILE_PATH);

    int defindex = funcItemData.defaultidx;

    QList<ItemData> itedatalist = funcItemData.dataList;

    ui->comboBox_func->blockSignals(true);
    for(int i = 0; i < itedatalist.size();i++)
    {
        ui->comboBox_func->addItem(itedatalist.at(i).describtion,itedatalist.at(i).index);
    }

    if(item_ultrasonic_->data_ultrasonic_.func_!=funcItemData.defaultidx)//若类型不是默认类型则说明是读取已存在的模型
    {
        ui->comboBox_func->setCurrentIndex(item_ultrasonic_->data_ultrasonic_.func_);
    }
    else
    {
        ui->comboBox_func->setCurrentIndex(defindex);
    }
    ui->comboBox_func->blockSignals(false);
    ui->comboBox_func->setOri_index(defindex);

    ui->spinBox_id->blockSignals(true);
    ui->spinBox_id->setValue (item_ultrasonic_->data_ultrasonic_.id_);
    ui->spinBox_id->setOriValue(item_ultrasonic_->data_ultrasonic_.id_);
    ui->spinBox_id->blockSignals(false);
    ui->doubleSpinBox_x->blockSignals(true);
    ui->doubleSpinBox_x->setValue (item_ultrasonic_->data_ultrasonic_.x_);
    ui->doubleSpinBox_x->setOriValue(item_ultrasonic_->data_ultrasonic_.x_);
    ui->doubleSpinBox_x->blockSignals(false);
    ui->doubleSpinBox_y->blockSignals(true);
    ui->doubleSpinBox_y->setValue (item_ultrasonic_->data_ultrasonic_.y_);
    ui->doubleSpinBox_y->setOriValue(item_ultrasonic_->data_ultrasonic_.y_);
    ui->doubleSpinBox_y->blockSignals(false);
    ui->doubleSpinBox_z->blockSignals(true);
    ui->doubleSpinBox_z->setValue (item_ultrasonic_->data_ultrasonic_.z_);
    ui->doubleSpinBox_z->setOriValue (item_ultrasonic_->data_ultrasonic_.z_);
    ui->doubleSpinBox_z->blockSignals(false);
    ui->doubleSpinBox_r->blockSignals(true);
    ui->doubleSpinBox_r->setValue (item_ultrasonic_->data_ultrasonic_.r_);
    ui->doubleSpinBox_r->setOriValue (item_ultrasonic_->data_ultrasonic_.r_);
    item_ultrasonic_->getAngelCtroller()->setOriAngel(item_ultrasonic_->data_ultrasonic_.r_);
    ui->doubleSpinBox_r->blockSignals(false);
    ui->doubleSpinBox_minDist->blockSignals(true);
    ui->doubleSpinBox_minDist->setValue (item_ultrasonic_->data_ultrasonic_.min_dist_);
    ui->doubleSpinBox_minDist->setOriValue (item_ultrasonic_->data_ultrasonic_.min_dist_);
    ui->doubleSpinBox_minDist->blockSignals(false);
    ui->doubleSpinBox_maxDist->blockSignals(true);
    ui->doubleSpinBox_maxDist->setValue (item_ultrasonic_->data_ultrasonic_.max_dist_);
    ui->doubleSpinBox_maxDist->setOriValue (item_ultrasonic_->data_ultrasonic_.max_dist_);
    ui->doubleSpinBox_maxDist->blockSignals(false);
    ui->doubleSpinBox_range->blockSignals(true);
    ui->doubleSpinBox_range->setValue (item_ultrasonic_->data_ultrasonic_.range_);
    ui->doubleSpinBox_range->setOriValue (item_ultrasonic_->data_ultrasonic_.range_);
    ui->doubleSpinBox_range->blockSignals(false);

}

void WidgetUltrasonicSetting::on_spinBox_id_valueChanged(int arg1)
{

    qDebug()<<"---------------------------111-------------";
    if(item_ultrasonic_->model_scene()->get_id_list(1).contains(QString::number(arg1)))
    {
        if(arg1 != item_ultrasonic_->data_ultrasonic_.id_)
        {
            ui->label_Prompt->setText(tr("The Id already exists"));
            ui->label_Prompt->setStyleSheet("color:red;");
        }
        else
        {
            item_ultrasonic_->data_ultrasonic_.id_=arg1;
            item_ultrasonic_->update_item ();
            ui->label_Prompt->setText(tr("Setup success"));
            ui->label_Prompt->setStyleSheet("color::green;");
        }
        return;
    }
    else
    {
        item_ultrasonic_->data_ultrasonic_.id_=arg1;
        item_ultrasonic_->update_item ();
        ui->label_Prompt->setText(tr("Setup success"));
        ui->label_Prompt->setStyleSheet("color::green;");
    }

    if(m_unStack==NULL)
    {
        return;
    }
    qDebug()<<"---------------------------222-------------";
    if(ui->spinBox_id->getIsClickedTrigger())
    {
        ui->spinBox_id->setNewValue(arg1);

        SpinBoxCommand* sp_command = new SpinBoxCommand(ui->spinBox_id,ui->spinBox_id->getOriValue(),
                                                        ui->spinBox_id->getNewValue(),this,NULL);

        m_unStack->push(sp_command);
        qDebug()<<"---------------------------333-------------";

        ui->spinBox_id->setOriValue(arg1);
    }

    ui->spinBox_id->setIsClickedTrigger(true);
}

void WidgetUltrasonicSetting::on_doubleSpinBox_x_valueChanged(double arg1)
{
    item_ultrasonic_->data_ultrasonic_.x_=arg1;

    item_ultrasonic_->update_item ();

    if(m_unStack ==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_x->getIsClickedTrigger())
    {
        ui->doubleSpinBox_x->setNewValue(arg1);
        doubleSpinBoxCommand* dbsp_command = new doubleSpinBoxCommand(ui->doubleSpinBox_x,NULL,
                                                                      ui->doubleSpinBox_x->getOriValue(),
                                                                      ui->doubleSpinBox_x->getNewValue(),
                                                                      this,NULL);
        m_unStack->push(dbsp_command);
        ui->doubleSpinBox_x->setOriValue(arg1);

    }

    ui->doubleSpinBox_x->setIsClickedTrigger(true);
}

void WidgetUltrasonicSetting::on_doubleSpinBox_y_valueChanged(double arg1)
{
    item_ultrasonic_->data_ultrasonic_.y_=arg1;

    item_ultrasonic_->update_item ();

    if(m_unStack ==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_y->getIsClickedTrigger())
    {
        ui->doubleSpinBox_y->setNewValue(arg1);
        doubleSpinBoxCommand* dbsp_command = new doubleSpinBoxCommand(ui->doubleSpinBox_y,NULL,
                                                                      ui->doubleSpinBox_y->getOriValue(),
                                                                      ui->doubleSpinBox_y->getNewValue(),
                                                                      this,NULL);
        m_unStack->push(dbsp_command);
        ui->doubleSpinBox_y->setOriValue(arg1);

    }

    ui->doubleSpinBox_y->setIsClickedTrigger(true);
}

void WidgetUltrasonicSetting::on_doubleSpinBox_z_valueChanged(double arg1)
{
    item_ultrasonic_->data_ultrasonic_.z_=arg1;

    if(m_unStack ==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_z->getIsClickedTrigger())
    {
        ui->doubleSpinBox_z->setNewValue(arg1);
        doubleSpinBoxCommand* dbsp_command = new doubleSpinBoxCommand(ui->doubleSpinBox_z,NULL,
                                                                      ui->doubleSpinBox_z->getOriValue(),
                                                                      ui->doubleSpinBox_z->getNewValue(),
                                                                      this,NULL);
        m_unStack->push(dbsp_command);
        ui->doubleSpinBox_z->setOriValue(arg1);

    }

    ui->doubleSpinBox_z->setIsClickedTrigger(true);
}

void WidgetUltrasonicSetting::on_doubleSpinBox_r_valueChanged(double arg1)
{
    item_ultrasonic_->data_ultrasonic_.r_=arg1;

    item_ultrasonic_->update_item ();

    if(m_unStack ==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_r->getIsClickedTrigger())
    {
        ui->doubleSpinBox_r->setNewValue(arg1);
        doubleSpinBoxCommand* dbsp_command = new doubleSpinBoxCommand(ui->doubleSpinBox_r,NULL,
                                                                      ui->doubleSpinBox_r->getOriValue(),
                                                                      ui->doubleSpinBox_r->getNewValue(),
                                                                      this,NULL);
        m_unStack->push(dbsp_command);
        ui->doubleSpinBox_r->setOriValue(arg1);

    }

    ui->doubleSpinBox_r->setIsClickedTrigger(true);

    item_ultrasonic_->getScanArea()->setPos(6*qCos((arg1)*M_PI/180.),6*qSin((arg1)*M_PI/180.));
    item_ultrasonic_->getScanArea()->setRotation(90+arg1);
    item_ultrasonic_->getScanArea()->update();
    item_ultrasonic_->getAngelCtroller()->set_rotation(arg1);
}

void WidgetUltrasonicSetting::on_doubleSpinBox_minDist_valueChanged(double arg1)
{
    ui->doubleSpinBox_maxDist->setMinimum(arg1);
    item_ultrasonic_->data_ultrasonic_.min_dist_=arg1;
    if(m_unStack ==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_minDist ->getIsClickedTrigger())
    {
        ui->doubleSpinBox_minDist->setNewValue(arg1);
        doubleSpinBoxCommand* dbsp_command = new doubleSpinBoxCommand(ui->doubleSpinBox_minDist,NULL,
                                                                      ui->doubleSpinBox_minDist->getOriValue(),
                                                                      ui->doubleSpinBox_minDist->getNewValue(),
                                                                      this,NULL);
        m_unStack->push(dbsp_command);
        ui->doubleSpinBox_minDist->setOriValue(arg1);

    }

    ui->doubleSpinBox_minDist->setIsClickedTrigger(true);
    item_ultrasonic_->getScanArea()->setMinDist(arg1);
}

void WidgetUltrasonicSetting::on_doubleSpinBox_maxDist_valueChanged(double arg1)
{
    ui->doubleSpinBox_minDist->setMaximum(arg1);
    item_ultrasonic_->data_ultrasonic_.max_dist_=arg1;

    if(m_unStack ==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_maxDist ->getIsClickedTrigger())
    {
        ui->doubleSpinBox_maxDist->setNewValue(arg1);
        doubleSpinBoxCommand* dbsp_command = new doubleSpinBoxCommand(ui->doubleSpinBox_maxDist,NULL,
                                                                      ui->doubleSpinBox_maxDist->getOriValue(),
                                                                      ui->doubleSpinBox_maxDist->getNewValue(),
                                                                      this,NULL);
        m_unStack->push(dbsp_command);
        ui->doubleSpinBox_maxDist->setOriValue(arg1);

    }

    ui->doubleSpinBox_maxDist->setIsClickedTrigger(true);

    item_ultrasonic_->getScanArea()->setMaxDist(arg1);
    item_ultrasonic_->getScanArea()->update();
}

void WidgetUltrasonicSetting::on_doubleSpinBox_range_valueChanged(double arg1)
{
    item_ultrasonic_->data_ultrasonic_.range_=arg1;

    if(m_unStack ==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_maxDist ->getIsClickedTrigger())
    {
        ui->doubleSpinBox_range->setNewValue(arg1);
        doubleSpinBoxCommand* dbsp_command = new doubleSpinBoxCommand(ui->doubleSpinBox_range,NULL,
                                                                      ui->doubleSpinBox_range->getOriValue(),
                                                                      ui->doubleSpinBox_range->getNewValue(),
                                                                      this,NULL);
        m_unStack->push(dbsp_command);
        ui->doubleSpinBox_range->setOriValue(arg1);

    }

    ui->doubleSpinBox_range->setIsClickedTrigger(true);

    item_ultrasonic_->getScanArea()->setRange(arg1);
    item_ultrasonic_->getScanArea()->update();
}

void WidgetUltrasonicSetting::on_comboBox_func_currentIndexChanged(int index)
{
    item_ultrasonic_->data_ultrasonic_.func_=index;

    if(m_unStack ==NULL)
    {
        return;
    }

    if(ui->comboBox_func->getIsClickedTrigger())
    {
        ui->comboBox_func->setNew_index(index);
        customboxCommand* comb_command   = new customboxCommand(ui->comboBox_func,ui->comboBox_func->getOri_index(),
                                                                ui->comboBox_func->getNew_index(),this,NULL);
        m_unStack->push(comb_command);
        ui->comboBox_func->setOri_index(index);

    }

    ui->comboBox_func->setIsClickedTrigger(true);

}

void WidgetUltrasonicSetting::slot_max_mum_changed(quint8 type, quint8 max_value)
{
    if(type==1)
    {
        ui->spinBox_id->setMaximum(max_value-1);
    }

}


void WidgetUltrasonicSetting::on_spinBox_id_editingFinished()
{
    int arg1=ui->spinBox_id->value();

    if(! item_ultrasonic_->model_scene()->get_id_list(1).contains(QString::number(arg1)))
    {
        item_ultrasonic_->data_ultrasonic_.id_=arg1;
        item_ultrasonic_->update_item ();
        ui->label_Prompt->setText(tr("Setup success"));
        ui->label_Prompt->setStyleSheet("color::green;");
    }
    else
    {
        if(arg1 != item_ultrasonic_->data_ultrasonic_.id_)
        {
            ui->spinBox_id->setValue(item_ultrasonic_->data_ultrasonic_.id_);
            ui->label_Prompt->setText(tr("The Id already exists"));
            ui->label_Prompt->setStyleSheet("color:red;");
        }
        else
        {
            ui->label_Prompt->setText(tr("Setup success"));
            ui->label_Prompt->setStyleSheet("color::green;");
        }
    }
    //500毫秒后隐藏
    item_ultrasonic_->timer.setInterval(500);
    item_ultrasonic_->timer.setSingleShot(true);
    item_ultrasonic_->timer.start();
}

void WidgetUltrasonicSetting::on_timer_out()
{
    ui->label_Prompt->setText("");
}

void WidgetUltrasonicSetting::set_func_byGuidConfigure(int idex)
{
    ui->comboBox_func->blockSignals(true);
    ui->comboBox_func->setCurrentIndex(idex);
    ui->comboBox_func->setOri_index(idex);
    ui->comboBox_func->blockSignals(false);
}

void WidgetUltrasonicSetting::set_range_byGuidConfigure(double arg)
{
    ui->doubleSpinBox_range->blockSignals(true);
    ui->doubleSpinBox_range->setValue(arg);
    ui->doubleSpinBox_range->setOriValue(arg);
    ui->doubleSpinBox_range->blockSignals(false);
}

void WidgetUltrasonicSetting::set_minDist_byGuidConfigure(double arg)
{
    ui->doubleSpinBox_minDist->blockSignals(true);
    ui->doubleSpinBox_minDist->setValue(arg);
    ui->doubleSpinBox_minDist->setOriValue(arg);
    ui->doubleSpinBox_minDist->blockSignals(false);
}

void WidgetUltrasonicSetting::set_maxDist_byGuidConfigure(double arg)
{
    ui->doubleSpinBox_maxDist->blockSignals(true);
    ui->doubleSpinBox_maxDist->setValue(arg);
    ui->doubleSpinBox_maxDist->setOriValue(arg);
    ui->doubleSpinBox_maxDist->blockSignals(false);
}

void WidgetUltrasonicSetting::set_Type_byGuidConfigure(int idex)
{
    item_ultrasonic_->model_scene()->model_view()->basic_widget()->getUltrasonicType()->blockSignals(true);
    item_ultrasonic_->model_scene()->model_view()->basic_widget()->getUltrasonicType()->setCurrentIndex(idex);
    item_ultrasonic_->model_scene()->model_view()->basic_widget()->getUltrasonicType()->blockSignals(false);
}

ItemBase *WidgetUltrasonicSetting::getRelateItem()
{
    return item_ultrasonic_;
}
