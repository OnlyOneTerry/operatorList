#include "SCBaseGraphicsView.h"
#include <QScrollBar>
#include <QWheelEvent>
#include <QApplication>
#include <QToolTip>
#include<QTimer>
/*******************************************************
 * Description:｛基类｝ 视图
 *
 * @author
 * @date
 *******************************************************/

SCBaseGraphicsView::SCBaseGraphicsView(QWidget *parent):
    QGraphicsView(parent),
    is_operate_refer_line_(false),
    cursorType_(NONE)
{
    this->setObjectName("SCBaseGraphicsView");
    setMouseTracking(true);

    rule_font_.setPixelSize(8);

    setToolTipDuration(0);

}


/*******************************************************
 * Description: [event] 滚轮缩放视角
 *
 * @para event
 * @author
 * @date
 *******************************************************/
void SCBaseGraphicsView::wheelEvent(QWheelEvent *event)
{
    QGraphicsView::wheelEvent(event);

    if(QApplication::keyboardModifiers() == Qt::ControlModifier)
    {
        qreal scale_size=event->delta()>0?1.2:0.8;
        set_scale_size(scale_size);
    }
    else
    {
        if(event->orientation()==Qt::Vertical)
        {
            int vsvalue=verticalScrollBar()->value();
            verticalScrollBar()->setValue(vsvalue-event->delta());
        }
        else if(event->orientation()==Qt::Horizontal)
        {
            int vsvalue=horizontalScrollBar()->value();
            horizontalScrollBar()->setValue(vsvalue-event->delta());
        }
    }
}


/*******************************************************
 * Description: [event] 绘制屏幕尺
 *
 * @para event
 * @author
 * @date
 *******************************************************/
void SCBaseGraphicsView::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);//view接收,先绘制场景

    int x_max=this->mapToScene(0,k_rule_width_).x();
    int y_min=this->mapToScene(this->viewport()->width(),0).y();
    int y_max=this->mapToScene(k_rule_width_,0).y();
    int x_min=this->mapToScene(0,this->viewport()->height()).x();

    QPainter painter(this->viewport());
    //  if(is_rule_visible_)
    {

        painter.setPen(Qt::NoPen);
        //        painter.setBrush(QColor(70,70,70,200));
        painter.setBrush(backgroundColor);
        painter.drawRect(0,0,this->viewport()->width(),k_rule_width_);
        painter.drawRect(0,k_rule_width_,k_rule_width_,this->viewport()->height());
        painter.setBrush(Qt::NoBrush);

        painter.setPen(QPen(textColor,1));

        int scales=(x_max-x_min)/100;
        scales==0?scales=1:NULL;
        QFont font;
        font.setPixelSize(10);
        painter.setFont(font);
        painter.drawText(QRectF(0,0,k_rule_width_,15)," cm");
        painter.setFont(rule_font_);

        for(int i=y_min;i<y_max;++i)
        {
            QPoint px=mapFromScene(0,i);
            if(0==i%scales)
            {
                if(0==i%(10*scales))
                {
                    painter.drawLine(px.x(),k_rule_width_,px.x(),9);
                    QString ruletext=QString::number(i);
                    painter.drawText(QPoint(px.x()-ruletext.size()*2.2,8),ruletext);
                }
                else if(0==i%(5*scales))
                {
                    painter.drawLine(px.x(),k_rule_width_,px.x(),12);
                }
                else
                {
                    painter.drawLine(px.x(),k_rule_width_,px.x(),15);
                }
            }
        }
        for(int j=x_min;j<x_max;++j)
        {
            QPoint py=mapFromScene(j,0);
            if(0==j%scales)
            {
                if(0==j%(10*scales))
                {
                    painter.drawLine(k_rule_width_,py.y(),9,py.y());

                    QString ruletext=QString::number(j);
                    QTransform transform;
                    transform.rotate(-90);
                    painter.setTransform(transform);
                    painter.drawText(QPoint(-py.y()-ruletext.size()*2.2,8),ruletext);
                    transform.rotate(90);
                    painter.setTransform(transform);
                }
                else if(0==j%(5*scales))
                {
                    painter.drawLine(k_rule_width_,py.y(),12,py.y());
                }
                else
                {
                    painter.drawLine(k_rule_width_,py.y(),15,py.y());
                }
            }
        }
        QPoint mouse_pos=current_mouse_screen_pos_;

        painter.setPen(QPen(QColor(132,223,223),1));
        painter.drawLine(k_rule_width_,mouse_pos.y(),0,mouse_pos.y());
        painter.drawLine(mouse_pos.x(),k_rule_width_,mouse_pos.x(),0);

        foreach (QPointF hpos, refer_hline_list_) {
            painter.drawLine(k_rule_width_,mapFromScene(hpos).y(),viewport()->width(),mapFromScene(hpos).y());
        }
        foreach (QPointF vpos, refer_vline_list_) {
            painter.drawLine(mapFromScene(vpos).x(),k_rule_width_,mapFromScene(vpos).x(),viewport()->height());
        }
        if(referLine==Hline )
        {
            painter.drawLine(k_rule_width_,mouse_pos.y(),viewport()->width(),mouse_pos.y());
        }
        else if(referLine==Vline )
        {
            painter.drawLine(mouse_pos.x(),k_rule_width_,mouse_pos.x(),viewport()->height());
        }

        if(cursorType_ == CrossCursor && mouseIsPressed_ )
        {
            painter.drawLine(k_rule_width_,mouse_pos.y(),viewport()->width(),mouse_pos.y());
            painter.drawLine(mouse_pos.x(),k_rule_width_,mouse_pos.x(),viewport()->height());
        }

    }

    //尺子
    if(rule_pos_list_.size())
    {
        painter.setPen(Qt::darkGreen);
        painter.drawLine(mapFromScene(rule_pos_list_.first()),mapFromScene(rule_pos_list_.last()));
    }

    //    QBrush brush(Qt::yellow);
    //    painter.setBrush(brush);
    //    painter.drawRect(0,0,k_rule_width_,k_rule_width_);

}

/*******************************************************
 * Description: [event] 鼠标单击
 *
 * @para event
 * @author
 * @date
 *******************************************************/
void SCBaseGraphicsView::mousePressEvent(QMouseEvent *event)
{

    //屏幕尺
    if(QRect(k_rule_width_,0,this->viewport()->width(),k_rule_width_).contains(event->pos())||QRect(0,k_rule_width_,k_rule_width_,this->viewport()->height()).contains(event->pos()))
    {
        is_operate_refer_line_=true;
        isNeedAddLine = true;
    }
    else
    {
        is_operate_refer_line_ = false;
        if(is_operate_refer_line_&&referLine==NO)
        {

        }
        else
        {
            QGraphicsView::mousePressEvent(event);
        }
    }

    if(viewport()->cursor().shape()==Qt::SplitVCursor)
    {
        refer_hline_list_.removeOne(pressed_refer_pos_);
        referLine=Hline;
        is_operate_refer_line_=true;
    }
    else  if(viewport()->cursor().shape()==Qt::SplitHCursor)
    {
        refer_vline_list_.removeOne(pressed_refer_pos_);
        referLine=Vline;
        is_operate_refer_line_=true;
    }


    if(cursorType_==CrossCursor)
    {
        is_operate_refer_line_ = true;
        referLine = V_H_line;
        foreach (QPointF pos, refer_vline_list_)
        {
            if(mapFromScene(pos).x()==crossPos_.x())
            {
                refer_vline_list_.removeOne(pos);
            }
        }
        foreach (QPointF pos, refer_hline_list_)
        {

            if(mapFromScene(pos).y()==crossPos_.y())
            {
                refer_hline_list_.removeOne(pos);
            }
        }
    }


    if (QApplication::keyboardModifiers() == Qt::AltModifier)
    {
        {//尺子
            rule_pos_list_.append(mapToScene(event->pos()));
            rule_pos_list_.append(mapToScene(event->pos()));
        }
    }

    mouseIsPressed_ = true;
}

/*******************************************************
 * Description: [event] 鼠标移动
 *
 * @para event
 * @author
 * @date
 *******************************************************/
void SCBaseGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    current_mouse_screen_pos_=event->pos();

    QGraphicsView::mouseMoveEvent(event);

    QCursor current_cursor=viewport()->cursor();
    //屏幕尺
    {
        viewport()->update(QRect(0,0,k_rule_width_,viewport()->height()));
        viewport()->update (QRect(0,0,viewport()->width (),k_rule_width_));

        bool is_cursor_need_change=false;

        //用于规避当同时拖拽纵横两条参考线时过另外一条线时，保持光标样式
        if(event->buttons()&Qt::LeftButton)
        {
            if(cursorType_ == CrossCursor)
            {
                crossFlag = true;
            }
            else
            {
                crossFlag = false;
            }

            //如果鼠标在交叉点附近但是光标不是十字型 直接返回
            foreach (QPointF crossPos, crossPoint_list_)
            {
                if(QRect(crossPos.x()-2,crossPos.y()-2,4,4).contains(event->pos()))
                {
                    if(cursorType_ != CrossCursor) return;
                }
            }

        }
        else
        {
            crossFlag = false;
            //如果是十字型单是不在响应区域
            foreach (QPointF crossPos, crossPoint_list_)
            {
                if(!QRect(crossPos.x()-2,crossPos.y()-2,4,4).contains(event->pos()))
                {
                    if(cursorType_ ==  CrossCursor)
                    {
                        viewport()->setCursor(Qt::ArrowCursor);
                    }
                }
            }

        }

        if(crossPoint_list_.size()>0)
        {
            foreach (QPointF crossPos, crossPoint_list_)
            {
                if(QRect(crossPos.x()-1,crossPos.y()-1,2,2).contains(event->pos()))
                {
                    qDebug()<<"--------------cursor is -----crossCursor----";
                    viewport()->setCursor(Qt::CrossCursor);
                    is_cursor_need_change=true;
                    cursorType_ = CrossCursor;
                    crossPos_ = crossPos;//获取当前交点
                    viewport()->update();
                    break;
                }
                else
                {
                    //*****
                    if(!mouseIsPressed_&& cursorType_ ==  CrossCursor)
                    {
                        //                         cursorType_ = NONE;
                        viewport()->setCursor(Qt::ArrowCursor);
                    }
                    foreach (QPointF hpos, refer_hline_list_)
                    {
                        if(QRect(k_rule_width_,mapFromScene(hpos).y()-1,viewport()->width()-k_rule_width_,2).contains(event->pos()))
                        {
                            if(crossFlag) return;
                            viewport()->setCursor(Qt::SplitVCursor);
                            is_cursor_need_change=true;
                            pressed_refer_pos_=hpos;
                            break;
                        }
                    }
                    foreach (QPointF Vpos, refer_vline_list_)
                    {
                        if(QRect(mapFromScene(Vpos).x()-1,k_rule_width_,2,viewport()->height()-k_rule_width_).contains(event->pos()))
                        {
                            if(crossFlag) return;
                            viewport()->setCursor(Qt::SplitHCursor);
                            is_cursor_need_change=true;
                            pressed_refer_pos_=Vpos;
                            break;
                        }
                    }

                }
            }
        }
        else
        {
            foreach (QPointF hpos, refer_hline_list_)
            {
                if(QRect(k_rule_width_,mapFromScene(hpos).y()-1,viewport()->width()-k_rule_width_,2).contains(event->pos()))
                {
                    if(crossFlag) return;
                    viewport()->setCursor(Qt::SplitVCursor);
                    is_cursor_need_change=true;
                    pressed_refer_pos_=hpos;
                    break;
                }
            }
            foreach (QPointF Vpos, refer_vline_list_)
            {
                if(QRect(mapFromScene(Vpos).x()-1,k_rule_width_,2,viewport()->height()-k_rule_width_).contains(event->pos()))
                {
                    if(crossFlag) return;
                    viewport()->setCursor(Qt::SplitHCursor);
                    is_cursor_need_change=true;
                    pressed_refer_pos_=Vpos;
                    break;
                }
            }
        }
        //------------------------
        if(event->buttons()&Qt::LeftButton)
        {

        }
        else
        {
            if(!is_cursor_need_change)
            {
                if(current_cursor.shape()==Qt::SplitHCursor||current_cursor.shape()==Qt::SplitVCursor||cursorType_ == CrossCursor)
                {
                    viewport()->setCursor(Qt::ArrowCursor);
                    cursorType_ = NONE;
                }
                else
                {
                    viewport()->setCursor(current_cursor);
                }
            }

        }
    }

    if (event->buttons()&Qt::LeftButton)
    {
        if(QRect(k_rule_width_,0,this->viewport()->width(),k_rule_width_).contains(event->pos()))
        {
            if(is_operate_refer_line_)
                referLine=Hline;

        }
        else if(QRect(0,k_rule_width_,k_rule_width_,this->viewport()->height()).contains(event->pos()))
        {
            if(is_operate_refer_line_)
                referLine=Vline;
        }
        else
        {
            foreach (QPointF crossPos, crossPoint_list_)
            {
                if(QRect(crossPos.x()-0.5,crossPos.y()-0.5,1,1).contains(event->pos()))
                {
                    cursorType_  = CrossCursor;
                }
            }

            if(QRect(0,0,k_rule_width_,k_rule_width_).contains(event->pos()))
            {
                if(is_operate_refer_line_)
                    referLine = V_H_line;
            }
        }
        update();
    }

    if (QApplication::keyboardModifiers() == Qt::AltModifier)
    {
        {//尺子
            if(rule_pos_list_.size()==2)
            {
                rule_pos_list_.last()=mapToScene(event->pos());

                QPointF sub_pox=rule_pos_list_.last()-rule_pos_list_.first();
                qreal sub_x =sub_pox.x();
                qreal sub_y =sub_pox.y();
                qreal angle = atan(sub_y / sub_x) * 180 / 3.141592654;
                sub_x < 0 ? (sub_y>0 ? angle += 180 : angle -= 180)   /*(fabs(sub_y-0)<0.0000001?angle=180:angle-=180))  */ : angle;

                QString str_infor=QString(tr("Length: %1m  Angle: %2").arg(qRound(sqrt(sub_x*sub_x + sub_y*sub_y)*10) / 1000.0).arg(qRound(1000*angle)/1000.0)+QStringLiteral("°"));

                if(event->buttons()&Qt::LeftButton)
                {
                    QToolTip::showText(event->globalPos(),str_infor);
                }
            }
        }
    }

    if(event->buttons()==Qt::MidButton)
    {
        viewport()->setCursor(Qt::ClosedHandCursor);
    }
}

/*******************************************************
 * Description: [event] 鼠标释放
 *
 * @para event
 * @author
 * @date
 *******************************************************/
void SCBaseGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    isNeedAddLine = false;
    crossFlag = false;
    mouseIsPressed_  = false;
    QGraphicsView::mouseReleaseEvent(event);
    viewport()->setCursor(Qt::ArrowCursor);
    //屏幕尺
    if(referLine==Hline)
    {
        if(!QRect(k_rule_width_,0,this->viewport()->width(),k_rule_width_).contains(event->pos()))
            refer_hline_list_.append(mapToScene(event->pos()));
    }
    else if(referLine==Vline)
    {
        if(!QRect(0,k_rule_width_,k_rule_width_,this->viewport()->height()).contains(event->pos()))
            refer_vline_list_.append(mapToScene(event->pos()));
    }

    if(cursorType_ == CrossCursor )
    {
        if(QRect(0,0,k_rule_width_,k_rule_width_).contains(event->pos()))
        {
        }
        else
        {
            if(!QRect(k_rule_width_,0,this->viewport()->width(),k_rule_width_).contains(event->pos()))
                refer_hline_list_.append(mapToScene(event->pos()));
            if(!QRect(0,k_rule_width_,k_rule_width_,this->viewport()->height()).contains(event->pos()))
                refer_vline_list_.append(mapToScene(event->pos()));
        }
    }
    //添加所有交点
    crossPoint_list_.clear();
    if(refer_hline_list_.size()>0&&refer_vline_list_.size()>0)
    {
        for(int i = 0; i<refer_hline_list_.size();i++)
        {
            for(int j = 0; j<refer_vline_list_.size();j++)
            {
                QPointF tempPos(refer_hline_list_.at(i).x(),refer_vline_list_.at(j).y());
                crossPoint_list_.append(mapFromScene(tempPos));
            }
        }
    }

    referLine=NO;
    cursorType_ = NONE;
    is_operate_refer_line_=false;
    update();
    rule_pos_list_.clear();
}

/*******************************************************
 * Description: [event] 鼠标双击
 *
 * @para event
 * @author
 * @date
 *******************************************************/
void SCBaseGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QGraphicsView::mouseDoubleClickEvent(event);
}

/*******************************************************
 * Description: 设置缩放尺寸
 *
 * @para scale_size-缩放量
 * @author
 * @date
 *******************************************************/
void SCBaseGraphicsView::set_scale_size(qreal scale_size)
{
    //  setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    scale_history_*=scale_size;
    static qreal temp = 1.0;
    if(scale_history_>=1)
    {
        temp = scale_history_;//记录有效缩放倍率
        scale(scale_size,scale_size);
    }
    scale_history_ = temp;//将有效倍率赋值给scale_history_ 使下次从有效值开始进行缩放操作
    centerOn(QPointF(0,0));
    // setTransformationAnchor(NoAnchor);
}

QCursor SCBaseGraphicsView::get_viewport_cursor() const
{
    return viewport()->cursor();
}
QColor SCBaseGraphicsView::getBackgroundColor() const
{
    return backgroundColor;
}

QColor SCBaseGraphicsView::getLineBorderColor() const
{
    return lineBorderColor;
}

QColor SCBaseGraphicsView::getTextColor() const
{
    return textColor;
}

void SCBaseGraphicsView::setTextColor(const QColor &value)
{
    textColor = value;
}

void SCBaseGraphicsView::setLineBorderColor(const QColor &value)
{
    lineBorderColor = value;
}

void SCBaseGraphicsView::setBackgroundColor(const QColor &value)
{
    backgroundColor = value;
}
