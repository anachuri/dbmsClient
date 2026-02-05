#ifndef SCRIPTWIDGET_H
#define SCRIPTWIDGET_H

#include <QWidget>
#include "findreplacedialog.h"

namespace Ui {
class ScriptWidget;
}

class QPrinter;

enum class ScriptState {
    New,
    Clean,
    Modified,
    ReadOnly
};

class ScriptWidget : public QWidget {
    Q_OBJECT

public:
    //explicit ScriptWidget(QWidget *parent = nullptr);
    ScriptWidget(QWidget *parent, const QString &filePath, const QString &fileName);
    ~ScriptWidget();
    void loadScript(const QString &content);
    bool isFilePathEmpty();
    QString const &getFilePath() const { return filePath; }
    QString const &getFileName() const { return fileName; }
    QString getScriptText() const;
    void setFilePath(const QString &filePath) {
        this->filePath = filePath;
        state = ScriptState::Clean;
    }
    void setFindReplaceDialog(FindReplaceDialog &dialog);
    void setClean();
    ScriptState getState(){ return state;}
    void copyText();
    void cutText();
    void pasteText();
    void print(QPrinter *printer);
private slots:
    void on_textEdit_textChanged();

private:
    Ui::ScriptWidget *ui;
    QString filePath;
    ScriptState state;
    QString fileName;
};

#endif // SCRIPTWIDGET_H
