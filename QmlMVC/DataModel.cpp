#include "DataModel.h"

DataModel::DataModel(QObject *parent)
{

}

DataModel::~DataModel()
{

}

QVariant DataModel::data(const QModelIndex &index, int role) const
{

    int row = index.row();
    if(row <0 || row >=m_data.count()){
        return QVariant();
    }

    const Data &data = m_data[row];
    switch (role) {
    case  TitleRole:
        return data.title();
        break;
    case ColorRole:
        return data.color();
    default:
        break;
    }

    return QVariant();
}

int DataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.count();
}

void DataModel::insert(int index, const Data &data)
{
    if(index < 0 || index > m_data.count()){
        return;
    }
    emit  beginInsertRows(QModelIndex(),index,index);
    m_data.insert(index,data);
    emit endInsertRows();
}

void DataModel::remove(int index)
{
    if(index < 0 || index >= m_data.count()){
        return;
    }

    beginRemoveRows(QModelIndex(),index,index);
    m_data.removeAt(index);
    endRemoveRows();
}

void DataModel::append(const QString &title, const QString &color)
{
    insert(m_data.count(),Data(title,color));
}

QHash<int, QByteArray> DataModel::roleNames() const
{
    QHash<int,QByteArray> roles;
    roles[TitleRole] = "dmTitle";
    roles[ColorRole] = "dmColor";
    return roles;
}

///////////////
Data::Data(const QString &titile, const QString &color)
    :title_(titile)
    ,color_(color)
{

}

QString Data::title() const
{
    return title_;
}

QString Data::color() const
{
    return color_;
}
