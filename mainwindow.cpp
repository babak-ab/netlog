#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_lineEditDelegate = new LineEditDelegate;
    m_dataModel = new DataModel();

    ui->tableView_send->resizeRowsToContents();

    m_customHeader = new CustomHeader(ui->tableView_send);
    ui->tableView_send->setHorizontalHeader(m_customHeader);
    ui->tableView_send->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // ui->tableView_send->setItemDelegateForRow(0, new HeaderDelegate(ui->tableView_send));

    ui->tableView_send->setModel(m_dataModel);
    ui->tableView_send->setItemDelegate(m_lineEditDelegate);
    ui->tableView_send->setEditTriggers(QAbstractItemView::AllEditTriggers);

    //ui->tableView_send->setHorizontalHeader(new CustomHeader(ui->tableView_send));
    // MyHeader *myHeader = new MyHeader(Qt::Horizontal, ui->tableView_send);
    // ui->tableView_send->setHorizontalHeader(myHeader);

    m_columnsState.insert(0, false);
    m_columnsState.insert(1, false);
    m_columnsState.insert(2, false);
    m_columnsState.insert(3, false);
    m_columnsState.insert(4, false);
    m_columnsState.insert(5, false);

    ui->frame_send->setEnabled(false);

    connect(m_customHeader, &CustomHeader::sigStateChecked, [this](int column, bool checked) {
        m_columnsState[column] = checked;
        bool enable = false;
        for (int i = 0; i < m_columnsState.count(); i++) {
            if (m_columnsState[i]) {
                enable |= m_columnsState[i];
            }
            ui->frame_send->setEnabled(enable);
        }
    });

    ui->plainTextEdit_receive->setReadOnly(true);
    ui->plainTextEdit_send->setReadOnly(true);
    ui->plainTextEdit_receive->installEventFilter(this);
    ui->plainTextEdit_send->installEventFilter(this);

    connect(&m_sendTimer, &QTimer::timeout, [this]() {
        sendData();
    });

    //    connect(m_dataModel, &DataModel::rowsInserted, [this]() {
    //        QModelIndex index = ui->tableView_send->currentIndex();
    //        ui->tableView_send->setCurrentIndex(m_dataModel->index(index.row() + 1,index.column()));
    //    });
    connect(m_dataModel, &DataModel::dataChanged, [this](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles = QVector<int>()) {
        QModelIndex index = topLeft;
        if (m_dataModel->data(index, Qt::DisplayRole).isValid()) {
            ui->tableView_send->setCurrentIndex(m_dataModel->index(index.row() + 1, index.column()));
        } else {
            ui->tableView_send->setCurrentIndex(m_dataModel->index(index.row() - 1, index.column()));
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{

    for (int i = 0; i < m_dataModel->columnCount(); i++) {
        ui->tableView_send->setColumnWidth(i, ui->tableView_send->width() / m_dataModel->columnCount());
    }
    QMainWindow::resizeEvent(event);
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::Wheel) {
        QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
        QPlainTextEdit* plainText = static_cast<QPlainTextEdit*>(watched);
        if (wheelEvent && plainText && m_ctrlKeyPressed) {
            if (wheelEvent->angleDelta().y() > 0) {
                plainText->zoomIn();
            } else {
                plainText->zoomOut();
            }
        }
        return true;
    }
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (event->type() == QEvent::KeyPress && keyEvent->modifiers() == Qt::ControlModifier) {
            m_ctrlKeyPressed = true;
        }
        if (event->type() == QEvent::KeyRelease) {
            m_ctrlKeyPressed = false;
        }
    } else {
        // standard event processing
        return QObject::eventFilter(watched, event);
    }
}

void MainWindow::sendData()
{
    for (int i = 0; i < 6; i++) {
        if (m_columnsState[i]) {
            QByteArray ba = m_dataModel->getData(i);
            QString str;

            for (int j = 0; j < ba.size(); j++) {

                if (Setting::instance()->sendType() == Setting::InputType_Dec) {
                    str.append(QString::number((quint8)ba[j], 10).rightJustified(2, '0').toUpper());
                }
                if (Setting::instance()->sendType() == Setting::InputType_Hex) {
                    str.append(QString::number((quint8)ba[j], 16).rightJustified(2, '0').toUpper());
                }
                if (Setting::instance()->sendType() == Setting::InputType_ASCII) {
                    str.append(QString((char)ba[j]));
                }

                str.append(" ");
            }
            if (m_colorChange) {
                ui->plainTextEdit_send->appendHtml("<span style='color: black'>" + str + "</span>");
            } else {
                ui->plainTextEdit_send->appendHtml("<span style='color: blue'>" + str + "</span>");
            }
            m_colorChange = !m_colorChange;

            str.clear();
        }
    }
    ui->plainTextEdit_send->moveCursor(QTextCursor::End);
}

void MainWindow::on_pushButton_send_clicked()
{
    sendData();
}

void MainWindow::on_checkBox_interval_stateChanged(int arg1)
{
    if (ui->checkBox_interval->isChecked()) {
        m_sendTimer.setInterval(ui->spinBox_interval->value());
        m_sendTimer.start();
    } else {
        m_sendTimer.stop();
    }
    ui->pushButton_send->setEnabled(!ui->checkBox_interval->isChecked());
}

void MainWindow::on_spinBox_interval_valueChanged(int arg1)
{
    m_sendTimer.setInterval(ui->spinBox_interval->value());
}

void MainWindow::on_pushButton_sendClear_clicked()
{
    ui->plainTextEdit_send->clear();
}
