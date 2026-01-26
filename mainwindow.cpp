#include "mainwindow.h"
#include <QBoxLayout>
#include <QDialog>
#include <QLabel>
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->mainSplitter->setSizes({200, 1000});
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionOpen_Sql_triggered() {}

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

void MainWindow::on_tabWidget_tabCloseRequested(int index) {
    ui->tabWidget->removeTab(index);
}

void MainWindow::on_actionNewScript_triggered() {
    //QWidget *currentTab = ui->tabWidget->currentWidget();
    //QTextEdit *edit = ui->tabWidget->findChild<QTextEdit*>();
    ui->tabWidget->addTab(new QTextEdit, "new");
}
