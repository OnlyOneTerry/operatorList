#include "GuidSimpleDevice.h"
#include "ui_GuidSimpleDevice.h"

GuidSimpleDevice::GuidSimpleDevice(QWidget *parent) :
    GuidBaseSetting(parent),
    ui(new Ui::GuidSimpleDevice)
{
    ui->setupUi(this);
    setType(GUID_SIMPLEDEVICE);
    initCombox(ui->comboBox_batteryType);
    initCombox(ui->comboBox_gyroType);
    initCombox(ui->comboBox_driverBrand);
}

GuidSimpleDevice::~GuidSimpleDevice()
{
    delete ui;
}

void GuidSimpleDevice::initWidgetData()
{

}

void GuidSimpleDevice::initCombox(QComboBox *box)
{
    QString filePath = CONFIGFILE_PATH;
    ConfigureData  configInfo;

    if(box == ui->comboBox_batteryType )
    {
        configInfo = readConfigure_BatteryType(filePath);
    }
    else if(box == ui->comboBox_gyroType )
    {
        configInfo = readConfigure_gyroType(filePath);
    }
    else if(box == ui->comboBox_driverBrand)
    {
        configInfo = readConfigure_drvierBrand(filePath);
    }

    int DefIndex = configInfo.defaultidx;

    QList<ItemData>::iterator iter = configInfo.dataList.begin();

    box->blockSignals(true);
    for(;iter!= configInfo.dataList.end();iter++)
    {
        box->addItem((*iter).describtion,(*iter).index);
    }
    int idx = box->findData(DefIndex,Qt::UserRole);
    box->setCurrentIndex(idx);
    box->blockSignals(false);
}

ConfigureData GuidSimpleDevice::readConfigure_drvierType(const QString &filePath)
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

    QJsonValue  mode_param_value= chassis_obj["mode_param"];
    QJsonObject mode_param_obj = mode_param_value.toObject();

    QJsonValue  driver_value = mode_param_obj["driver"];
    QJsonObject driver_obj = driver_value.toObject();

    ConfigureData  driverData;
    QJsonValue defaultIdex = driver_obj["defaultIndex"].toInt();
    driverData.defaultidx = defaultIdex.toInt();
    QJsonArray typeList = driver_obj["typeList"].toArray();

    for(int i = 0; i < typeList.size();i++)
    {
        QJsonObject  tempObj = typeList.at(i).toObject();
        QJsonValue  tempIndex = tempObj["index"].toInt();
        QJsonValue  tempDesc = tempObj["desc"].toString();
        ItemData  tempdata;
        tempdata.index = tempIndex.toInt();
        tempdata.describtion = tempDesc.toString();
        driverData.dataList.push_back(tempdata);
    }

    return driverData;
}

ConfigureData GuidSimpleDevice::readConfigure_gyroType(const QString &filePath)
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

    QJsonValue gyro_value = chassis_obj.value("gyro");
    QJsonObject gyro_obj = gyro_value.toObject();

    QJsonValue defindx = gyro_obj["defaultIndex"];


    QJsonArray typeList = gyro_obj["typeList"].toArray();

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

ConfigureData GuidSimpleDevice::readConfigure_BatteryType(const QString &filePath)
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

    QJsonValue battery_value = chassis_obj.value("batteryInfo");
    QJsonObject battery_obj = battery_value.toObject();

    QJsonValue defindx = battery_obj["defaultIndex"];

    QJsonArray typeList = battery_obj["typeList"].toArray();

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

ConfigureData GuidSimpleDevice::readConfigure_drvierBrand(const QString &filePath)
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

    QJsonValue  mode_param_value= chassis_obj["mode_param"];
    QJsonObject mode_param_obj = mode_param_value.toObject();

    QJsonValue  driver_value = mode_param_obj["driverBrand"];
    QJsonObject driver_obj = driver_value.toObject();

    ConfigureData  driverBrandData;
    QJsonValue defaultIdex = driver_obj["defaultIndex"].toInt();
    driverBrandData.defaultidx = defaultIdex.toInt();
    QJsonArray typeList = driver_obj["typeList"].toArray();

    for(int i = 0; i < typeList.size();i++)
    {
        QJsonObject  tempObj = typeList.at(i).toObject();
        QJsonValue  tempIndex = tempObj["index"].toInt();
        QJsonValue  tempDesc = tempObj["desc"].toString();
        ItemData  tempdata;
        tempdata.index = tempIndex.toInt();
        tempdata.describtion = tempDesc.toString();
        driverBrandData.dataList.push_back(tempdata);
    }

    return driverBrandData;
}

void GuidSimpleDevice::on_comboBox_batteryType_currentIndexChanged(int index)
{
    basicData_.battery_info_ = ui->comboBox_batteryType->currentData().toInt();
}

void GuidSimpleDevice::on_comboBox_gyroType_currentIndexChanged(int index)
{
    basicData_.gyro_type_ = ui->comboBox_gyroType->currentData().toInt();
    qDebug()<<"basicData_.gyro_type_----------------is "<<basicData_.gyro_type_;
}

void GuidSimpleDevice::on_comboBox_driverBrand_currentIndexChanged(int index)
{
    basicData_.driverBrand_ = ui->comboBox_driverBrand->currentText();
}
