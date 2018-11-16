#ifndef EDITROBOTMODELWIDGET_H
#define EDITROBOTMODELWIDGET_H

#include <QWidget>
#include<QVBoxLayout>
#include<QVariantMap>
#include <QGroupBox>
#include "ModelGraphics/ModelGraphicsView.h"
#include<QUndoStack>
#include"UndoStack.h"
namespace Ui {
class EditRobotModelWidget;
}
class ModelBasicSettingWidget;

class EditRobotModelWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditRobotModelWidget(QWidget *parent = 0);
    ~EditRobotModelWidget();
    bool saveFile(QString filePath,int index = 0,bool isGuid = false);
    bool saveAsOtherFile(bool isGuid);
    bool saveAsTemp(bool isGuid = false);//另存为模板
    bool saveAsTemp(QString name, bool isGuid = false);//另存为模板
    bool loadFile(QString filePath);
    bool load_from_byteArray(const QByteArray& byte_array);
    void init_new();
    bool isAllowSave(bool isGuid=false);//是否允许保存
    void setUndoStack(QUndoStack* stack);
    QUndoStack* getUndoStack();
    ModelGraphicsView* getGraphicView();
    QByteArray readFile(const QString &filePath);

    QString filePath() const;
    void setFilePath(const QString &filePath);
    QString fileName() const;
    void setFileName(QString& fileName);
    QString errorString() const;
    void setErrorString(const QString &errorString);

    bool isChanged() const;
    void setIsChanged(bool isChanged);
    bool getIsSaveSuccess();

private:
    QList<QVariantMap> get_device_infor(const int& type) const;
    void init_model_graphics();
private:
    Ui::EditRobotModelWidget *ui;
    QVariantMap robot_model_variant_map_;

    //用于保存主窗口的QundoStack指针，传递到子窗口中
    QUndoStack* m_undoStack;

    /*ModelGraphcisControl*/
private:
    ModelGraphicsView *model_view_=NULL;
    QString _filePath;
    QString _fileName;
    QFile _file;
    QString _errorString;
    bool _isChanged;
    bool _isSaveSuccess;//判断文件是否保存成功

};

#endif // EDITROBOTMODELWIDGET_H
