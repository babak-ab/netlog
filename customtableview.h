#ifndef CUSTOMTABLEVIEW_H
#define CUSTOMTABLEVIEW_H

#include <QApplication>
#include <QClipboard>
#include <QContextMenuEvent>
#include <QMenu>
#include <QMimeData>
#include <QTableView>
#include <QWidget>
#include <setting.h>

class CustomTableView : public QTableView {
    Q_OBJECT
public:
    explicit CustomTableView(QWidget* parent = nullptr)
        : QTableView(parent)
    {
        setAcceptDrops(true);
    }

    // QWidget interface
protected:
    void contextMenuEvent(QContextMenuEvent* event)
    {
        QMenu menu(this);
        QMenu* subMenu = menu.addMenu("Input Type");

        QAction* dec = subMenu->addAction("Dec");
        dec->setData(Setting::InputType_Dec);
        dec->setCheckable(true);
        dec->setChecked(Setting::instance()->inputType() == Setting::InputType_Dec);

        QAction* hex = subMenu->addAction("Hex");
        hex->setData(Setting::InputType_Hex);
        hex->setCheckable(true);
        hex->setChecked(Setting::instance()->inputType() == Setting::InputType_Hex);

        QAction* ascii = subMenu->addAction("ASCII");
        ascii->setData(Setting::InputType_ASCII);
        ascii->setCheckable(true);
        ascii->setChecked(Setting::instance()->inputType() == Setting::InputType_ASCII);

        QActionGroup* actionGroup = new QActionGroup(this);
        actionGroup->addAction(dec);
        actionGroup->addAction(hex);
        actionGroup->addAction(ascii);
        actionGroup->setExclusive(true);

        connect(subMenu, &QMenu::triggered, [subMenu](QAction* action) {
            Setting::instance()->setInputType((Setting::InputType)action->data().toInt());
        });

        menu.exec(event->globalPos());
    }

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent* event)
    {
        switch (event->key()) {
        case Qt::Key_Delete:
        case Qt::Key_Backspace:
            model()->setData(currentIndex(), -1);
            break;
        default:
            QAbstractItemView::keyPressEvent(event);
        }
    }

    // QWidget interface
protected:
    void dropEvent(QDropEvent* event)
    {
        const QClipboard* clipboard = QApplication::clipboard();
        const QMimeData* mimeData = clipboard->mimeData();

        if (mimeData->hasText()) {
            qDebug() << mimeData->text();
        }
    }
};

#endif // CUSTOMTABLEVIEW_H
