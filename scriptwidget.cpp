#include "scriptwidget.h"
#include <QFileInfo>
#include <QTabWidget>
#include "ui_scriptwidget.h"
#include <QDebug>

// ScriptWidget::ScriptWidget(QWidget *parent)
//     : QWidget(parent)
//     , ui(new Ui::ScriptWidget)
//     , filePath("") {
//     ui->setupUi(this);
// }

// ScriptWidget::ScriptWidget(QTabWidget *parent, const QString &filePath)
//     : QWidget(parent)
//     , ui(new Ui::ScriptWidget)
//     , filePath(filePath)
//     , tabWidget(parent) {
//     ui->setupUi(this);
// }

ScriptWidget::ScriptWidget(QWidget *parent,const QString &filePath):
    QWidget(parent), ui(new Ui::ScriptWidget),filePath(filePath),state(ScriptState::New){
    ui->setupUi(this);
}

ScriptWidget::~ScriptWidget(){
    delete ui;
}

void ScriptWidget::loadScript(const QString &content){
    ui->textEdit->setPlainText(content);
    state = ScriptState::Clean;
}

bool ScriptWidget::isFilePathEmpty() {
    return filePath.isEmpty();
}

QString ScriptWidget::getScriptText() const {
    return ui->textEdit->toPlainText();
}

void ScriptWidget::setClean(){
    state = ScriptState::Clean;
    QTabWidget *tabWidget = nullptr;
    QWidget *p = parentWidget();
    while (p && !(tabWidget = qobject_cast<QTabWidget *>(p))) {
        p = p->parentWidget();
    }
    if (!tabWidget)
        return;
    QString fileName = QFileInfo(filePath).fileName();
    if(fileName.isEmpty()){
        tabWidget->setTabText(tabWidget->currentIndex(),
                              QString("Script %0").arg(tabWidget->currentIndex()+1));
        return;
    }
    tabWidget->setTabText(tabWidget->currentIndex(),fileName);
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
    if(state == ScriptState::New && !filePath.isEmpty()){
        state = ScriptState::Clean;
        return;
    }
    state = ScriptState::Modified;
    QTabWidget *tabWidget = nullptr;
    QWidget *p = parentWidget();
    while (p && !(tabWidget = qobject_cast<QTabWidget *>(p))) {
        p = p->parentWidget();
    }
    if (!tabWidget)
        return;
    QString fileName = QFileInfo(filePath).fileName();
    if(fileName.isEmpty()){
        tabWidget->setTabText(tabWidget->currentIndex(),
                              QString("Script %0*").arg(tabWidget->currentIndex()+1));
        return;
    }
    if (fileName.endsWith("*"))
        return;
    fileName.append("*");
    tabWidget->setTabText(tabWidget->currentIndex(),fileName);
}
