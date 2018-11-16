#ifndef SCRobotFileModelWidget_H
#define SCRobotFileModelWidget_H

#include <QWidget>
#include <QButtonGroup>
#include <QToolButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include<QJsonDocument>
#include "EditRobotModelWidget.h"
#include<QUndoStack>
#include "RobotManager.h"
#include<QToolBar>
#include"DialogModelChose.h"
#include"WidgetGuidance.h"
#include"PropertyMissPromtWidget.h"
#include"DialogSaveAsTemplate.h"
#include<QMenu>
#include<QAction>
namespace Ui {
class SCRobotFileModelWidget;
}

class SCRobotFileModelWidget : public QWidget
{
    Q_OBJECT
    enum FileType
    {
        FILE_MODEL,//模型文件
        FILE_TEMP//模型模板
    };
public:
    SCRobotFileModelWidget(QWidget *parent = 0);
    ~SCRobotFileModelWidget();
    enum SCRobotFileModelWidgetType{
        E_SCRobotFileModelWidget = RobotInterface::E_CN_UserRole +13
    };
    void initUI();
    QButtonGroup *getButtonGroup() const;
    void setButtonGroup(QButtonGroup *value);

    void setToolButtonStyleBy(Qt::ToolButtonStyle m_style);
    QGroupBox *getGroupBox() const;
    void setGroupBox(QGroupBox *value);
    bool loadFile(const QString &filePath);//加载模型文件
    bool loadTemplate(const QString &filePath);//加载模板
    void set_file_existed(const bool& is_exsit);
    void setUndoStack(QUndoStack* stack);
    QUndoStack* getUndoStack();
    EditRobotModelWidget* getEidtRobotModelWidget();

    ConfigureData readConfigure_model(const QString& filePath);
    //获取读取文件的版本号
    bool check_modelfile_reasonable(const QString filePath);
    //过滤缺失冗余字段
    void filterDevice(QList<QString>& list);
    //获取标准模型文件keys
    void getModelKeys(const QString filePath,QList<QString>& list);
    //递归json获取标准模型文件key功能函数
    void getKeys(QJsonObject& rootObj,QString parent,QList<QString>& list);
    //获取当前模型文件比标准模型文件多余的Keys
    QList<QString> getExtralKeys(QList<QString> &current,QList<QString> &standard);
    //获取加载模型文件的keys
    void getLoadModelKeys(const QString filePath,QList<QString>& list);
    //递归json获取加载模型文件key功能函数
    void getLoadKeysFunc(QJsonObject& rootObj,QString parent,QList<QString>& list);
    bool willCloseWidget();
    //根据连接状态及模型文件是否存在设置上传下载状态
    void setUpandDownloadBtnEnable(bool isConnected);
    void setCurrentFilePath(QString path);
    QString getCurrentFilePath();

    void init();
    void updateToolTipWidget();
    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent *event);

    void newModel();
    void updateToolButtonStyle(int index);
    //属性框自适应
    void fitPropertyWidget();
    void keyPressEvent(QKeyEvent *event);
    //是否允许继续执行
    bool isAllowExec();
    //如果是引导创建则判断是否可以保存
    bool isGuidenceFinished();
    //获取机器人目录下模型文件数量
    QStringList getModelNames();
    //是否是引导创建
    bool getIsGeneratbyGuid();
    void setIsGeneratbyGuid(bool guid);
public slots:
    void slotMessage(const QString &msg, int type = 0, bool isMask = false);
    void slotGuidNewModel();
    void slotNewModel();
    void slotLoadModel();
    void slotSaveModel();
    void slotSaveModelAs();
    void slotSaveAsTemp();
    void slotOpenTemp();
    void slotClose();
    void slotUpload();//上传
    void slotDownload();//下载
    void slotEnableSaveMode();
    void slot_Prompt_close();//关闭异常字段提示框
    void slotStatusTcpReceiveAll(const QString &ip, const QString &sCD, const QVariant &jsonData, int number, bool isError);
    void slotConfigTcpReceiveAll(const QString &ip, const QString &sCD, const QVariant &jsonData, int number, bool isError);
    void slotRoTcpManagerStatus(const QString &ip, RobotInterface::RobotCellStatus status);
    void slotSynchronizeData();
    void slotSendGlobalMsgResult(int sgs, const QVariant &var, bool isError);
    void slot_choseConfirm();
    void slot_redo_clicked();
    void slot_undo_clicked();
    void slotToolBarRobotControlMenu(bool trigger);
    void slotRoboModelContextMenuRequested(const QPoint& point);
    void slotNewModelbyDefault();
    void slotNewModelbyGudience();
    void slotNewModelbyTemplate();
    void slotOpenTempLocation();//打开模板文件所在目录
    void slot_saveAsTempConfirm(QString name);
    void slot_saveASTempCancel();

protected:
    void closeEvent(QCloseEvent *event);
signals:
    void sigClosedWidget();
private:
    Ui::SCRobotFileModelWidget *ui;
    QButtonGroup *ButtonGroup = Q_NULLPTR;
    EditRobotModelWidget*pEditRobotModelWidget = Q_NULLPTR;
    QGroupBox *GroupBox = Q_NULLPTR;
    QMenu*  m_newModelMenu;//新建菜单
    QMenu*  m_tempMenu;//模板相关
    QToolBar*  m_toolBar;
    QAction*  m_act_newModel;
    QAction*  m_act_loadModel;
    QAction*  m_act_saveModel;
    QAction*  m_act_openTemplate;//打开模板
    QAction*  m_act_openTempLocation;//打开模板
    QAction*  m_act_saveModelAs;
    QAction*  m_act_upload;
    QAction*  m_act_saveAsTemplate;
    QAction*  m_act_Template;


    //新建模型相关
    QAction* m_newByDefault;//使用默认
    QAction* m_newByGuidence;//使用引导
    QAction* m_newByTemplate;//使用模板

    QVBoxLayout* m_mainLayout ;
    QSpacerItem* m_spacer;
    QWidget* m_occupation;
    QHBoxLayout* m_occupLayout;
    ///撤销 前进
    QAction *undoAction = Q_NULLPTR;
    QAction *redoAction = Q_NULLPTR;
    //用于保存主窗口的QundoStack指针，传递到子窗口中
    QUndoStack* m_undoStack = Q_NULLPTR;
    //上传下载相关
    bool _isSaved;//判断是否已保存
    QString m_currentFilePath;

    //判断读取的模型文件是否合法
    QMap<QString,bool> model_containKeyMap;
    bool m_isModelFileReasonable;

    qint64 _toolTipWidgetKey = 0;
    bool _isShowToolTipWidget = false;

    QComboBox* _combx_models;
    DialogModelChose* _modelchoseDlg;
    QList<QString> _lostKey;//缺失的字段
    //控制按钮样式，暂时保留
    QMenu* _pToolBarModelMenu;
    int _currentToolButtonStyleIndex = 0 ;
    QList<Qt::ToolButtonStyle>ToolButtonStyleList;
    WidgetGuidance* _guidenceWidget;//引导窗口
    PropertyMissPromtWidget* _promptWid;
    DialogSaveAsTemplate* _saveTempWid;
    QWidget* _maskWid;
    QString _missInforString;//缺失字段
    QString _extralInforString;//多余字段
    //最新模板所有设备key
    QList<QString> standardModelKeys_list;
    //当前模型文件所有keys
    QList<QString> currentModelKeys_list;
    //获取当前模型文件比标准模型文件多余的Keys
    QList<QString> extralKeys_list;
    //缺失的keys
    QList<QString> missKeys_list;

    //用于超声，磁传感器，di,do的编号重复检测
    QList<int> DO_id_list;
    QList<int> DI_id_list;
    QList<int> Magnetic_id_list;
    QList<int> Ultrasonic_id_list;
    //编号重复提示
    QList<QString> _repeatPromtList;
    //当前文件类型
    FileType _filetype;
    //
    bool _is_Ctrl_Key_pressed = false;
    bool _isGeneratedByGuidence=false;//是否是引导创建
};


#endif // SCRobotFileModelWidget_H
