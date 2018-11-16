#ifndef PROPERTYBASICWIDGET_H
#define PROPERTYBASICWIDGET_H

#include <QWidget>
#include"ModelCommon.h"
class PropertyBasicWidget : public QWidget
{
    Q_OBJECT
public:

public:
    explicit PropertyBasicWidget(QWidget *parent = nullptr);
public:
    void setType(widType type);
    widType getType();
private:

    widType m_type;
};

#endif // PROPERTYBASICWIDGET_H
