#include "WidgetSetting.h"
#include"UndoStack.h"

WidgetSetting::WidgetSetting(QWidget *parent) :
    PropertyBasicWidget(parent)
{
    //setAttribute (Qt::WA_DeleteOnClose);

    m_unStack = UndoStack::InitStack();
}

WidgetSetting::~WidgetSetting()
{
    emit sig_deleted (type());
}

void WidgetSetting::set_r(const qreal &angel)
{

}

ItemBase *WidgetSetting::getRelateItem()
{
   return NULL;
}
