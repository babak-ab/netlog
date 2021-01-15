#include "lineeditdelegate.h"

LineEditDelegate::LineEditDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
    m_hexEnabled = true;
    m_addHexPrefix = true;
}

QWidget* LineEditDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QLineEdit* editor = new QLineEdit(parent);

    if (m_hexEnabled) {
        QRegExp re("[0-9a-fA-F]{1,2}");
        QRegExpValidator* validator = new QRegExpValidator(re);
        editor->setValidator(validator);
    } else {
        QRegExp re("[0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5]");
        //QRegExp re("0[xX][0-9a-fA-F]{1,2}");
        QRegExpValidator* validator = new QRegExpValidator(re);
        editor->setValidator(validator);
    }
    if(editor->hasAcceptableInput() == false){
        editor->clear();
    }

    return editor;
}

void LineEditDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit* line = static_cast<QLineEdit*>(editor);
    line->setText(value);
}

void LineEditDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QLineEdit* line = static_cast<QLineEdit*>(editor);
    QString value = line->text();
    if (m_hexEnabled && m_addHexPrefix) {
        model->setData(index, "0x" + value);
    } else {
        model->setData(index, value);
    }
}

void LineEditDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    editor->setGeometry(option.rect);
}

void LineEditDelegate::setHexEnabled(bool enabled)
{
    m_hexEnabled = enabled;
}

bool LineEditDelegate::hexEnabled() const
{
    return m_hexEnabled;
}

bool LineEditDelegate::addHexPrefix() const
{
    return m_addHexPrefix;
}

void LineEditDelegate::setAddHexPrefix(bool addHexPrefix)
{
    m_addHexPrefix = addHexPrefix;
}
