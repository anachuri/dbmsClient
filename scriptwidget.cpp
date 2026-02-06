#include "scriptwidget.h"
#include <QFileInfo>
#include <QPrinter>
#include <QTabWidget>
#include "ui_scriptwidget.h"

ScriptWidget::ScriptWidget(QWidget *parent, const QString &filePath, const QString &fileName)
    : QWidget(parent)
    , ui(new Ui::ScriptWidget)
    , filePath(filePath)
    , fileName(fileName)
    , state(ScriptState::New) {
    ui->setupUi(this);
}

ScriptWidget::~ScriptWidget(){
    delete ui;
}

void ScriptWidget::loadScript(const QString &content){
    ui->textEdit->setPlainText(content);
}

bool ScriptWidget::isFilePathEmpty() {
    return filePath.isEmpty();
}

QString ScriptWidget::getScriptText() const {
    return ui->textEdit->toPlainText();
}

void ScriptWidget::setFindReplaceDialog(FindReplaceDialog &dialog) {
    dialog.setTextEdit(ui->textEdit);
}

void ScriptWidget::setClean(){
    if (state == ScriptState::Clean || state == ScriptState::New)
        return;
    if (fileName.endsWith("*"))
        fileName = fileName.removeAt(fileName.length() - 1);
    QTabWidget *tabWidget = nullptr;
    QWidget *p = parentWidget();
    while (p && !(tabWidget = qobject_cast<QTabWidget *>(p))) {
        p = p->parentWidget();
    }
    if (!tabWidget)
        return;
    tabWidget->setTabText(tabWidget->currentIndex(), fileName);
    state = ScriptState::Clean;
}

void ScriptWidget::on_textEdit_textChanged() {
    if (state == ScriptState::New && !filePath.isEmpty()) {
        state = ScriptState::Clean;
        return;
    }
    state = ScriptState::Modified;
    QTabWidget *tabWidget = nullptr;
    QWidget *p = parentWidget();
    while (p && !(tabWidget = qobject_cast<QTabWidget *>(p))) {
        p = p->parentWidget();
    }
    if (!tabWidget || fileName.endsWith("*"))
        return;
    fileName.append("*");
    tabWidget->setTabText(tabWidget->currentIndex(), fileName);
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

void ScriptWidget::print(QPrinter *printer) {
    ui->textEdit->print(printer);
}
