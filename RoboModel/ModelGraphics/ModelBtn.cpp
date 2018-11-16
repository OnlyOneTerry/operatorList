#include "ModelBtn.h"
#include "QDebug"

#include <QMouseEvent>
ModelBtn::ModelBtn(int type,QWidget *widget):
    widget_(widget),
    type_(type)
{
    setIconSize (QSize(30,30));
}

ModelBtn::~ModelBtn()
{

}

void ModelBtn::mousePressEvent (QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {

    }
}

void ModelBtn::mouseMoveEvent (QMouseEvent *event)
{
    if(event->buttons ()&Qt::LeftButton)
    {
        if(rect ().contains (event->pos ()))
        {
            widget_->setCursor (Qt::ArrowCursor);
        }
        else
        {
            widget_->setCursor (QCursor( icon().pixmap (QSize(80,80))));
        }
    }
}

void ModelBtn::mouseReleaseEvent (QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        widget_->setCursor (Qt::ArrowCursor);
        if(rect ().contains (event->pos ()))
        {
        }
        else
        {
            emit sig_add_polygon (mapToParent (event->pos ()),type_);
        }
    }
}
