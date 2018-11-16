#include "PropertyBasicWidget.h"

PropertyBasicWidget::PropertyBasicWidget(QWidget *parent) : QWidget(parent)
{
  m_type = WIDGET_INVALID;
}

void PropertyBasicWidget::setType(widType type)
{
    m_type = type;
}

widType PropertyBasicWidget::getType()
{
   return m_type;
}
