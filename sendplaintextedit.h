#ifndef SENDPLAINTEXTEDIT_H
#define SENDPLAINTEXTEDIT_H

#include "setting.h"
#include <QApplication>
#include <QClipboard>
#include <QContextMenuEvent>
#include <QDebug>
#include <QDialog>
#include <QMenu>
#include <QPlainTextEdit>
#include <QThread>
#include <QToolTip>

class SendPlainTextEdit : public QPlainTextEdit {

public:
    explicit SendPlainTextEdit(QWidget* parent = nullptr)
        : QPlainTextEdit(parent)
    {
    }
    // QWidget interface
protected:
    void contextMenuEvent(QContextMenuEvent* event)
    {
        QMenu* menu = createStandardContextMenu();
        QMenu* subMenuConvert = menu->addMenu("Convert");
        QMenu* subMenuType = menu->addMenu("Diplay Type");
        subMenuConvert->setEnabled(this->textCursor().selectedText().trimmed() != "");

        QAction* toDec = subMenuConvert->addAction("to Dec");
        toDec->setData(Setting::InputType_Dec);
        QAction* toHex = subMenuConvert->addAction("to Hex");
        toHex->setData(Setting::InputType_Hex);
        QAction* toBin = subMenuConvert->addAction("to Bin");
        toBin->setData(Setting::InputType_Bin);
        QAction* toAscii = subMenuConvert->addAction("to Ascii");
        toAscii->setData(Setting::InputType_ASCII);

        QString currentText = this->textCursor().selectedText().trimmed();

        connect(subMenuConvert, &QMenu::hovered, [subMenuConvert, currentText](QAction* action) {
            QStringList list = currentText.split(' ');
            QString str;
            bool ok;
            for (int i = 0; i < list.count(); i++) {
                int base = 0;
                if (Setting::instance()->sendType() == Setting::InputType_Dec)
                    base = 10;
                if (Setting::instance()->sendType() == Setting::InputType_Hex)
                    base = 16;

                if (action->data() == Setting::InputType_Dec) {
                    if (base == 0) {
                        str += QString::number(list[i].toLatin1()[0], 10).toUpper();
                    } else {
                        str += QString::number(list[i].toUInt(&ok, base), 10).toUpper();
                    }
                }
                if (action->data() == Setting::InputType_Hex) {
                    if (base == 0) {
                        str += QString::number(list[i].toLatin1()[0], 16).toUpper();
                    } else {
                        str += QString::number(list[i].toUInt(&ok, base), 16).toUpper().rightJustified(2, '0');
                    }
                }
                if (action->data() == Setting::InputType_Bin) {
                    if (base == 0) {
                        str += QString::number(list[i].toLatin1()[0], 2).toUpper();
                    } else {
                        str += QString::number(list[i].toUInt(&ok, base), 2).toUpper().rightJustified(8, '0');
                    }
                }
                if (action->data() == Setting::InputType_ASCII) {
                    if (base == 0) {
                        str += list[i].toLatin1();
                    } else {
                        str += QString((char)list[i].toUInt(&ok, base));
                    }
                }

                str += " ";
            }
            QClipboard* clipboard = QApplication::clipboard();

            clipboard->setText(str);

            QToolTip::showText(QPoint(subMenuConvert->pos().x() + subMenuConvert->width(),
                                   subMenuConvert->pos().y()),
                str);
        });

        QAction* dec = subMenuType->addAction("to Dec");
        dec->setData(Setting::InputType_Dec);
        QAction* hex = subMenuType->addAction("to Hex");
        hex->setData(Setting::InputType_Hex);
        QAction* ascii = subMenuType->addAction("to Ascii");
        ascii->setData(Setting::InputType_ASCII);

        connect(subMenuType, &QMenu::triggered, [subMenuType](QAction* action) {
            Setting::instance()->setSendType((Setting::InputType)action->data().toInt());
        });

        menu->exec(event->globalPos());
        delete menu;
    }
};

#endif // SENDPLAINTEXTEDIT_H
