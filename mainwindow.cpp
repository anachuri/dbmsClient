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

void MainWindow::on_actionOpenDatabase_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Open database",
                                                    QDir::currentPath(),
                                                    "database (*.db)");
    if (fileName.isEmpty())
        return;
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();
    treeItem->setIcon(0, QIcon(":/img/database.png"));
    treeItem->setText(0, fileName);
    database.setDatabaseName(fileName);
    if (!database.open()) {
        QMessageBox::critical(this, "Error", "an error has ocurred, database cannot be opened");
        return;
    }
    QSqlQuery query("select tbl_name from sqlite_master where type like 'table';");
    while (query.next()) {
        QTreeWidgetItem *table = new QTreeWidgetItem;
        table->setIcon(0, QIcon(":/img/cells.png"));
        table->setText(0, query.value(0).toString());
        treeItem->addChild(table);
    }
    ui->treeWidget->addTopLevelItem(treeItem);
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
    treeItem->setText(0, fileName);
    database.setDatabaseName(fileName.append(".db"));
    if (!database.open())
        QMessageBox::critical(this, "Error", "an error has ocurred, database cannot be saved");
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
    ScriptWidget *scriptWidget = new ScriptWidget(ui->tabWidget, filePath);
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

void MainWindow::on_actionPrint_triggered() {
    QPrintDialog printDialog(this);
    ScriptWidget *scriptWidget = currentScriptWidget();
    if (!currentScriptWidget())
        return;
    scriptWidget->print(printDialog.printer());
    printDialog.exec();
}

void MainWindow::on_actionClose_triggered() {
    close();
}

void MainWindow::on_actionExecute_triggered() {
    ScriptWidget *scriptWidget = currentScriptWidget();
    if (!currentScriptWidget())
        return;
    QString sql = scriptWidget->getScriptText();
    if (sql.startsWith("SELECT", Qt::CaseInsensitive)
        || sql.startsWith("PRAGMA", Qt::CaseInsensitive))
        queryModel->setQuery(sql);
    else
        QSqlQuery(sql).exec();
}

void MainWindow::on_actionPreferences_triggered() {}

void MainWindow::on_actionManual_triggered() {}

void MainWindow::on_actionAbout_dbmsClient_triggered() {
    QMessageBox::about(this, "dbmsClient", "Created by anachuri - January 2026");
}

void MainWindow::on_actionFind_and_replace_triggered() {
    FindReplaceDialog dialog(this);
    ScriptWidget *scriptWidget = currentScriptWidget();
    if (!scriptWidget)
        return;
    scriptWidget->setFindReplaceDialog(dialog);
    dialog.exec();
}

void MainWindow::on_actionNewScript_triggered() {
    ui->tabWidget->addTab(new ScriptWidget(ui->tabWidget, ""),
                          QString("Script %0").arg(ui->tabWidget->count() + 1));
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
    if (scriptWidget->getState() == ScriptState::Clean) {
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

void MainWindow::onTreeContextMenu(const QPoint &pos) {
    QTreeWidgetItem *item = ui->treeWidget->itemAt(pos);
    if (!item || item->childCount() == 0)
        return;
    QMenu menu(this);
    QAction *newTableAction = menu.addAction("New Table");
    //QAction *deleteAct = menu.addAction("Eliminar");
    QAction *selected = menu.exec(ui->treeWidget->viewport()->mapToGlobal(pos));
    if (!selected)
        return;

    if (selected == newTableAction) // {
        qDebug() << "Abrir:" << item->text(0);
    //} else if (selected == deleteAct) {
    //   delete item;
    //}
}

void MainWindow::on_treeWidget_clicked(const QModelIndex &index) {
    QTreeWidgetItem *item = ui->treeWidget->itemFromIndex(index);
    if (!item || item->childCount() == 0)
        return;
    database.setDatabaseName(item->text(0));
    if (!database.open())
        QMessageBox::critical(this, "Error", "An error has occurred, database cannot be opened");
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column) {
    if (item->childCount() > 0)
        return;
    QString sql = QString("PRAGMA table_info(%1)").arg(item->text(0));
    QSqlQuery query;
    QString content = QString("Table: %1").arg(item->text(0)).append("\n\nColumns: \n");
    if (query.exec(sql)) {
        while (query.next())
            content += QString("\t%1 %2\n").arg(query.value(1).toString(), query.value(2).toString());
        ui->tableInfo->setText(content);
    } else {
        ui->tableInfo->setText("Error en la consulta");
    }
}
