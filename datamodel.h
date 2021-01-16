#ifndef DATAMODEL_H
#define DATAMODEL_H

#include "setting.h"
#include <QAbstractItemModel>
#include <QDebug>

class DataModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit DataModel(int columns = 6, QObject* parent = Q_NULLPTR);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role);
    QVariant data(const QModelIndex& index, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;

    QByteArray getData(int column);
private Q_SLOTS:
    void sltDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles = QVector<int>());

private:
    int m_columns;
    QVector<QVector<int>> m_data;

    // QAbstractItemModel interface
public:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // DATAMODEL_H
