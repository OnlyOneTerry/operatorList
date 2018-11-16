#ifndef MODELHELPER_H
#define MODELHELPER_H

#include <QByteArray>
#include <QVariantMap>
#include <QComboBox>
#include"ModelCustomCommand.h"

QByteArray optimizationVariantMap(const QVariant &var);

bool is0(qreal value);

void adjust_combox_items(bool zero_start,quint16 index,customComboBox *cb);
void adjust_combox_items(quint16 defaultIndex,customComboBox *cb);
void adjust_combox_items(QString str,customComboBox *cb);//设置字符串类型


#endif // MODELHELPER_H
