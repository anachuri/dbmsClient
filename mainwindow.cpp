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

void MainWindow::on_actionCopy_triggered() {
    ui->textEdit->copy();
}

void MainWindow::on_actionCut_triggered() {
    ui->textEdit->cut();
}

void MainWindow::on_actionPaste_triggered() {
    ui->textEdit->paste();
}

void MainWindow::on_actionExecute_triggered() {}

void MainWindow::on_actionPreferences_triggered() {}

void MainWindow::on_actionManual_triggered() {}

void MainWindow::on_actionAbout_dbmsClient_triggered() {
    // QDialog dialog;
    // dialog.setWindowTitle("About dbmsClient");
    // QHBoxLayout *layout = new QHBoxLayout(&dialog);
    // layout->addWidget(new QLabel("Created by anachuri - January 2026")); //&dialog innecesario
    // dialog.exec();

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
