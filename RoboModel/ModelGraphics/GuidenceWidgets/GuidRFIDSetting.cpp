#include "GuidRFIDSetting.h"
#include "ui_GuidRFIDSetting.h"
#include"StyleObject.h"
GuidRFIDSetting::GuidRFIDSetting(QWidget *parent) :
    GuidBaseSetting(parent),
    ui(new Ui::GuidRFIDSetting)
{
    ui->setupUi(this);
    setType(GUID_RFIDSENSOR);
    initWidgetData();
    ui->radioButton_set->setChecked(true);
   QString imagePath= StyleObject::init()->getCurrentSkinDirPath()+"/images/RoModelPlugin/";
   ui->label_image->setPixmap(QPixmap(imagePath+"RFID"));
   ui->label_introduction->setText(tr("The device is suitable for reading and writing card scene of robot in magnetic stripe navigation, and can be used to judge the position of robot in magnetic strip navigation."));
}

GuidRFIDSetting::~GuidRFIDSetting()
{
    delete ui;
}

void GuidRFIDSetting::initWidgetData()
{
    //初始化func类型
    ConfigureData typeData = readConfigure_RFIDType(CONFIGFILE_PATH);

    int idx = typeData.defaultidx;

    QList<ItemData> datalist = typeData.dataList;

    for(int i = 0; i < datalist.size();i++)
    {
        ui->comboBox_Type->addItem(datalist.at(i).describtion,datalist.at(i).index);
    }

    ui->comboBox_Type->setCurrentIndex(idx);
}

ConfigureData GuidRFIDSetting::readConfigure_RFIDType(const QString &filePath)
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

    QJsonValue RFID_value = chassis_obj.value("RFID");
    QJsonObject RFID_obj = RFID_value.toObject();

    QJsonValue defindx = RFID_obj["defaultIndex"];


    QJsonArray typeList = RFID_obj["typeList"].toArray();

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

void GuidRFIDSetting::on_radioButton_set_clicked()
{
    setConfigure(true);
    ui->comboBox_Type->setEnabled(true);
    emit sig_enableNextStep(true);
    //send to magenticSensorWid
    emit sig_set();
}

void GuidRFIDSetting::on_radioButton_ignore_clicked()
{
    setConfigure(false);

    ui->comboBox_Type->setEnabled(false);
    emit sig_enableNextStep(true);
    //send to magenticSensorWid
    emit sig_ignore();
    ui->comboBox_Type->setCurrentIndex(0);
    basicData_.RFID_ = ui->comboBox_Type->currentData().toInt();
}

void GuidRFIDSetting::on_comboBox_Type_currentIndexChanged(int index)
{
   basicData_.RFID_ = ui->comboBox_Type->currentData().toInt();
}
