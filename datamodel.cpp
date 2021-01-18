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

    if (!index.isValid()) {
        return false;
    }

    if (value == -1 && (index.row() + 1) < rowCount()) {
        if (m_data[index.column()][index.row() + 1] != -1) {
            return false;
        }
    }

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
            return "0x" + QString::number(value, 16).rightJustified(2, '0').toUpper();
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

    if (index.row() < m_data[index.column()].count()) {
        if (index.row() >= 1 && m_data[index.column()][index.row() - 1] == -1)
            return Qt::NoItemFlags;
    }

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

void DataModel::insertData(int column, int row, QByteArray ba)
{

    bool flag = false;
    if (rowCount() < row + ba.count() - 1) {
        flag = true;
        Q_EMIT beginInsertRows(QModelIndex(), row, row + ba.count() - 1);
        for (int j = 0; j < ba.count(); j++) {
            for (int j = 0; j < m_columns; j++) {
                QVector<int> temp(1);
                temp.fill(-1);
                m_data[j].append(temp);
            }
        }
    }

    for (int j = 0; j < ba.count(); j++) {
        m_data[column][row + j] = (quint8)ba[j];
    }

    if (flag)
        Q_EMIT endInsertRows();
    else {
        Q_EMIT dataChanged(index(row, column), index(row + ba.count() - 1, column));
    }
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

QByteArray DataModel::getData(int column, int startRow, int count)
{
    QByteArray buffer;
    for (int i = 0; i < count; ++i) {

        buffer.append(m_data[column].at(i + startRow));
    }
    return buffer;
}

void DataModel::sltDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
    bool insert = false;
    bool remove = true;
    for (int j = 0; j < m_columns; j++) {
        if (topLeft.row() < m_data[j].count() && m_data[j][topLeft.row()] != -1) {
            insert = true;
            remove = false;
        }
    }
    if (insert && (topLeft.row() + 1 == m_data[0].count())) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        for (int j = 0; j < m_columns; j++) {
            QVector<int> temp(1);
            temp.fill(-1);
            m_data[j].append(temp);
        }
        endInsertRows();
    }
    if (remove && (topLeft.row() + 2 == m_data[0].count())) {
        beginRemoveRows(QModelIndex(), rowCount() - 1, rowCount() - 1);
        for (int j = 0; j < m_columns; j++) {
            m_data[j].remove(topLeft.row() + 1);
        }
        endRemoveRows();
    }
}

QVariant DataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QAbstractTableModel::headerData(section, orientation, role);
}
