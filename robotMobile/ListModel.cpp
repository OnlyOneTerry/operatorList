#include "ListModel.h"
#include <QDebug>

ListModel::ListModel(QObject *parent)
    : QAbstractListModel(parent), m_RobotCount(0)
{

}

ListModel::~ListModel()
{

}


QVariant ListModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row <0 || row >= m_data.count()){
        return QVariant();
    }

    const Data &data = m_data[row];
    switch (role) {
    case  RobotTITLE:
        return data.title();
        break;
    case RobotIp:
        return data.IP();
    case RobotContent:
        return data.source();
    default:
        break;
    }

    return QVariant();
}

int ListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.count();
}

QHash<int, QByteArray> ListModel::roleNames() const
{
    QHash<int, QByteArray> roles; //设置列表的角色，即所有属性字段
    roles[RobotTITLE] = "robotTitle";
    roles[RobotIp] = "robotIp";
    roles[RobotContent]="robotContent";

    return roles;
}


void ListModel::resetModel()
{
    beginResetModel();

    endResetModel();
}

void ListModel::insert(int index, const Data &data)
{
    if(index < 0 || index > m_data.count()){
        return;
    }
    emit  beginInsertRows(QModelIndex(),index,index);
    m_data.insert(index,data);
    emit endInsertRows();
}

void ListModel::remove(int index)
{
    if(index < 0 || index >= m_data.count()){
        return;
    }

    beginRemoveRows(QModelIndex(),index,index);
    m_data.removeAt(index);
    endRemoveRows();
}

void ListModel::append(const QString &title, const QString &ip,const QString src)
{
    qDebug()<<"src is -----------"<<src;
    insert(m_data.count(),Data(title,ip,src));
}
//机器人相关属性
Data::Data(const QString &title, const QString &ip, const QString &src):
    title_(title),
    ip_(ip),
    source_(src)
{

}

//int showObject2GetMoney(gender* object = NULL,
//                        gender* family = NULL,
//                        gender* anybody = NULL )
//{
//   bool invalid = false;

//   if(object){
//     invalid = (this&object).show();
//   }
//   else{
//     invalid = (this&family).show();
//   }

//   if(invalid){
//       return  Money();
//     }
//     else{
//       if(anybody==NULL) return 0 ;

//       (this&anybody).show();

//       return 100;
//     }
//}

QString Data::title() const
{
    return title_;
}

QString Data::IP() const
{
    return ip_;
}

QString Data::source() const
{
    return source_;
}
