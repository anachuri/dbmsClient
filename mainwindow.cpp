#include "mainwindow.h"
#include <QBoxLayout>
#include <QDebug>
#include <QDialog>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include "./ui_mainwindow.h"
#include "scriptwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->mainSplitter->setSizes({200, 1000});
    on_actionNewScript_triggered();
}

MainWindow::~MainWindow() {
    delete ui;
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
    ScriptWidget *scriptWidget = new ScriptWidget(this,filePath);
    scriptWidget->loadScript(io.readAll());
    ui->tabWidget->addTab(scriptWidget, QFileInfo(file).fileName());
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
    file.close();
}

void MainWindow::on_actionSave_Sql_triggered() {}

void MainWindow::on_actionPrint_triggered() {}

void MainWindow::on_actionClose_triggered() {
    close();
}

// void MainWindow::on_actionCopy_triggered() {
//     ui->textEdit->copy();
// }

// void MainWindow::on_actionCut_triggered() {
//     ui->textEdit->cut();
// }

// void MainWindow::on_actionPaste_triggered() {
//     ui->textEdit->paste();
// }

void MainWindow::on_actionExecute_triggered() {}

void MainWindow::on_actionPreferences_triggered() {}

void MainWindow::on_actionManual_triggered() {}

void MainWindow::on_actionAbout_dbmsClient_triggered() {
    QMessageBox::about(this, "dbmsClient", "Created by anachuri - January 2026");
}

void MainWindow::on_actionFind_and_replace_triggered() {}

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
    ui->treeWidget->addTopLevelItem(treeItem);
}

void MainWindow::on_actionSaveDatabase_triggered() {}

void MainWindow::on_actionNewScript_triggered() {
    ui->tabWidget->addTab(new ScriptWidget(ui->tabWidget), QString("Script %0").arg(ui->tabWidget->count() + 1));
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
}

void MainWindow::saveScriptFile(const ScriptWidget *scriptWidget){
    QFile file(scriptWidget->getFilePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "error guardar", file.errorString());
        return;
    }
    QTextStream io(&file);
    io << scriptWidget->getScriptText();
    file.flush();
    file.close();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index) {
    QWidget *selectedTab = ui->tabWidget->widget(index);
    ScriptWidget *scriptWidget = static_cast<ScriptWidget *>(selectedTab);
    if(scriptWidget->isClosable()){
        ui->tabWidget->removeTab(index);
        return;
    }
    if(scriptWidget->isEdited()){
        QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Guardar cambios",
                                                                   tr("El script tiene cambios sin guardar.\n¿Deseas guardarlos antes de cerrar?"),
                                                                   QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                   QMessageBox::Yes);
        if (resBtn == QMessageBox::Yes) {
            if(scriptWidget->isFilePathEmpty()){
                //saveNewScriptFile();
                ui->tabWidget->removeTab(index);
                //tab->deleteLater();
            }
        } else if (resBtn == QMessageBox::No) {
            ui->tabWidget->removeTab(index);
            //tab->deleteLater();
        }
    }
}


