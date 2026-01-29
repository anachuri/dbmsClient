#include "scriptwidget.h"
#include <QFileInfo>
#include <QTabWidget>
#include "ui_scriptwidget.h"

ScriptWidget::ScriptWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ScriptWidget)
    , filePath("") {
    ui->setupUi(this);
}

ScriptWidget::ScriptWidget(QTabWidget *parent, const QString &filePath)
    : QWidget(parent)
    , ui(new Ui::ScriptWidget)
    , filePath(filePath)
    , tabWidget(parent) {
    ui->setupUi(this);
}

// ScriptWidget::ScriptWidget(QWidget *parent,const QString &filePath):
//     QWidget(parent), ui(new Ui::ScriptWidget),filePath(filePath){
//     ui->setupUi(this);
// }

ScriptWidget::~ScriptWidget() {
    delete ui;
}

void ScriptWidget::loadScript(const QString &content){
    ui->textEdit->setPlainText(content);
    edited = false;
}

bool ScriptWidget::isFilePathEmpty() {
    return filePath.isEmpty();
}

QString ScriptWidget::getScriptText() const {
    return ui->textEdit->toPlainText();
}

void ScriptWidget::copyText(){
    ui->textEdit->copy();
}

void ScriptWidget::cutText(){
    ui->textEdit->cut();
}

void ScriptWidget::pasteText(){
    ui->textEdit->paste();
}

void ScriptWidget::on_textEdit_textChanged(){
    edited = true;
    if (isFilePathEmpty()) {
        tabWidget->setTabText(tabWidget->currentIndex(),
                              QString("Script %0*").arg(tabWidget->count()));
        return;
    }
    tabWidget->setTabText(tabWidget->currentIndex(), QFileInfo(filePath).fileName().append("*"));
}
