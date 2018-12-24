#include "ImportImageThread.h"
#include <QFile>
#include <QFileInfo>
#include "PublicClass.h"

ImportImageThread::ImportImageThread(QObject *parent) : QThread(parent)
{
    _threadType = E_Import;
}
ImportImageThread::~ImportImageThread()
{
}
void ImportImageThread::setBackgroundColor(const QString &colorStr)
{
    if(colorStr.isEmpty())return;
    QString str = colorStr;
    QStringList list = str.split(",");
    if(list.size()>=4){
        RList.append(list.at(0));
        GList.append(list.at(1));
        BList.append(list.at(2));
        AList.append(list.at(3));
    }
}
void ImportImageThread::setImportThread(const ThreadType &threadType,
                                        qreal resolution,
                                        qreal countScale,
                                        QImage &image,
                                        const QString &filePath)
{
    RList.clear();
    GList.clear();
    BList.clear();
    AList.clear();
    _filePath = filePath;
    _resolution = resolution;//默认0.02 m
    _scale = countScale;

    _importImage = image.scaled(image.width()*_scale,image.height()*_scale);
    _isSetStop = false;
    setThreadType(threadType);
}
ImportImageThread::ThreadType ImportImageThread::threadType() const
{
    return _threadType;
}

void ImportImageThread::setThreadType(const ThreadType &threadType)
{
    _threadType = threadType;
}
void ImportImageThread::run()
{
    switch (threadType()) {
    case E_Import:
    {
        createJson(_importImage);
    }
        break;
    default:
        break;
    }
}
void ImportImageThread::createJson(QImage &image)
{
    map_data_.Clear();

    x_min_value = 999999999;
    y_min_value = 999999999;
    x_max_value = -999999999;
    y_max_value = -999999999;

    emit sigThreadProgress(0,image.width()*image.height());
    qreal xTmp = 0.0;
    qreal yTmp = 0.0;
    QColor oldColor;
    Message_MapPos* map_pos = Q_NULLPTR;
    int imgW = image.width();
    int imgH = image.height();
    //遍历图片.
    for(int x = 0;x<imgW;++x){
        for(int y = 0;y<imgH;++y){
            if(_isSetStop){
                return;
            }
            oldColor = image.pixelColor(x,y);
            if(!oldColor.isValid()){
                continue;
            }
            if((RList.contains(QString::number(oldColor.red()))
                && GList.contains(QString::number(oldColor.green()))
                && BList.contains(QString::number(oldColor.blue())))
                    ||oldColor.alpha()<=20//透明度小于50的过滤.
                    )
            {
                continue;
            }
            map_pos = map_data_.add_normal_pos_list();
            //根据像素-获取实际距离.
            xTmp = x*_resolution*1000.0;
            yTmp = -y*_resolution*1000.0;
            //最大点，最小点单位是m，所以不用转化。.
            setPointMinAndMax(q_3((xTmp)),q_3((yTmp)));
            map_pos->set_x(q_3(mm2m(xTmp)));
            map_pos->set_y(q_3(mm2m(yTmp)));

        }
        if(0==x%10){
            emit sigThreadProgress(x,imgW);
        }
    }
    //    _filePath = getSaveFileName(_filePath);
    _filePath +=".smap";
//    QFileInfo newFileInfo(_filePath);
//    QString newFilePath = QString("%1/%2.smap")
//            .arg(PublicClass::init()->getCurrentRobotRunningDir())
//            .arg(newFileInfo.completeBaseName());

    QFile file(_filePath);
    if(file.exists()){
        file.remove();
    }
    saveSmap(map_data_,_filePath);
//    _filePath = newFilePath;
    emit sigThreadProgress(image.width(),image.width());
}
//递归命名文件名，同名自动+1
QString ImportImageThread::getSaveFileName(QString fileName)
{
    static int index = 0;
    QFileInfo fileInfo(fileName);
    //    SCDebug<<"fileName: index:"<<index+1<<fileName;
    if(fileInfo.exists()){
        fileName = QString("%1/%2(%3)")
                .arg(fileInfo.absolutePath())
                .arg(fileInfo.completeBaseName())
                .arg(++index);

        fileName = getSaveFileName(fileName);
    }
    return fileName;
}
void ImportImageThread::setPointMinAndMax(double x, double y)
{
    if (x < x_min_value) {
        x_min_value = x;
    }

    if (x > x_max_value) {
        x_max_value = x;
    }

    if (y < y_min_value) {
        y_min_value = y;
    }

    if (y > y_max_value) {
        y_max_value = y;
    }
}

QString ImportImageThread::filePath() const
{
    return _filePath;
}
void ImportImageThread::setStop(bool isStop)
{
    _isSetStop = isStop;
}

bool ImportImageThread::getIsSetStop() const
{
    return _isSetStop;
}
void ImportImageThread::setFilePath(const QString &filePath)
{
    _filePath = filePath;
}
qreal ImportImageThread::mm2m(qreal mm_value)
{
    return qRound(mm_value)/1000.0;
}
qreal ImportImageThread::cm2m(qreal cm_value)
{
    return qRound(cm_value)/100.0;
}
qreal ImportImageThread::q_3(qreal qvalue)
{
    return  (qRound(1000*qvalue))/1000.0;
}
bool ImportImageThread::saveSmap(Message_Map &madata,const QString&path)
{
    //
    madata.mutable_header()->mutable_max_pos()->set_x(mm2m(x_max_value));
    madata.mutable_header()->mutable_max_pos()->set_y(mm2m(y_max_value));
    madata.mutable_header()->mutable_min_pos()->set_x(mm2m(x_min_value));
    madata.mutable_header()->mutable_min_pos()->set_y(mm2m(y_min_value));

    QString version = "";
    if(PublicClass::init()->map_version_supported_list_.size()>0){
        version = PublicClass::init()->map_version_supported_list_.last();
    }
    if(version.isEmpty()){
        version = "1.0.6";
    }
    madata.mutable_header()->set_version("1.0.6");
    double resolution = q_3(_resolution);

    madata.mutable_header()->set_resolution(resolution);
    madata.set_map_directory("");

    //================================
    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {

        //=======================
        //        QTextStream ofs(&file);
        google::protobuf::util::JsonOptions json_options;
        json_options.always_print_primitive_fields = false;
        std::string json_buffer;
        google::protobuf::util::MessageToJsonString(madata, &json_buffer, json_options);
        //        ofs << QByteArray::fromStdString(json_buffer);
        //        file.flush();
        file.write(json_buffer.c_str());
        file.close();

        //清理
        madata.Clear();
    }
    SCDebug<<"ImportImageThread::saveSmap:return true :save Path:"<<path;
    return true;
}
//图片转灰度值
QImage * ImportImageThread::greyScale(QImage * origin){

    QImage * newImage = new QImage(origin->width(), origin->height(), QImage::Format_ARGB32);
    QRgb * line;

    for(int y = 0; y<newImage->height(); y++){
        QRgb * line = (QRgb *)origin->scanLine(y);

        for(int x = 0; x<newImage->width(); x++){

            int average = (qRed(line[x]) + qGreen(line[x]) + qRed(line[x]))/3;
            newImage->setPixel(x,y, qRgb(average, average, average));
        }
    }

    return newImage;
}

int ImportImageThread::resolution() const
{
    return _resolution;
}

void ImportImageThread::setResolution(int resolution)
{
    _resolution = resolution;
}
