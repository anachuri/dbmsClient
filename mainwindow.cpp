#include "mainwindow.h"
#include <QBoxLayout>
#include <QDebug>
#include <QDialog>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPrintDialog>
#include <QPrinter>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "./ui_mainwindow.h"
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

void MainWindow::on_actionOpenDatabase_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Open database",
                                                    QDir::currentPath(),
                                                    "database (*.db)");
    if (fileName.isEmpty())
        return;
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();
    treeItem->setIcon(0, QIcon(":/img/database-white"));
    treeItem->setText(0, QFileInfo(fileName).fileName());
    treeItem->setData(0, Qt::UserRole, QVariant(fileName));
    ui->treeWidget->addTopLevelItem(treeItem);
    setDatabase(treeItem);
    loadTables(treeItem);
}

void MainWindow::on_actionSaveDatabase_triggered() {
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Save database",
                                                    QDir::currentPath(),
                                                    "database (*.db)");
    if (fileName.isEmpty())
        return;
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();
    treeItem->setIcon(0, QIcon(":/img/database.png"));
    treeItem->setText(0, QFileInfo(fileName).fileName());
    treeItem->setData(0, Qt::UserRole, QVariant(fileName.append(".db")));
    setDatabase(treeItem);
    ui->treeWidget->addTopLevelItem(treeItem);
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
                                                  QFileInfo(filePath).fileName());
    ui->tabWidget->addTab(scriptWidget, QFileInfo(file).fileName());
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
    scriptWidget->loadScript(io.readAll());
    file.close();
}

void MainWindow::on_actionSave_Sql_triggered() {
    ScriptWidget *scriptWidget = currentScriptWidget();
    if (!scriptWidget)
        return;
    if (scriptWidget->getState() == ScriptState::Clean)
        return;
    if (scriptWidget->isFilePathEmpty()) {
        QString filePath = QFileDialog::getSaveFileName(this, "Save Script");
        if (filePath.isEmpty())
            return;
        scriptWidget->setFilePath(filePath.append(".sql"));
    }
    saveScriptFile(scriptWidget);
    scriptWidget->setClean();
}

void MainWindow::on_actionExecute_triggered() {
    ScriptWidget *scriptWidget = currentScriptWidget();
    if (!scriptWidget || !ui->treeWidget->currentItem())
        return;
    QTreeWidgetItem *itemDb = ui->treeWidget->currentItem();
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
            return;
        }
    }
    ui->listWidget->addItem(new QListWidgetItem(QIcon(":/img/success"), sql));
}

void MainWindow::on_actionPreferences_triggered() {
    PreferencesDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_actionManual_triggered() {}

void MainWindow::on_actionNewScript_triggered() {
    ScriptWidget *scriptWidget
        = new ScriptWidget(ui->tabWidget, "", QString("Script %0").arg(ui->tabWidget->count() + 1));
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
    if (!scriptWidget)
        return;
    if (scriptWidget->getState() == ScriptState::Clean
        || scriptWidget->getState() == ScriptState::New) {
        ui->tabWidget->removeTab(index);
        return;
    }
    QMessageBox::StandardButton resBtn = QMessageBox::question(
        this,
        "Save changes",
        tr("The script has unsaved changes, Do you want to save them before closing?"),
        QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
        QMessageBox::Yes);
    if (resBtn == QMessageBox::Yes) {
        if (scriptWidget->isFilePathEmpty()) {
            QString filePath = QFileDialog::getSaveFileName(this, "Save Script");
            if (filePath.isEmpty())
                return;
            scriptWidget->setFilePath(filePath.append(".sql"));
        }
        saveScriptFile(scriptWidget);
        ui->tabWidget->removeTab(index);
        //tab->deleteLater();
    } else if (resBtn == QMessageBox::No) {
        ui->tabWidget->removeTab(index);
        //tab->deleteLater();
    }
}

void MainWindow::onTreeContextMenu(const QPoint &pos) {
    QTreeWidgetItem *item = ui->treeWidget->itemAt(pos);
    if (!item || item->parent())
        return;
    QMenu menu(this);
    QAction *newTableAction = menu.addAction("New Table");
    QAction *setDatabaseAction = menu.addAction("Set as default database");
    connect(newTableAction, &QAction::triggered, this, &MainWindow::onNewTableActionTriggered);
    connect(setDatabaseAction, &QAction::triggered, this, &MainWindow::onSetDatabaseActionTriggered);
    menu.exec(ui->treeWidget->viewport()->mapToGlobal(pos));
}

void MainWindow::onSetDatabaseActionTriggered() {
    QTreeWidgetItem *dbItem = ui->treeWidget->currentItem();
    setDatabase(dbItem);
}

void MainWindow::onNewTableActionTriggered() {
    PreferencesDialog dialog(this);

    QVBoxLayout layout(&dialog);
    layout.addWidget(new QTableView(&dialog));

    dialog.exec(); // modal y seguro
}

void MainWindow::setDatabase(QTreeWidgetItem *selectedDb) {
    QString filePath = selectedDb->data(0, Qt::UserRole).toString();
    database.setDatabaseName(filePath);
    if (!database.open()) {
        QMessageBox::critical(this, "Error", "An error has occurred, database cannot be opened");
        return;
    }
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++) {
        QTreeWidgetItem *item = ui->treeWidget->topLevelItem(i);
        QFont font = item->font(0);
        font.setBold(false);
        item->setFont(0, font);
        //item->setText(0, item->text(0));
    }
    QFont font = selectedDb->font(0);
    font.setBold(true);
    selectedDb->setFont(0, font);
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

void MainWindow::on_actionCopy_triggered() {
    ScriptWidget *scriptWidget = currentScriptWidget();
    if (scriptWidget)
        scriptWidget->copyText();
}

void MainWindow::on_actionCut_triggered() {
    ScriptWidget *scriptWidget = currentScriptWidget();
    if (scriptWidget)
        scriptWidget->cutText();
}

void MainWindow::on_actionPaste_triggered() {
    ScriptWidget *scriptWidget = currentScriptWidget();
    if (scriptWidget)
        scriptWidget->pasteText();
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
