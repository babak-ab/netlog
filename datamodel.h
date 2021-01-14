#ifndef DATAMODEL_H
#define DATAMODEL_H

#include "lineeditdelegate.h"

#include <QAbstractItemModel>
#include <QDebug>

class DataModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit DataModel(LineEditDelegate* lineEdit, int columns = 6, QObject* parent = Q_NULLPTR);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role);
    QVariant data(const QModelIndex& index, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;

private:
    int m_columns;
    int m_rows;
    QVector<QVector<int>> m_data;
    LineEditDelegate* m_lineEditDelegate;
};

#endif // DATAMODEL_H
