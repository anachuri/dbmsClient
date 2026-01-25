#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "dbtreewidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowIcon(QIcon(":/img/database.png"));
    DbTreeWidget *treeWidget = new DbTreeWidget(this);
    ui->splitter->insertWidget(0, treeWidget);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionOpen_triggered(){

}

void MainWindow::on_actionNew_table_triggered()
{

}

