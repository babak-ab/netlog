#include "datamodel.h"

DataModel::DataModel(int columns, QObject* parent)
    : QAbstractTableModel(parent)
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

    if (Setting::instance()->inputType() == Setting::InputType_Hex) {
        m_data[index.column()][index.row()] = value.toString().toInt(&ok, 16);
        if (ok == false) {
            return false;
        }
    } else if (Setting::instance()->inputType() == Setting::InputType_Dec) {
        m_data[index.column()][index.row()] = value.toString().toInt(&ok, 10);
        if (ok == false) {
            return false;
        }
    } else {
        m_data[index.column()][index.row()] = (int)value.toString().toLatin1()[0];
    }

    emit dataChanged(index, index, { Qt::DisplayRole, Qt::EditRole });
    return true;
}

QVariant DataModel::data(const QModelIndex& index, int role) const
{
    if (index.isValid() && role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }

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
        if (Setting::instance()->inputType() == Setting::InputType_Hex) {
            int value = m_data.at(index.column()).at(index.row());
            return "0x" + QString::number(value, 16).toUpper();
        } else if (Setting::instance()->inputType() == Setting::InputType_Dec) {
            return m_data.at(index.column()).at(index.row());
        } else {
            int value = m_data.at(index.column()).at(index.row());
            return QString("/" + QString(QChar(value)));
        }
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

        //qDebug() << m_data[column].at(i) << QString(m_data[column].at(i)).toLatin1() << QString::number(m_data[column].at(i), 16);
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
    return QAbstractTableModel::headerData(section, orientation, role);
}
