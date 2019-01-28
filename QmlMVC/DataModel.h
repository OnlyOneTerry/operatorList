#ifndef DATAMODEL_H
#define DATAMODEL_H
#include<QObject>
#include<QAbstractListModel>
//table 表

class Data
{
public:
    Data(const QString &titile,const QString &color);
    QString title()const;
    QString color() const;
private:
    QString title_;
    QString color_;
};

class DataModel:public QAbstractListModel
{
    Q_OBJECT
    enum DataRoles{
        TitleRole = Qt::UserRole + 1,
        ColorRole
    };
public:
    DataModel(QObject* parent = 0);
    ~DataModel();
    //继承基类的函数
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    //Add data;
    Q_INVOKABLE void insert(int index,const Data &data);
    //Remove data;
    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE void append(const QString& title,const QString &color);
    QHash<int,QByteArray> roleNames()const;//重新实现roleNames
signals:
    void dataChanged();
private:
    QList<Data> m_data;//用QList<QVariantList>做数据源
};

#endif // DATAMODEL_H
