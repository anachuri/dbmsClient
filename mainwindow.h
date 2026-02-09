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
class QTreeWidgetItem;
class QFont;

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadTables(QTreeWidgetItem *treeItem);
    void setDatabase(QTreeWidgetItem *selectedDb, int index);
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
    ScriptWidget *currentScriptWidget() const;
    void onTreeContextMenu(const QPoint &pos);
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void onSetDatabaseActionTriggered();
    void onNewTableActionTriggered();
    void onFontChanged(QFont font);
    void onScriptContentChanged(const QString &fileName);

private:
    Ui::MainWindow *ui;
    QSqlDatabase database;
    QSqlQueryModel *queryModel;
    int dbIndex = 0;
};
#endif // MAINWINDOW_H
