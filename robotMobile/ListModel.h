#ifndef MUSICMODEL_H
#define MUSICMODEL_H

#include <QObject>
#include <QAbstractListModel>

class ListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ListModel(QObject *parent = 0);
    ~ListModel();

    enum RobotRole
    {
        RobotTITLE = Qt::UserRole + 1,
        RobotIp
    };
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    void resetModel();
signals:

public slots:

protected:
    QHash<int, QByteArray> roleNames() const;
    int m_RobotCount;
};

#endif // MUSICMODEL_H
