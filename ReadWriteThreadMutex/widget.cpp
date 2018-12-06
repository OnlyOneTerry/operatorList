#include "widget.h"
#include "ui_widget.h"
#include "PublicThreadClass.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //    PublicThreadClass::instance(); //在主线程中初始化单例类
    //    _pReadThread = new ReadThread(this);
    //    _pWriteThread = new WriteThread(this);

    createJumpList();
    createTaskbar();
    createThumbnailToolBar();

    //    pTimer = new QTimer(this);
    //    connect(pTimer,SIGNAL(timeout()),this,SLOT(slotTimeOut()));
    //    pTimer->start(100);

    //    _pReadThread->start();
    //    _pWriteThread->start();
    stylize();

}
//! [0]
bool Widget::event(QEvent *event)
{
    if (event->type() == QWinEvent::CompositionChange || event->type() == QWinEvent::ColorizationChange)
        stylize();
    return QWidget::event(event);
}
void Widget::initShow()
{
    taskbarButton->setOverlayIcon(style()->standardIcon(QStyle::SP_MediaPause));
    taskbarProgress->show();
    taskbarProgress->pause();
}
//! [4]
void Widget::createJumpList()
{
    QWinJumpList jumplist;
    jumplist.recent()->setVisible(true);
}
//! [4]

//! [5]
void Widget::createTaskbar()
{
    taskbarButton = new QWinTaskbarButton(this);
    taskbarButton->setWindow(this->windowHandle());
    taskbarProgress = taskbarButton->progress();
    taskbarProgress->show();
    connect(ui->horizontalSlider, &QAbstractSlider::valueChanged, taskbarProgress, &QWinTaskbarProgress::setValue);
    connect(ui->horizontalSlider, &QAbstractSlider::rangeChanged, taskbarProgress, &QWinTaskbarProgress::setRange);


    //    connect(&mediaPlayer, &QMediaPlayer::stateChanged, this, &MusicPlayer::updateTaskbar);
}
//! [1]
void Widget::stylize()
{
    if (QtWin::isCompositionEnabled()) {
        QtWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);
        setAttribute(Qt::WA_TranslucentBackground, true);
        setAttribute(Qt::WA_NoSystemBackground, false);
        setStyleSheet(QStringLiteral("MusicPlayer { background: transparent; }"));
    } else {
        QtWin::resetExtendedFrame(this);
        setAttribute(Qt::WA_TranslucentBackground, false);
        setStyleSheet(QStringLiteral("MusicPlayer { background: %1; }").arg(QtWin::realColorizationColor().name()));
    }
}
//! [5]

/** 有些资源的释放是要注意释放顺序的，
 *   这时候就需要用手动释放
 * @brief Widget::~Widget
 */
Widget::~Widget()
{
    if(_pReadThread)
        _pReadThread->deleteLater();
    if(_pWriteThread)
        _pWriteThread->deleteLater();
    delete ui;
}
void Widget::createThumbnailToolBar()
{
    thumbnailToolBar = new QWinThumbnailToolBar(this);
    thumbnailToolBar->setWindow(windowHandle());

    playToolButton = new QWinThumbnailToolButton(thumbnailToolBar);
    playToolButton->setEnabled(false);
    playToolButton->setToolTip(tr("Play"));
    playToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    //    connect(playToolButton, &QWinThumbnailToolButton::clicked, this, &MusicPlayer::togglePlayback);

    forwardToolButton = new QWinThumbnailToolButton(thumbnailToolBar);
    forwardToolButton->setEnabled(false);
    forwardToolButton->setToolTip(tr("Fast forward"));
    forwardToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    //    connect(forwardToolButton, &QWinThumbnailToolButton::clicked, this, &MusicPlayer::seekForward);

    backwardToolButton = new QWinThumbnailToolButton(thumbnailToolBar);
    backwardToolButton->setEnabled(false);
    backwardToolButton->setToolTip(tr("Rewind"));
    backwardToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    //    connect(backwardToolButton, &QWinThumbnailToolButton::clicked, this, &MusicPlayer::seekBackward);

    thumbnailToolBar->addButton(backwardToolButton);
    thumbnailToolBar->addButton(playToolButton);
    thumbnailToolBar->addButton(forwardToolButton);

    //    connect(&mediaPlayer, &QMediaPlayer::positionChanged, this, &MusicPlayer::updateThumbnailToolBar);
    //    connect(&mediaPlayer, &QMediaPlayer::durationChanged, this, &MusicPlayer::updateThumbnailToolBar);
    //    connect(&mediaPlayer, &QMediaPlayer::stateChanged, this, &MusicPlayer::updateThumbnailToolBar);
}
void Widget::slotTimeOut()
{
    if(ui->progressBar->value() <100){

        ui->progressBar->setValue(ui->progressBar->value()+1);
    }else{
        ui->progressBar->setValue(0);
    }
    //    taskbarProgress->setValue(ui->progressBar->value());
}
