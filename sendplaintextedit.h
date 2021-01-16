#ifndef SENDPLAINTEXTEDIT_H
#define SENDPLAINTEXTEDIT_H

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
        QMenu* subMenu = menu->addMenu("Convert");

        QAction* toDec = subMenu->addAction("to Dec");
        toDec->setData(0);
        QAction* toHex = subMenu->addAction("to Hex");
        toHex->setData(1);
        QAction* toBin = subMenu->addAction("to Bin");
        toBin->setData(2);

        QString currentText = this->textCursor().selectedText().trimmed();
        connect(subMenu, &QMenu::hovered, [subMenu, currentText](QAction* action) {
            QStringList list = currentText.split(' ');
            QString str;
            bool ok;
            for (int i = 0; i < list.count(); i++) {
                if (action->data() == 0) {
                    str += QString::number(list[i].toUInt(&ok, 16), 10).toUpper();
                }
                if (action->data() == 1) {
                    str += QString::number(list[i].toUInt(&ok, 16), 16).toUpper().rightJustified(2, '0');
                }
                if (action->data() == 2) {
                    str += QString::number(list[i].toUInt(&ok, 16), 2).toUpper().rightJustified(8, '0');                    
                }

                str += " ";
            }
            QClipboard* clipboard = QApplication::clipboard();

            clipboard->setText(str);

            QToolTip::showText(QPoint(subMenu->pos().x() + subMenu->width(),
                                   subMenu->pos().y()),
                str);
        });

        menu->exec(event->globalPos());
        delete menu;
    }
};

#endif // SENDPLAINTEXTEDIT_H
