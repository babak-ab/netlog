#ifndef LINEEDITDELEGATE_H
#define LINEEDITDELEGATE_H

#include "setting.h"
#include <QDebug>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QStyledItemDelegate>

class LineEditDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit LineEditDelegate(QObject* parent = Q_NULLPTR);
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

#endif // LINEEDITDELEGATE_H
