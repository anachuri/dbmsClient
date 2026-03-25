#include "mainwindow.h"
#include <QBoxLayout>
#include <QDebug>
#include <QDialog>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QMimeData>
#include <QPrintDialog>
#include <QPrinter>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTextEdit>
#include <QUrl>
#include "./ui_mainwindow.h"
#include "newtabledialog.h"
#include "preferencesdialog.h"
#include "scriptwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->mainSplitter->setSizes({300, 1000});
    ui->databaseSplitter->setSizes({700, 300});
    on_actionNewScript_triggered();
    database = QSqlDatabase::addDatabase("QSQLITE");
    queryModel = new QSqlQueryModel(this);
    ui->tableView->setModel(queryModel);
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget,
            &QTreeWidget::customContextMenuRequested,
            this,
            &MainWindow::onTreeContextMenu);
    setAcceptDrops(true);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::loadTables(QTreeWidgetItem *treeItem) {
    QSqlQuery query("select tbl_name from sqlite_master where type like 'table';");
    while (query.next()) {
        QTreeWidgetItem *table = new QTreeWidgetItem;
        table->setIcon(0, QIcon(":/img/cells-white"));
        table->setText(0, query.value(0).toString());
        treeItem->addChild(table);
    }
}

bool MainWindow::isDatabaseOpen(const QString &database) {
    int i;
    for (i = 0; i < ui->treeWidget->topLevelItemCount()
                && database != ui->treeWidget->topLevelItem(i)->text(0);
         i++)
        ;
    return i != ui->treeWidget->topLevelItemCount();
}

void MainWindow::on_actionOpen_Sql_triggered() {
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Open SQL Script",
                                                    QDir::currentPath(),
                                                    "SQL Files (*.sql)");
    if (filePath.isEmpty())
        return;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream io(&file);
    ScriptWidget *scriptWidget = new ScriptWidget(ui->tabWidget,
                                                  filePath,
                                                  QFileInfo(filePath).fileName(),
                                                  ui->actionCopy,
                                                  ui->actionCut,
                                                  ui->actionPaste);
    connect(scriptWidget, &ScriptWidget::contentChanged, this, &MainWindow::onScriptContentChanged);
    ui->tabWidget->addTab(scriptWidget, QFileInfo(file).fileName());
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
    scriptWidget->loadScript(io.readAll());
    file.close();
}

void MainWindow::on_actionSave_Sql_triggered() {
    ScriptWidget *scriptWidget = currentScriptWidget();
    if (!scriptWidget || scriptWidget->getState() == ScriptState::Clean)
        return;
    if (scriptWidget->isFilePathEmpty()) {
        QString filePath = QFileDialog::getSaveFileName(this, "Save Script");
        if (filePath.isEmpty())
            return;
        scriptWidget->setFilePath(filePath.append(".sql"));
        scriptWidget->setFileName(QFileInfo(filePath).fileName());
    }
    saveScriptFile(scriptWidget);
    scriptWidget->setClean();
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), scriptWidget->getFileName());
}

void MainWindow::on_actionExecute_triggered() {
    ScriptWidget *scriptWidget = currentScriptWidget();
    if (!scriptWidget || !ui->treeWidget->topLevelItem(dbIndex))
        return;
    QTreeWidgetItem *itemDb = ui->treeWidget->topLevelItem(dbIndex);
    if (itemDb->parent())
        itemDb = itemDb->parent();
    QString sql = scriptWidget->getScriptText();
    QSqlQuery qry;
    if (sql.startsWith("select", Qt::CaseInsensitive)) {
        queryModel->setQuery(sql);
    } else if (sql.startsWith("create table", Qt::CaseInsensitive)) {
        QTreeWidgetItem *itemTable = new QTreeWidgetItem;
        itemTable->setIcon(0, QIcon(":/img/cells-white"));
        if (!qry.exec(sql)) {
            ui->listWidget->addItem(
                new QListWidgetItem(QIcon(":/img/error"),
                                    sql.append(" - error:  " + qry.lastError().text())));
            ui->listWidget->scrollToBottom();
            return;
        }
        QString createTable = QString("create table ");
        int pos1 = createTable.size();
        int pos2 = sql.indexOf("(", pos1);
        QString tableName = sql.mid(pos1, pos2 - pos1);
        tableName = tableName.mid(0, tableName.indexOf(" "));
        itemTable->setText(0, tableName);
        itemDb->addChild(itemTable);
    } else if (sql.startsWith("drop table", Qt::CaseInsensitive)) {
        if (!qry.exec(sql)) {
            ui->listWidget->addItem(
                new QListWidgetItem(QIcon(":/img/error"),
                                    sql.append(" - error:  " + qry.lastError().text())));
            ui->listWidget->scrollToBottom();
            return;
        }
        QString dropTable = QString("drop table ");
        int pos1 = dropTable.size();
        int pos2 = sql.indexOf("(", pos1);
        QString tableName = sql.mid(pos1, pos2 - pos1);
        tableName = tableName.mid(0, tableName.indexOf(" "));
        int i = 0;
        for (i = 0; i < itemDb->childCount() && itemDb->child(i)->text(0) != tableName; i++)
            ;
        itemDb->takeChild(i);
    } else {
        if (!qry.exec(sql)) {
            ui->listWidget->addItem(
                new QListWidgetItem(QIcon(":/img/error"),
                                    sql.append(" - error:  " + qry.lastError().text())));
            ui->listWidget->scrollToBottom();
            return;
        }
    }
    ui->listWidget->addItem(new QListWidgetItem(QIcon(":/img/success"), sql));
    ui->listWidget->scrollToBottom();
}

void MainWindow::onScriptContentChanged(const QString &fileName) {
    if (ui->tabWidget->tabText(ui->tabWidget->currentIndex()).endsWith("*"))
        return;
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), fileName + "*");
}

void MainWindow::on_actionPreferences_triggered() {
    PreferencesDialog dialog(this);
    for (int i = 0; i < ui->tabWidget->count(); i++)
        connect(&dialog,
                &PreferencesDialog::settingsChanged,
                qobject_cast<ScriptWidget *>(ui->tabWidget->widget(i)),
                &ScriptWidget::onSettingsChanged);
    dialog.exec();
}

void MainWindow::on_actionManual_triggered() {}

void MainWindow::on_actionNewScript_triggered() {
    ScriptWidget *scriptWidget
        = new ScriptWidget(ui->tabWidget,
                           "",
                           QString("Script %0").arg(ui->tabWidget->count() + 1),
                           ui->actionCopy,
                           ui->actionCut,
                           ui->actionPaste);
    connect(scriptWidget, &ScriptWidget::contentChanged, this, &MainWindow::onScriptContentChanged);
    ui->tabWidget->addTab(scriptWidget, scriptWidget->getFileName());
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
}

void MainWindow::saveScriptFile(const ScriptWidget *scriptWidget) {
    QFile file(scriptWidget->getFilePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "an error has ocurred: ", file.errorString());
        return;
    }
    QTextStream io(&file);
    io << scriptWidget->getScriptText();
    file.flush();
    file.close();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index) {
    ScriptWidget *scriptWidget = currentScriptWidget();
    if (!scriptWidget || scriptWidget->getState() != ScriptState::Modified) {
        ui->tabWidget->removeTab(index);
        return;
    }
    QMessageBox::StandardButton resBtn = QMessageBox::question(
        this,
        "Save changes",
        tr("The script has unsaved changes, Do you want to save them before closing?"),
        QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
        QMessageBox::Yes);
    if (resBtn == QMessageBox::Cancel)
        return;
    if (resBtn == QMessageBox::No) {
        ui->tabWidget->removeTab(index);
        return;
    }
    if (scriptWidget->isFilePathEmpty()) {
        QString filePath = QFileDialog::getSaveFileName(this, "Save Script");
        if (filePath.isEmpty())
            return;
        scriptWidget->setFilePath(filePath.append(".sql"));
        scriptWidget->setFileName(QFileInfo(filePath).fileName());
    }
    saveScriptFile(scriptWidget);
    ui->tabWidget->removeTab(index);
}

void MainWindow::onTreeContextMenu(const QPoint &pos) {
    QTreeWidgetItem *item = ui->treeWidget->itemAt(pos);
    if (!item)
        return;
    QMenu menu(this);
    if(item->parent()){
        QAction *selectFromAction = menu.addAction("Select * from");
        QAction *dropTableAction = menu.addAction("Drop table");
        connect(dropTableAction, &QAction::triggered, this, &MainWindow::onDropTableActionTriggered);
        connect(selectFromAction,
                &QAction::triggered,
                this,
                &MainWindow::onSelectFromActionTriggered);
        menu.exec(ui->treeWidget->viewport()->mapToGlobal(pos));
        return;
    }
    QAction *newTableAction = menu.addAction("New Table");
    QAction *setDatabaseAction = menu.addAction("Set as default database");
    connect(setDatabaseAction, &QAction::triggered, this, &MainWindow::onSetDatabaseActionTriggered);
    connect(newTableAction, &QAction::triggered, this, &MainWindow::onNewTableActionTriggered);
    menu.exec(ui->treeWidget->viewport()->mapToGlobal(pos));
}

void MainWindow::onSelectFromActionTriggered() {
    QSqlQuery query;
    QString sql = "select * from " + ui->treeWidget->currentItem()->text(0);
    queryModel->setQuery(sql);
    ui->listWidget->addItem(new QListWidgetItem(QIcon(":/img/success"), sql));
    ui->listWidget->scrollToBottom();
}

void MainWindow::onSetDatabaseActionTriggered() {
    QTreeWidgetItem *dbItem = ui->treeWidget->currentItem();
    setDatabase(dbItem, ui->treeWidget->indexOfTopLevelItem(dbItem));
}

void MainWindow::onNewTableActionTriggered() {
    NewTableDialog dialog(this);
    connect(&dialog, &NewTableDialog::scriptGenerated, this, &MainWindow::addTable);
    dialog.exec();
}

void MainWindow::addTable(const QString &sql, const QString &tableName) {
    QSqlQuery query;
    if (!query.exec(sql)) {
        QMessageBox::critical(this, "Error while creating table", query.lastError().text());
        return;
    }
    QTreeWidgetItem *itemDb = ui->treeWidget->topLevelItem(dbIndex);
    QTreeWidgetItem *itemTable = new QTreeWidgetItem;
    itemTable->setIcon(0, QIcon(":/img/cells-white"));
    itemTable->setText(0, tableName);
    itemDb->addChild(itemTable);
}

void MainWindow::onDropTableActionTriggered() {
    QSqlQuery query;
    QString sql = "drop table " + ui->treeWidget->currentItem()->text(0);
    if (!query.exec(sql) && query.lastError().text().contains("no such table")) {
        QMessageBox::critical(this,
                              "Error while deleting table",
                              "you only can delete tables from database setted as default");
        return;
    }
    ui->treeWidget->currentItem()->parent()->removeChild(ui->treeWidget->currentItem());
    ui->listWidget->scrollToBottom();
}

void MainWindow::setDatabase(QTreeWidgetItem *selectedDb, int index) {
    QString filePath = selectedDb->data(0, Qt::UserRole).toString();
    database.setDatabaseName(filePath);
    if (!database.open()) {
        QMessageBox::critical(this, "Error", "An error has occurred, database cannot be opened");
        return;
    }
    QTreeWidgetItem *item = ui->treeWidget->topLevelItem(dbIndex);
    if (!item)
        return;
    QFont font = item->font(0);
    font.setBold(false);
    item->setFont(0, font);
    //item->setText(0, item->text(0));
    font = selectedDb->font(0);
    font.setBold(true);
    selectedDb->setFont(0, font);
    dbIndex = index;
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column) {
    if (!item->parent())
        return;
    QString sql = QString("PRAGMA table_info(%1)").arg(item->text(0));
    QSqlQuery query;
    if (query.exec(sql)) {
        if (query.next()) {
            QString content = QString("Table: %1").arg(item->text(0)).append("\n\nColumns: \n");
            do {
                content += QString("\t%1 %2\n")
                               .arg(query.value(1).toString(), query.value(2).toString());
                ui->tableInfo->setText(content);
            } while (query.next());
        } else
            ui->tableInfo->setText("");
    } else {
        ui->tableInfo->setText("");
    }
}

ScriptWidget *MainWindow::currentScriptWidget() const {
    int index = ui->tabWidget->currentIndex();
    if (index < 0)
        return nullptr;
    return qobject_cast<ScriptWidget *>(ui->tabWidget->widget(index));
}

void MainWindow::on_actionPrint_triggered() {
    QPrintDialog printDialog(this);
    ScriptWidget *scriptWidget = currentScriptWidget();
    if (!currentScriptWidget())
        return;
    scriptWidget->print(printDialog.printer());
    printDialog.exec();
}

void MainWindow::on_actionFind_and_replace_triggered() {
    FindReplaceDialog dialog(this);
    ScriptWidget *scriptWidget = currentScriptWidget();
    if (!scriptWidget)
        return;
    scriptWidget->setFindReplaceDialog(dialog);
    dialog.exec();
}

void MainWindow::on_actionClose_triggered() {
    close();
}

void MainWindow::on_actionAbout_dbmsClient_triggered() {
    QMessageBox::about(this, "dbmsClient", "Created by anachuri - January 2026");
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    event->accept();
}
void MainWindow::dragLeaveEvent(QDragLeaveEvent *event) {
    event->accept();
}
void MainWindow::dragMoveEvent(QDragMoveEvent *event) {
    event->accept();
}
void MainWindow::dropEvent(QDropEvent *event) {
    for (const QUrl &url : event->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        //to do controlar la extension
        if (!isDatabaseOpen(QFileInfo(fileName).fileName()))
            addDatabase(fileName);
    }
}

void MainWindow::addDatabase(const QString &fileName) {
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();
    treeItem->setIcon(0, QIcon(":/img/database-white"));
    treeItem->setText(0, QFileInfo(fileName).fileName());
    treeItem->setData(0, Qt::UserRole, QVariant(fileName));
    ui->treeWidget->addTopLevelItem(treeItem);
    setDatabase(treeItem, ui->treeWidget->topLevelItemCount() - 1);
    loadTables(treeItem);
}

void MainWindow::on_actionOpenDatabase_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Open database",
                                                    QDir::currentPath(),
                                                    "database (*.db)");
    if (fileName.isEmpty() || isDatabaseOpen(QFileInfo(fileName).fileName()))
        return;
    addDatabase(fileName);
}

void MainWindow::on_actionSaveDatabase_triggered() {
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Save database",
                                                    QDir::currentPath(),
                                                    "database (*.db)");
    if (fileName.isEmpty())
        return;
    if (!fileName.endsWith(".db", Qt::CaseInsensitive))
        fileName.append(".db");
    addDatabase(fileName);
}
