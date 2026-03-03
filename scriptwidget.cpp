#include "scriptwidget.h"
#include <QAction>
#include <QFileInfo>
#include <QFont>

#include <QPrinter>
#include <QSettings>
#include <QTabWidget>
#include "ui_scriptwidget.h"

ScriptWidget::ScriptWidget(QWidget *parent,
                           const QString &filePath,
                           const QString &fileName,
                           QAction *actionCopy,
                           QAction *actionCut,
                           QAction *actionPaste)
    : QWidget(parent)
    , ui(new Ui::ScriptWidget)
    , filePath(filePath)
    , fileName(fileName)
    , state(ScriptState::New) {
    ui->setupUi(this);
    connect(actionCopy, &QAction::triggered, ui->textEdit, &QTextEdit::copy);
    connect(actionCut, &QAction::triggered, ui->textEdit, &QTextEdit::cut);
    connect(actionPaste, &QAction::triggered, ui->textEdit, &QTextEdit::paste);
    ui->textEdit->setFont(settings.load("font","").value<QFont>());
}

ScriptWidget::~ScriptWidget() {
    delete ui;
}

void ScriptWidget::loadScript(const QString &content) {
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

void ScriptWidget::setClean() {
    state = ScriptState::Clean;
}

void ScriptWidget::on_textEdit_textChanged() {
    if (state == ScriptState::New && !filePath.isEmpty()) {
        state = ScriptState::Clean;
        return;
    }
    state = ScriptState::Modified;
    emit contentChanged(fileName);
}

void ScriptWidget::onFontChanged(const QFont &font) {
    ui->textEdit->setFont(font);
}
void ScriptWidget::print(QPrinter *printer) {
    ui->textEdit->print(printer);
}
