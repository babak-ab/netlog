#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_actionGroupInputTypes = new QActionGroup(this);
    m_actionGroupInputTypes->addAction(ui->actionHex);
    m_actionGroupInputTypes->addAction(ui->actionDecimal);
    m_actionGroupInputTypes->setExclusionPolicy(QActionGroup::ExclusionPolicy::Exclusive);
    connect(m_actionGroupInputTypes, &QActionGroup::triggered, this, &MainWindow::sltActionGroupInputTypesTriggered);

    m_lineEditDelegate = new LineEditDelegate;
    m_dataModel = new DataModel(m_lineEditDelegate);

    ui->tableView_send->resizeRowsToContents();
    ui->tableView_send->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableView_send->setModel(m_dataModel);
    ui->tableView_send->setItemDelegate(m_lineEditDelegate);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sltActionGroupInputTypesTriggered(QAction* action)
{
    Q_UNUSED(action)
    if (ui->actionHex->isChecked()) {
        m_lineEditDelegate->setHexEnabled(true);
    }
    if (ui->actionDecimal->isChecked()) {
        m_lineEditDelegate->setHexEnabled(false);
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{

    for (int i = 0; i < m_dataModel->columnCount(); i++) {
        ui->tableView_send->setColumnWidth(i, ui->tableView_send->width() / m_dataModel->columnCount());
    }
    QMainWindow::resizeEvent(event);
}

void MainWindow::on_actionAddHexPrefix_triggered()
{
    m_lineEditDelegate->setAddHexPrefix(ui->actionAddHexPrefix->isChecked());
}

