#include "ImportImageWidget.h"
#include "ui_ImportImageWidget.h"
#include <QFileDialog>
#include <QtMath>
#include "EditorPublicClass.h"
#include "UiClass.h"
#include "SendGlobalMsg.h"
#include <QProcess>

ImportImageWidget::ImportImageWidget(QWidget *parent) :
    CustomChildBaseDialog(parent),
    ui(new Ui::ImportImageWidget)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0,1);
    this->d->showMinMaxButton();
    this->resize(1200,730);
}

ImportImageWidget::~ImportImageWidget()
{
    SCDebug;
    releaseAllItems();
    if(pEditorView){
        delete pEditorView;
        pEditorView = Q_NULLPTR;
    }
    UiClass::init()->releaseSCToastr(this);
    delete ui;
}

void ImportImageWidget::releaseAllItems()
{
    if(pEditorView && pEditorView->cScene()){
        pEditorView->cScene()->clearScene();
    }
    if(pZoomIn){
        delete pZoomIn;
        pZoomIn = Q_NULLPTR;
    }
    if(pImportImageThread){
        pImportImageThread->setStop(true);
        pImportImageThread->wait();
        pImportImageThread->exit();
        delete pImportImageThread;
        pImportImageThread = Q_NULLPTR;
    }
    ui->progressBar->setValue(0);
}
void ImportImageWidget::init()
{
    initEditViewToolBar();
    pEditorView = new EditorView(this);
    ui->horizontalLayout->addWidget(pEditorView);
    EditorPublicClass::init()->DoubleSpinBox = ui->doubleSpinBox_value;
    ui->progressBar->setValue(0);
    LineEditList.append(ui->lineEdit_bkColor1);
    LineEditList.append(ui->lineEdit_bkColor2);
    LineEditList.append(ui->lineEdit_bkColor3);
    foreach(auto var,LineEditList){
        QRegExp regExp("^[0-9]{0}([0-9]|[,])+$");
        QRegExpValidator *ev = new QRegExpValidator(regExp);
        var->setToolTip(tr("Enter a color (for example: 255,255,255,255)"));
        var->setValidator(ev);
        var->installEventFilter(this);
    }
    //--------------------------------------
    //    pQActionGroup->actions().at(1)->trigger();
}
//事件过滤，QLineEdit
bool ImportImageWidget::eventFilter(QObject *watched, QEvent *event)
{
    for(int i=0;i<LineEditList.size();++i){
        if( watched == LineEditList.at(i) )
        {
            if(event->type()==QEvent::FocusIn){
                _currentLineEditIndex = i;
                break;
            }
        }
    }
    return CustomChildBaseDialog::eventFilter(watched,event);
}

bool ImportImageWidget::willCloseWidget()
{
    if(0 != stopImportImage()){
        return false;
    }
    return true;
}

void ImportImageWidget::initEditViewToolBar()
{
    QActionGroup*group = new QActionGroup(this);
    connect(group,SIGNAL(triggered(QAction*)),
            this,SLOT(slotEditViewToolBarTriggered(QAction*)));
    ActionData *i0 = new ActionData;
    i0->_icon = "load_image_normal";
    i0->_id = 3;
    i0->_isCheckable = true;
    i0->_toolTip = tr("Load");
    i0->_statusTip =tr("Map name cannot contain Chinese characters");
    group->addAction(createAction(i0));

    ActionData *a0 = new ActionData;
    a0->_icon = "cursor_default_normal";
    a0->_id = 0;
    a0->_isCheckable = true;
    a0->_toolTip = tr("Selected");
    a0->_statusTip = tr("Select the line or area in the view");
    group->addAction(createAction(a0));

    ActionData *a2 = new ActionData;
    a2->_icon = "line_normal";
    a2->_id = 1;
    a2->_isCheckable = true;
    a2->_toolTip = tr("Line");
    a2->_statusTip = tr("The distance of the line is the pixel distance, and the value will be automatically filled into the edit box on the right");
    group->addAction(createAction(a2));

    ActionData *a1 = new ActionData;
    a1->_icon = "rectangle_normal";
    a1->_id = 2;
    a1->_toolTip = tr("Area");
    a1->_statusTip = tr("The selected area is the effective area when the map is loaded. When not selected, the default picture is the export effective area");
    a1->_isCheckable = true;
    group->addAction(createAction(a1));

    for(int i=0;i<group->actions().size();++i){
        QToolButton *tBtn = new QToolButton(this);
        tBtn->setDefaultAction(group->actions().at(i));
        tBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        ui->horizontalLayout_toolButton->addWidget(tBtn);
        ToolButtonList.append(tBtn);
    }
}
void ImportImageWidget::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<"ImportImageWidget::keyPressEvent";
    switch (event->key()) {
    case Qt::Key_Delete:
        pEditorView->cScene()->deleteSelection();
        break;
    default:
        break;
    }
    CustomChildBaseDialog::keyPressEvent(event);
}

void ImportImageWidget::slotEditViewToolBarTriggered(QAction* ac)
{
    int id = ac->data().value<ActionData*>()->_id;
    switch (id) {
    case 0://默认选中
    {
        pEditorView->cScene()->setMode("Select");
    }
        break;
    case 1:
    {
        pEditorView->cScene()->setMode("EditorLineItem");
    }
        break;
    case 2://截区域
    {
        pEditorView->cScene()->setMode("EditorRectangleItem");
    }
        break;
    case 3:
    {
        QString filePath = QFileDialog::getOpenFileName(this,
                                                        tr("Images"),
                                                        "",
                                                        QString(("Images(%1)"))
                                                        .arg(Load_image_filter));
        if (filePath.isEmpty())
        {
            return;
        }
        _filePath = filePath;
        pEditorView->loadPixmap(filePath);
    }
        break;
    default:
        break;
    }
}
QAction* ImportImageWidget::createAction(ActionData*data)
{
    QIcon icon(QString("%1/images/RoExImportMapPlugin/%2").arg(StyleObject::init()->getCurrentSkinDirPath()).arg(data->_icon));
    QAction *action = new QAction(icon, data->_text, this);

    QList<QKeySequence>keylist;//设置两个快捷键
    for(int i=0;i<data->_shortcuts.size();++i){
        keylist.append(data->_shortcuts.at(i));
    }
    action->setObjectName(data->_objectName);
    if(keylist.size()>0)
        action->setShortcuts(keylist);
    action->setText(data->_toolTip);
    action->setCheckable(data->_isCheckable);
    action->setData(QVariant::fromValue(data));
    action->setStatusTip(data->_statusTip);
    return action;
}

bool ImportImageWidget::isLineEditListEmpty()
{
    for(int i=0;i<LineEditList.size();++i){
        if(!LineEditList.at(i)->text().isEmpty()){
            return false;
        }
    }
    return true;
}
int ImportImageWidget::currentLineEditIndex()
{
    for(int i=0;i<LineEditList.size();++i){
        if(LineEditList.at(i)->hasFocus()){
            return i;
        }
    }
    return 0;
}
void ImportImageWidget::on_pushButton_startImport_clicked()
{
    if(!pImportImageThread){
        pImportImageThread = new ImportImageThread(this);
        connect(pImportImageThread,SIGNAL(finished()),this,SLOT(slotImportImageThread()));
        connect(pImportImageThread,SIGNAL(sigThreadProgress(qint64,qint64)),
                this,SLOT(slotThreadProgress(qint64,qint64)));
    }
    double actualLength = 0.02;
    if(ui->doubleSpinBox_ActualLength->value()>=0.000001){
        actualLength = ui->doubleSpinBox_ActualLength->value();
    }
    if(ui->doubleSpinBox_value->text().toDouble()<=0.000001){
        slotMessage(tr("Please draw a line and enter the actual distance"),SCToastr::E_Warning);
        return;
    }else if(isLineEditListEmpty()){
        slotMessage(tr("The background color cannot be empty"),SCToastr::E_Warning);
        return;
    }else if(pEditorView->cScene()->getSelectionImage().isNull()
             || pEditorView->cScene()->getSelectionImage().width()<= 0
             || pEditorView->cScene()->getSelectionImage().height()<=0)
    {
        slotMessage(tr("Please load a picture"),SCToastr::E_Warning);
        return;
    }
    if(!pImportImageThread->isRunning()){
        //比例 = 实际距离/像素距离
        double scaleValue = actualLength/ui->doubleSpinBox_value->text().toDouble();
        //需要转换的图片实际距离0.02 m/0.05m
        double imageRelution = ui->comboBox_smapRelution->currentText().toDouble();//0.02
        //比例除以0.02得到图片---放大倍数
        double scale1 = scaleValue/ui->comboBox_smapRelution->currentText().toDouble();

        SCDebug<<"scaleValue:"<<scaleValue
              <<"imageRelution:"<<imageRelution
             <<"scale1:"<<scale1;
        QImage image =  pEditorView->cScene()->getSelectionImage();
        //
        SCDebug<<"image->width()*scale1:"<<image.width()*scale1<<
                 "image->height()*scale1:"<<image.height()*scale1;

#ifdef Q_OS_WIN64

#else
        if(image.width()*scale1*image.height()*scale1>10000*8000){
            UiClass::init()->showDialog(tr("The actual distance is too small and the picture is too large"));
            return;
        }
#endif

        pImportImageThread->setImportThread(ImportImageThread::E_Import,
                                            imageRelution,
                                            scale1,
                                            image,
                                            _filePath);
        //因为setImportThread函数中clear了QList,所以要放到下面。
        for(int i=0;i<LineEditList.size();++i){
            if(!LineEditList.at(i)->text().isEmpty()){
                pImportImageThread->setBackgroundColor(LineEditList.at(i)->text());
            }
        }
        ui->pushButton_startImport->setText(tr("Stop"));
        pImportImageThread->start();
    }else{
        stopImportImage();
    }
}
int ImportImageWidget::stopImportImage()
{
    int index = -1;
    if(pImportImageThread&&pImportImageThread->isRunning()){
        index = UiClass::init()->showDialog(QString(tr("The data is currently being imported, are you sure to terminate?")),tr("Information"),0);
        switch (index) {
        case 0:
        {
            ui->pushButton_startImport->setText(tr("Import"));
            ui->progressBar->setValue(0);
            pImportImageThread->setStop(true);
            pImportImageThread->wait();
        }
            break;
        }
    }else{
        return 0;
    }
    return index;
}
void ImportImageWidget::setBackgroundPixmap(const QPixmap &pixmap)
{
    pEditorView->loadPixmap(pixmap);
}
void ImportImageWidget::slotImportImageThread()
{
    switch (pImportImageThread->threadType())
    {
    case ImportImageThread::E_Import:
    {
        if(!pImportImageThread->getIsSetStop()){
            ui->pushButton_startImport->setText(tr("Import"));
            SCDebug<<"ImportImageThread::E_Import:"<<pImportImageThread->filePath();
            QString filePath =  pImportImageThread->filePath();
            //            filePath = filePath.replace("/","\\");
            //            QProcess process;
            //            process.startDetached(QString(QStringLiteral("explorer.exe /select,%1")).arg(QString::fromLocal8Bit(filePath.toLocal8Bit())));

            PublicClass::init()->openDirSelectFile(filePath);
            //            SendGlobalMsg::init()->sendGlobalMsg(SendGlobalMsg::E_GS_ImportImageToOpenSMap,pImportImageThread->filePath());
            //            SendGlobalMsg::init()->sendGlobalMsg(SendGlobalMsg::E_GS_ChangedMainTab,"RoPlugins/TabPlugins/RobotGraphics.dll&0");
        }
    }
        break;
    default:
        break;
    }
}
void ImportImageWidget::setButtonStatus(bool isRunning)
{

}
void ImportImageWidget::slotThreadProgress(qint64 value,qint64 max)
{
    ui->progressBar->setMaximum(max);
    ui->progressBar->setValue(value);
}

void ImportImageWidget::on_pushButton_zoomIn_clicked()
{
    if(pZoomIn&&!pZoomIn->isHidden()){
        delete pZoomIn;
        pZoomIn = Q_NULLPTR;
        ui->pushButton_zoomIn->setText(tr("Pick Color"));
    }else{
        _currentLineEditIndex = currentLineEditIndex();
        if(!pZoomIn){
            pZoomIn = new ZoomIn();
            connect(pZoomIn,SIGNAL(sigShowMessage(QString,int)),this,
                    SLOT(slotMessage(QString,int)));
            connect(pZoomIn,SIGNAL(sigZoomIn(int ,const QString&)),
                    this,SLOT(slotZoomIn(int ,const QString&)));
        }
        ui->pushButton_zoomIn->setText(tr("Close Color Picker"));
        pZoomIn->show();
    }
}

void ImportImageWidget::slotZoomIn(int index,const QString& str)
{
    switch (index) {
    case 0:
    {
        LineEditList.at(_currentLineEditIndex)->setText(str);
    }
        break;
    case 1:
        pZoomIn->close();
        ui->pushButton_zoomIn->setText(tr("Pick the background color"));
        break;
    default:
        break;
    }
}
//type = 0 正常提示窗口 1警告窗口 2错误窗口
void ImportImageWidget::slotMessage(const QString&msg,int type)
{
    UiClass::init()->showToastr(msg,(SCToastr::SCToastrType)type,this);
}

///** 添加编辑框
// * @brief ImportImageWidget::on_pushButton_addLineEdit_clicked
// */
//void ImportImageWidget::on_pushButton_addLineEdit_clicked()
//{
//    int row = ui->gridLayout_2->rowCount();
//    ui->gridLayout_2->addWidget(new QLabel(QString(QStringLiteral("过滤图片背景色%1：")).arg(LineEditList.size())),row-1,0);
//    QLineEdit *lineEdit = new QLineEdit(this);
//    lineEdit->setPlaceholderText(QStringLiteral("选中自动填充背景色"));
//    LineEditList.append(lineEdit);
//    ui->gridLayout_2->addWidget(lineEdit,row-1,1);
//    ui->gridLayout_2->update();
//    SCDebug<<row;
//}
