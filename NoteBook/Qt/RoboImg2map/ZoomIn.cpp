#include "ZoomIn.h"
#include <QPainter>
#include <QApplication>
#include <QScreen>
#include <qDebug>
#include <QMouseEvent>
#include <QAction>
#include <QDateTime>
#include <QColor>
#include <QRgb>
#include "UiClass.h"


ZoomIn::ZoomIn(QWidget *parent)
    : BaseWidget(parent),
      _shapeMode(0)
{
    m_pos = QPoint();
    this->resize(200,200);
    setWindowOpacity(1);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    //    setAttribute(Qt::WA_TranslucentBackground);
    QAction *act0,*act1, *act2, *act3,*act4,*act5,*act6,*act7;
    act0 = new QAction("100%", this);
    act1 = new QAction("150%", this);
    act2 = new QAction("200%", this);
    act3 = new QAction("400%", this);
    act4 = new QAction(tr("Area snapshot"), this);
    act5 = new QAction(tr("Desktop snapshot"), this);
    act6 = new QAction(tr("Rectangular"),this);
    act7 = new QAction(tr("Exit"), this);
    //多用group
    QActionGroup *group = new QActionGroup(this);
    connect(group,SIGNAL(triggered(QAction*)),
            this,SLOT(slotTriggered( QAction*)));
    group->addAction(act0);
    group->addAction(act1);
    group->addAction(act2);
    group->addAction(act3);
    group->addAction(act4);
    group->addAction(act5);
    group->addAction(act6);
    group->addAction(act7);
    //互斥
    group->setExclusive(true);
    for(int i=0;i<group->actions().size();++i)
    {
        group->actions().at(i)->setData(i);
        group->actions().at(i)->setCheckable(true);
    }

    this->addActions(group->actions());
    this->setContextMenuPolicy(Qt::ActionsContextMenu);
    //添加事件过滤
    this->installEventFilter(this);
    act0->trigger();
}

ZoomIn::~ZoomIn()
{
    qDebug()<<"~ZoomIn";
}

void ZoomIn::showEvent(QShowEvent *e)
{
    m_pix = QApplication::primaryScreen()->grabWindow(0);
    QWidget::showEvent(e);
}

bool ZoomIn::eventFilter(QObject *watched, QEvent *event)
{
    if( watched == this )
    {
        if(!m_isDis && QEvent::WindowDeactivate == event->type())
            m_isDis = true;
        else if(m_isDis && QEvent::WindowActivate == event->type())
        {
            this->setVisible(false);//隐藏下自己，防止下面截屏捕捉到
            m_pix = QApplication::primaryScreen()->grabWindow(0);
            this->setVisible(true);

            m_isDis = false;
            return true;
        }
    }
    return false;
}

void ZoomIn::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
        m_pos = e->pos();
    BaseWidget::mousePressEvent(e);
}

void ZoomIn::mouseMoveEvent(QMouseEvent *e)
{
    if (m_pos.isNull())
        return;
    //    if ((e->pos()-m_pos).manhattanLength() > 0) {
    //        QPoint p = pos()+e->pos()-m_pos;
    //        if (p.x() <= -(this->width()/2))
    //            p.setX(-(this->width()/2));
    //        if (p.x() > m_pix.width()-this->width()/2)
    //            p.setX(m_pix.width()-this->width()/2);
    //        if (p.y() <= -(this->height()/2))
    //            p.setY(-(this->height()/2));
    //        if (p.y() > m_pix.height()-this->height()/2)
    //            p.setY(m_pix.height()-this->height()/2);
    //        this->move(p);
    this->update();
    //    }
    BaseWidget::mouseMoveEvent(e);
}

void ZoomIn::mouseReleaseEvent(QMouseEvent *e)
{
    m_pos = QPoint();
    BaseWidget::mouseReleaseEvent(e);
}

void ZoomIn::paintEvent(QPaintEvent *)
{
    QPoint p = pos() + QPoint(m_O,m_O);
    QPainter paint(this);
    QPainterPath path;
    QRect r = this->rect();

    //去锯齿
    paint.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    //调整
    if(0==_shapeMode){
        r.adjust(1,1,-1,-1);
        //圆
        path.addEllipse(r);
        m_tempPix = m_pix.copy(QRect(p.x(), p.y(), m_width, m_height))
                .scaled(this->width(), this->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    }else if(1==_shapeMode){
        r.adjust(1,1,-1,-1);
        //矩形
        path.addRect(r);
        m_tempPix = m_pix.copy(QRect(p.x(), p.y(), m_width, m_height))
                .scaled(this->width(), this->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }


    paint.fillPath(path,
                   QBrush(m_tempPix));
    paint.setPen(QPen(QColor(45,130,255)));
    paint.drawPath(path);
    paint.setPen(Qt::blue);
    paint.drawLine(QPointF(0,this->rect().height()/2),
                   QPointF(this->rect().width(),this->rect().height()/2));
    paint.drawLine(QPointF(this->rect().width()/2,0),
                   QPointF(this->rect().width()/2,this->rect().height()));
    QPoint centerPoint = QPoint(this->rect().width()/2,this->rect().height()/2);

    if(!m_tempPix.isNull()){
        QColor color;
        QRgb rgb;
        QString rgbStr ;
# if(QT_VERSION >= QT_VERSION_CHECK(5,6,0))
        {
            color = m_tempPix.toImage().pixelColor(centerPoint);
            rgbStr  = QString("%1,%2,%3,%4")
                    .arg(color.red())
                    .arg(color.green())
                    .arg(color.blue())
                    .arg(color.alpha());
        }
#else
        {
            rgb = m_tempPix.toImage().pixel(centerPoint);
            rgbStr  = QString("%1,%2,%3,%4")
                    .arg(qRed(rgb))
                    .arg(qGreen(rgb))
                    .arg(qBlue(rgb))
                    .arg(qAlpha(rgb));
        }
#endif
        QFont f1("Helvetica [Cronyx]");
        QFontMetrics fm(f1);
        paint.setPen(Qt::red);
        paint.setFont(f1);
        int textWidthInPixels = fm.width(rgbStr);
        int textHeightInPixels = fm.height();
        QRectF textRect = QRectF(centerPoint.x(),centerPoint.y(),textWidthInPixels,textHeightInPixels);
        paint.drawText(textRect,rgbStr);
        emit sigZoomIn(0,rgbStr);
    }
}
void ZoomIn::resizeEvent(QResizeEvent *event)
{
    m_width = this->width();
    m_height = this->height();
    this->setTitleHeight(m_height);
    this->update();
}
void ZoomIn::slotTriggered(QAction*ac)
{
    switch (ac->data().toInt()) {
    case 0:
        m_O = 0;
        m_width = this->width();
        m_height = this->height();
        goto update;
        break;
    case 1:
        m_O = 25, m_width = this->width()*0.75; m_height = this->height()*0.75;
        goto update;
        break;
    case 2://自己计算比例
        m_O = 50, m_width = m_height = 100;
        goto update;
        break;
    case 3:
        m_O = 75, m_width = m_height = 50;
        goto update;
        break;
    case 4:
        m_tempPix.save(QString("%1_Area")
                       .arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz")));
        slotMessage(tr("Image saved"));
        goto update;
        break;
    case 5:
        m_pix.save(QString("%1_FullScreen")
                   .arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz")));
        slotMessage(tr("Image saved"));
        goto update;
        break;
    case 6:
        if(0==_shapeMode){
            ac->setText(tr("Circular"));
            _shapeMode = 1;
        }else{
            ac->setText(tr("Rectangular"));
            _shapeMode = 0;
        }
        goto update;
        break;
    case 7:
        sigZoomIn(1,"");
        break;
    default:
        break;
    }
    return;
update:
    {
        this->update();
    }
}
//type = 0 正常提示窗口 1警告窗口 2错误窗口
void ZoomIn::slotMessage(const QString&msg,int type)
{
    UiClass::init()->showToastr(msg,(SCToastr::SCToastrType)type);
}
