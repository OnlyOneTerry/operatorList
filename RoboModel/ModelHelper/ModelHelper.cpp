#include "ModelHelper.h"
#include "QJson/serializer.h"
#include"GlobalVariant.h"
/*******************************************************
 * Description: json格式化处理
 *
 * variant_map
 *******************************************************/
QByteArray optimizationVariantMap(const QVariant &var)
{
    QByteArray temp_byte_array;
    QJson::Serializer serializer;
    serializer.setDoublePrecision(5);
    serializer.setIndentMode (QJson::IndentFull);
    bool is_sucess = false;
    temp_byte_array = serializer.serialize(var,&is_sucess);

    if(is_sucess)
    {
        return temp_byte_array;
    }

    return "";
}

bool is0(qreal value)
{
    if(qFabs(value)>0.0001)
        return false;
    return true;
}


void  adjust_combox_items(bool zero_start, quint16 index, customComboBox *cb)
{
    quint16 cb_size=cb->count();
    quint16 t_index=zero_start?index+1:index;
    if(t_index>cb_size)
    {
        for(int ti=cb_size+1;ti<=t_index;ti++)
        {
            cb->blockSignals(true);
            cb->addItem(QString::number(zero_start?ti-1:ti));
            cb->blockSignals(false);
        }
    }
    cb->blockSignals(true);
    cb->setCurrentIndex(zero_start?index:index-1);
    cb->setOri_index(cb->currentIndex());
    cb->blockSignals(false);

}

void adjust_combox_items(quint16 idx, customComboBox *cb)//idx 保存文件中读取到index值
{
    int curIndex = cb->findData(idx);
    cb->blockSignals(true);
    cb->setCurrentIndex(curIndex);
    cb->setOri_index(cb->currentIndex());
    cb->blockSignals(false);
}

void adjust_combox_items(QString str, customComboBox *cb)
{
    cb->blockSignals(true);
    cb->setCurrentText(str);
    cb->setOri_index(cb->currentIndex());
    cb->blockSignals(false);
}
