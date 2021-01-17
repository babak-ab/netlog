#include "lineeditdelegate.h"

#include <QEvent>
#include <qevent.h>

LineEditDelegate::LineEditDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

QWidget* LineEditDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QLineEdit* editor = new QLineEdit(parent);

    if (Setting::instance()->inputType() == Setting::InputType_Hex) {
        QRegExp re("[0-9a-fA-F]{1,2}");
        QRegExpValidator* validator = new QRegExpValidator(re);
        editor->setValidator(validator);
    } else if (Setting::instance()->inputType() == Setting::InputType_Dec) {
        QRegExp re("[0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5]");
        //QRegExp re("0[xX][0-9a-fA-F]{1,2}");
        QRegExpValidator* validator = new QRegExpValidator(re);
        editor->setValidator(validator);
    } else if (Setting::instance()->inputType() == Setting::InputType_ASCII) {
        QRegExp re("[ -~]");
        QRegExpValidator* validator = new QRegExpValidator(re);
        editor->setValidator(validator);
    }

    if (editor->hasAcceptableInput() == false) {
        editor->clear();
    }

    editor->setAlignment(Qt::AlignCenter);

    connect(editor, &QLineEdit::textChanged, [editor]() {
        if (editor->text() == "") {
            editor->setValidator(Q_NULLPTR);
        } else {
            if (Setting::instance()->inputType() == Setting::InputType_Hex) {
                QRegExp re("[0-9a-fA-F]{1,2}");
                QRegExpValidator* validator = new QRegExpValidator(re);
                editor->setValidator(validator);
            } else if (Setting::instance()->inputType() == Setting::InputType_Dec) {
                QRegExp re("[0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5]");
                //QRegExp re("0[xX][0-9a-fA-F]{1,2}");
                QRegExpValidator* validator = new QRegExpValidator(re);
                editor->setValidator(validator);
            } else if (Setting::instance()->inputType() == Setting::InputType_ASCII) {
                QRegExp re("[ -~]");
                QRegExpValidator* validator = new QRegExpValidator(re);
                editor->setValidator(validator);
            }
        }
    });

    return editor;
}

void LineEditDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QString value = index.model()->data(index, Qt::DisplayRole).toString();
    QLineEdit* line = static_cast<QLineEdit*>(editor);
    line->setAlignment(Qt::AlignCenter);
    line->setText(value);
}

void LineEditDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QLineEdit* line = static_cast<QLineEdit*>(editor);
    QString value = line->text();
    if (value == "") {
        model->setData(index, -1);
    } else {
        model->setData(index, value);
    }
}

void LineEditDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    editor->setGeometry(option.rect);
}
