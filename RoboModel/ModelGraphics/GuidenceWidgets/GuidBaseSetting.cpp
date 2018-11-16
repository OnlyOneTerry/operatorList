#include "GuidBaseSetting.h"

GuidBaseSetting::GuidBaseSetting(QWidget *parent) : QWidget(parent)
{

}

void GuidBaseSetting::setType(GuidType tp)
{
    m_type = tp;
}

GuidType GuidBaseSetting::getType()
{
    return m_type;
}

int GuidBaseSetting::getNUM()
{
    return num_;
}

void GuidBaseSetting::setConfigure(bool conf)
{
    isConfigure_ = conf;
}

bool GuidBaseSetting::getConfigure()
{
    return isConfigure_;
}

void GuidBaseSetting::enableNextStep()
{

}

void GuidBaseSetting::showNumWid()
{

}

void GuidBaseSetting::setNextPreBtnDisplay(bool flag)
{

}

void GuidBaseSetting::slot_enableNextStep()
{

}
