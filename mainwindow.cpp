#include "mainwindow.h"
#include <QBoxLayout>
#include <QDialog>
#include <QLabel>
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->mainSplitter->setSizes({200, 1000});
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionOpen_Sql_triggered() {
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Open SQL Script",
                                                    QDir::currentPath(),
                                                    "sql (*.sql)");
    if (filePath.isEmpty())
        return;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream io(&file);
    QTextEdit *textEdit = new QTextEdit(this);
    textEdit->setText(io.readAll());
    ui->tabWidget->addTab(textEdit, file.fileName());
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
    file.close();
}

// void MainWindow::on_actionOpen_Sql_triggered()
// {
//     QString filePath = QFileDialog::getOpenFileName(
//         this,
//         "Open SQL Script",
//         QDir::currentPath(),
//         "SQL Files (*.sql)"
//         );

//     if (filePath.isEmpty())
//         return;

//     QFile file(filePath);
//     if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//         // QMessageBox::warning(this, "Error", "No se pudo abrir el archivo");
//         return;
//     }

//     QTextStream io(&file);

//     QTextEdit *textEdit = new QTextEdit(this);
//     textEdit->setPlainText(io.readAll());
//     textEdit->setFont(QFont("Consolas", 10));

//     QFileInfo fileInfo(filePath);
//     ui->tabWidget->addTab(textEdit, fileInfo.fileName());
//     ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);

//     file.close();
// }




void MainWindow::on_actionSave_Sql_triggered() {
    QWidget *currentTab = ui->tabWidget->currentWidget();
    QTextEdit *edit = ui->tabWidget->findChild<QTextEdit*>();
}

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

void MainWindow::on_tabWidget_tabCloseRequested(int index) {
    ui->tabWidget->removeTab(index);
}

void MainWindow::on_actionNewScript_triggered() {
    ui->tabWidget->addTab(new QTextEdit(this), QString("Script %0").arg(ui->tabWidget->count()+1));
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
}
