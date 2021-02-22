#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "datamodel.h"

#include <QActionGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QHeaderView>
#include <QMainWindow>
#include <QMenu>
#include <QPainter>
#include <QPlainTextEdit>
#include <QPointer>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTableView>
#include <QTimer>
#include <lineeditdelegate.h>

class HeaderObject : public QWidget {
    Q_OBJECT
public:
    HeaderObject(QWidget* parent = 0)
        : QWidget(parent)
    {
        QCheckBox* cb = new QCheckBox(this);
        cb->setStyleSheet("::indicator {subcontrol-position: center; subcontrol-origin: padding;}");

        QHBoxLayout* l = new QHBoxLayout;

        l->setSpacing(0);
        l->setAlignment(cb, Qt::AlignCenter);
        l->setMargin(0);
        l->addWidget(cb);

        setLayout(l);

        connect(cb, &QCheckBox::stateChanged, [this, cb]() {
            Q_EMIT sigCheckStateChanged(cb->isChecked());
        });
    }
Q_SIGNALS:
    void sigCheckStateChanged(bool state);
};

class HeaderDelegate : public QStyledItemDelegate {
public:
    HeaderDelegate(QWidget* parent = 0)
        : QStyledItemDelegate(parent)
    {
        for (int i = 0; i < 6; i++) {
            headerSections.insert(i, new HeaderObject(qobject_cast<QTableView*>(parent)->viewport()));
            headerSections[i]->hide();
        }
    }

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        if (index.row() == 0) {
            qDebug() << "QRect: " << option.rect;
            headerSections[index.column()]->setGeometry(option.rect);
            headerSections[index.column()]->show();
        }
    }

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        qDebug() << "Asking for SIZEHINT: " << headerSections[0]->sizeHint();
        return headerSections[0]->sizeHint();
    }

private:
    QVector<QPointer<HeaderObject>> headerSections;
};

class CustomHeader : public QHeaderView {
    Q_OBJECT
public:
    CustomHeader(QWidget* parent = 0)
        : QHeaderView(Qt::Horizontal, parent)
    {
        for (int i = 0; i < 6; i++) {
            headerSections.insert(i, new HeaderObject(this));
            headerSections[i]->hide();

            connect(headerSections[i].data(), &HeaderObject::sigCheckStateChanged, [this, i](bool checked) {
                Q_EMIT sigStateChecked(i, checked);
            });
        }
    }

protected:
    void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const
    {
        if (!rect.isValid())
            return;

        QPen pen(Qt::gray);
        pen.setWidth(1);
        painter->setPen(pen);
        painter->drawRect(rect.x(), rect.y(), rect.width() - 2, rect.height() - 1);

        headerSections[logicalIndex]->setGeometry(rect);
        headerSections[logicalIndex]->show();
    }

private:
    QVector<QPointer<HeaderObject>> headerSections;

    // QWidget interface
public:
    QSize sizeHint() const
    {
        QSize baseSize = QHeaderView::sizeHint();

        // Override the height with a custom value.
        baseSize.setHeight(20);

        return baseSize;
    }
Q_SIGNALS:
    void sigStateChecked(int column, bool checked);
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow* ui;
    QActionGroup* m_actionGroupInputTypes;
    LineEditDelegate* m_lineEditDelegate;
    DataModel* m_dataModel;
    CustomHeader* m_customHeader;

    QTimer m_sendTimer;
    QMap<int, bool> m_columnsState;
    bool m_colorChange = false;
    bool m_ctrlKeyPressed = false;

    QSerialPort m_serialPort;
    QIntValidator* m_intValidator = nullptr;

    void fillSerialParameters();
    void fillSerialPorts();
private Q_SLOTS:

    void on_pushButton_send_clicked();

    void on_checkBox_interval_stateChanged(int arg1);

    void on_spinBox_interval_valueChanged(int arg1);

    void on_pushButton_sendClear_clicked();

    void on_pushButton_serialPortOpen_clicked();

    void checkCustomBaudRatePolicy(int idx);

    void checkCustomDevicePathPolicy(int idx);

protected:
    void resizeEvent(QResizeEvent* event);

    // QObject interface
public:
    bool eventFilter(QObject* watched, QEvent* event);
    void sendData();
};
#endif // MAINWINDOW_H
