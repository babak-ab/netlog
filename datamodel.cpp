#include "datamodel.h"

DataModel::DataModel(LineEditDelegate* lineEdit, int columns, QObject* parent)
    : QAbstractTableModel(parent)
    , m_lineEditDelegate(lineEdit)
    , m_columns(columns)
    , m_rows(255)

{
    for (int j = 0; j < m_columns; j++) {
        QVector<int> temp(m_rows);
        temp.fill(-1);
        m_data.append(temp);
    }
}

int DataModel::rowCount(const QModelIndex& parent) const
{
    return m_rows;
}

int DataModel::columnCount(const QModelIndex& parent) const
{
    return m_columns;
}

bool DataModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (role != Qt::EditRole)
        return false;

    bool ok = false;
    m_data[index.column()][index.row()] = value.toString().toInt(&ok, 16);
    if (ok == false) {
        return false;
    }
    emit dataChanged(index, index, { Qt::DisplayRole, Qt::EditRole });
    return true;
}

QVariant DataModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }
    if (m_data.at(index.column()).at(index.row()) == -1)
        return QVariant();
    else {
        if (m_lineEditDelegate == Q_NULLPTR) {
            return m_data.at(index.column()).at(index.row());
        }
        if (m_lineEditDelegate->hexEnabled()) {
            int value = m_data.at(index.column()).at(index.row());
            if (m_lineEditDelegate->addHexPrefix()) {
                return "0x" + QString::number(value, 16).toUpper();
            } else {
                return QString::number(value, 16).toUpper();
            }
        }
        return m_data.at(index.column()).at(index.row());
    }
}

Qt::ItemFlags DataModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}
