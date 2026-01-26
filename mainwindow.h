#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dbtreewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_actionOpen_Sql_triggered();
    void on_actionSave_Sql_triggered();
    void on_actionPrint_triggered();
    void on_actionClose_triggered();
    // void on_actionCopy_triggered();
    // void on_actionCut_triggered();
    // void on_actionPaste_triggered();
    void on_actionExecute_triggered();
    void on_actionPreferences_triggered();
    void on_actionManual_triggered();
    void on_actionAbout_dbmsClient_triggered();
    void on_actionFind_and_replace_triggered();
    void on_actionOpenDatabase_triggered();
    void on_actionSaveDatabase_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
