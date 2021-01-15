#include "datamodel.h"

DataModel::DataModel(LineEditDelegate* lineEdit, int columns, QObject* parent)
    : QAbstractTableModel(parent)
    , m_lineEditDelegate(lineEdit)
    , m_columns(columns)

{
    for (int j = 0; j < m_columns; j++) {
        QVector<int> temp(1);
        temp.fill(-1);
        m_data.append(temp);
    }

    connect(this, &QAbstractItemModel::dataChanged, this, &DataModel::sltDataChanged);
}

int DataModel::rowCount(const QModelIndex& parent) const
{
    return m_data[0].count();
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

    if (m_data.count() <= index.column())
        return QVariant();

    if (m_data[index.column()].count() <= index.row())
        return QVariant();

    if (m_data.at(index.column()).at(index.row()) == -1) {
        return QVariant();
    } else {
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

    if (index.row() >= 1 && m_data[index.column()][index.row() - 1] == -1)
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

QByteArray DataModel::getData(int column)
{
    QByteArray buffer;
    for (int i = 0; i < m_data[column].size(); ++i) {
        if (m_data[column].at(i) == -1)
            break;
        buffer.append(m_data[column].at(i));
    }
    return buffer;
}

void DataModel::sltDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
    if (topLeft.row() + 1 == m_data[0].count()) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        for (int j = 0; j < m_columns; j++) {
            QVector<int> temp(1);
            temp.fill(-1);
            m_data[j].append(temp);
        }
        endInsertRows();
    }
}

QVariant DataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    //    if (Qt::Horizontal != orientation)
    //        return QAbstractTableModel::headerData(section, orientation, role);

    //    switch (role) {
    //    case Qt::DecorationRole: {
    //        QPixmap p { 12, 12 };
    //        p.fill(Qt::CheckState(headerData(section, orientation, Qt::CheckStateRole).toUInt()) ? Qt::green : Qt::red);
    //        return p;
    //    }
    //    }

    return QAbstractTableModel::headerData(section, orientation, role);
}
