#include "ListModel.h"

ListModel::ListModel(QObject *parent)
    : QAbstractListModel(parent), m_RobotCount(0)
{

}

ListModel::~ListModel()
{

}


QVariant ListModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= m_RobotCount)
    {
        return QVariant();
    }

//    QDBusReply<QVariantMap> reply = Services::musicService()->call("getMusicData",index.row());

    QVariant retData;
//    switch(role)
//    {
//    case MUSICTITLE:
//        retData = (reply.value())["musicTitle"];
//        break;
//    case MUSICARTIST:
//        retData = (reply.value())["musicArtist"];
//        break;
//    case MUSICALBUM:
//        retData = (reply.value())["musicAlbum"];
//        break;
//    case MUSICPATH:
//        retData = (reply.value())["musicPath"];
//        break;
//    case MEDIATYPE:
//        retData = (reply.value())["mediaType"];
//        break;
//    default:
//        break;
//    }

    return retData;
}

int ListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_RobotCount;
}

QHash<int, QByteArray> ListModel::roleNames() const
{
    QHash<int, QByteArray> role;
//    role[MUSICTITLE]    = "MusicTitle";
//    role[MUSICARTIST]   = "MusicArtist";
//    role[MUSICALBUM]    = "MusicAlbum";
//    role[MUSICPATH]     = "MusicPath";
//    role[MEDIATYPE]     = "MediaType";

    return role;
}


void ListModel::resetModel()
{
    beginResetModel();
//    QDBusReply<int> reply = Services::musicService()->call("getMusicCount");
//    m_RobotCount = reply.value();
    endResetModel();
}
