#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}

class ScriptWidget;
class QSqlQueryModel;

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void onTabWidgetCloseRequested();
private slots:
    void on_actionOpen_Sql_triggered();
    void on_actionSave_Sql_triggered();
    void on_actionPrint_triggered();
    void on_actionClose_triggered();
    void on_actionExecute_triggered();
    void on_actionPreferences_triggered();
    void on_actionManual_triggered();
    void on_actionAbout_dbmsClient_triggered();
    void on_actionFind_and_replace_triggered();
    void on_actionOpenDatabase_triggered();
    void on_actionSaveDatabase_triggered();
    void on_actionNewScript_triggered();
    void on_tabWidget_tabCloseRequested(int index);
    void saveScriptFile(const ScriptWidget *scriptWidget);
    void on_actionCopy_triggered();
    void on_actionCut_triggered();
    void on_actionPaste_triggered();
    ScriptWidget *currentScriptWidget() const;
    void on_treeWidget_clicked(const QModelIndex &index);
    void onTreeContextMenu(const QPoint &pos);

private:
    Ui::MainWindow *ui;
    QSqlDatabase database;
    QSqlQueryModel *queryModel;
};
#endif // MAINWINDOW_H
