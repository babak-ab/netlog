#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "datamodel.h"

#include <QActionGroup>
#include <QMainWindow>
#include <lineeditdelegate.h>

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

private Q_SLOTS:
    void sltActionGroupInputTypesTriggered(QAction* action);

    // QWidget interface
    void on_actionAddHexPrefix_triggered();

protected:
    void resizeEvent(QResizeEvent* event);
};
#endif // MAINWINDOW_H
