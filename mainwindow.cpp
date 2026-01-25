#include "mainwindow.h"
#include <QBoxLayout>
#include <QDialog>
#include <QLabel>
#include "./ui_mainwindow.h"
//#include "dbtreewidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    // setWindowIcon(QIcon(":/img/database.png"));
    // DbTreeWidget *treeWidget = new DbTreeWidget(this);
    // ui->splitter->insertWidget(0, treeWidget);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionOpen_triggered() {}

void MainWindow::on_actionSave_triggered() {}

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
    QDialog dialog;
    dialog.setWindowTitle("About dbmsClient");
    QHBoxLayout *layout = new QHBoxLayout(&dialog);
    layout->addWidget(new QLabel("Created by anachuri - January 2026", &dialog));
    dialog.exec();
}

void MainWindow::on_actionFind_and_replace_triggered() {}
