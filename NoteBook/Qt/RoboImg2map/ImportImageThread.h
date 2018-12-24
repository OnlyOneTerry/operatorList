#ifndef IMPORTIMAGETHREAD_H
#define IMPORTIMAGETHREAD_H

#include <QThread>
#include <QImage>
#include <QRgb>
#include <QtDebug>
#include "Protobuf_Messages/message_map.pb.h"
#include "google/protobuf/util/json_util.h"
#include "google/protobuf/util/type_resolver.h"
#include "google/protobuf/util/type_resolver_util.h"
#include "PublicClass.h"

using namespace rbk::protocol;

class ImportImageThread : public QThread
{
    Q_OBJECT
public:
    explicit ImportImageThread(QObject *parent = 0);
    void run();
    ~ImportImageThread();
    //
    enum ThreadType{
        E_Import = 0//导入
    };
    ThreadType threadType() const;
    void setThreadType(const ThreadType &threadType);

    QImage *greyScale(QImage *origin);
    int resolution() const;
    void setResolution(int resolution);

    void setImportThread(const ThreadType &threadType,
                         qreal resolution,
                         qreal countScale,
                         QImage &image, const QString &filePath);
    void createJson(QImage &image);
    qreal mm2m(qreal mm_value);
    bool saveSmap(Message_Map &madata, const QString &path);
    void setPointMinAndMax(double x, double y);
    QString filePath() const;
    void setFilePath(const QString &filePath);

    void setBackgroundColor(const QString &colorStr);
    //    cv::Mat QImage2cvMat(QImage image);
    qreal q_3(qreal qvalue);
    QString getSaveFileName(QString fileName);
    qreal cm2m(qreal cm_value);
    void setStop(bool isStop);
    bool getIsSetStop() const;

signals:
    void sigThreadProgress(qint64 value,qint64 max);
public slots:
private:

    ThreadType _threadType;
    qreal _resolution;
    qreal _scale;
    QImage _importImage;
    QString _filePath;
    double x_min_value = 999999999;
    double y_min_value = 999999999;
    double x_max_value = -999999999;
    double y_max_value = -999999999;
    Message_Map map_data_;
    QStringList RList;
    QStringList GList;
    QStringList BList;
    QStringList AList;//透明度.
    bool _isSetStop = false;

};

#endif // IMPORTIMAGETHREAD_H
