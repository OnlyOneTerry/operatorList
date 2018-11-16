#include "DialogModelChose.h"
#include "ui_DialogModelChose.h"
#include<QJsonDocument>
#include<QJsonValue>
#include<QJsonObject>
#include<QJsonArray>
#include"RoLog.h"
DialogModelChose::DialogModelChose(QWidget *parent) :
    CustomChildBaseDialog(parent),
    ui(new Ui::DialogModelChose)
{
    ui->setupUi(this);
    setWindowTitle(tr("Template"));
//    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowCloseButtonHint);
    //读取可默认已配置模型文件
    loadMaps(DEFAULTCONFIGMODEL_PATH);
}

DialogModelChose::~DialogModelChose()
{
    delete ui;
}

ConfigureData DialogModelChose::readConfigure_model(const QString &filePath)
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

QComboBox *DialogModelChose::getCombox()
{
    return ui->comboBox_models;
}

void DialogModelChose::loadMaps(QString path)
{
    QFileInfoList list = GetFileList(path);
    addFileToCombox(list);
}

void DialogModelChose::addFileToCombox(QFileInfoList list)
{
    ui->comboBox_models->clear();
    ui->comboBox_models->addItem("default");

    for(int i = 0; i < list.size();i++)
    {
        ui->comboBox_models->addItem(list.at(i).fileName().remove(".tmp"));
    }
}

QFileInfoList DialogModelChose::GetFileList(QString path)
{
    SCDebug<<"path is 1======================"<<path;
    QDir dir(path);
    //查看路径中后缀为.model格式的文件
    QStringList filters;
    filters<<QString("*.tmp");
    QFileInfoList file_list = dir.entryInfoList(filters, QDir::Files | QDir::Hidden | QDir::NoSymLinks);//设置要读取的文件格式
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i = 0; i != folder_list.size(); i++)
    {
        QString name = folder_list.at(i).absoluteFilePath();
        QFileInfoList child_file_list = GetFileList(name);
        file_list.append(child_file_list);
    }
    return file_list;
}

void DialogModelChose::on_btn_ok_clicked()
{
    emit sig_confirm();
    close();
}

void DialogModelChose::on_btn_cancel_clicked()
{
    close();
}
