#ifndef MUSICMODEL_H
#define MUSICMODEL_H

#include <QObject>
#include <QAbstractListModel>

class Data
{
public:
    Data(const QString &title, const QString &ip,const QString &src);
    QString title()const;
    QString IP() const;
    QString source()const;
private:
    QString title_;
    QString ip_;
    QString source_;
};



class ListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ListModel(QObject *parent = 0);
    ~ListModel();

    enum RobotRole
    {
        RobotTITLE = Qt::UserRole + 1,
        RobotIp,
        RobotContent
    };
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    void resetModel();

    //Add data;
    Q_INVOKABLE void insert(int index,const Data &data);
    //Remove data;
    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE void append(const QString& title, const QString &ip, const QString src);
signals:
    void dataChanged();
public slots:

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    int m_RobotCount;
    QList<Data> m_data;//用QList<Data>做数据源
};

#endif // MUSICMODEL_H
