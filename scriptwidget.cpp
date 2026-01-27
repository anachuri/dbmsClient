#include "scriptwidget.h"
#include "ui_scriptwidget.h"

ScriptWidget::ScriptWidget(QWidget *parent,QString *filePath):
    QWidget(parent), ui(new Ui::ScriptWidget),filePath(filePath){
    ui->setupUi(this);
}


ScriptWidget::~ScriptWidget() {
    delete ui;
}

void ScriptWidget::loadScript(QString content){
    ui->textEdit->setPlainText(content);
}
