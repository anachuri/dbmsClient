#ifndef SCRIPTWIDGET_H
#define SCRIPTWIDGET_H

#include <QWidget>

namespace Ui {
class ScriptWidget;
}

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
    ScriptWidget(QWidget *parent,const QString &filePath);
    ~ScriptWidget();
    void loadScript(const QString &content);
    bool isFilePathEmpty();
    QString getFilePath() const { return filePath; }
    QString getScriptText() const;
    void setFilePath(QString filePath) {
        this->filePath = filePath;
        state = ScriptState::Clean;
    }
    void setClean();
    ScriptState getState(){ return state;}
    void copyText();
    void cutText();
    void pasteText();
private slots:
    void on_textEdit_textChanged();

private:
    Ui::ScriptWidget *ui;
    QString filePath;
    ScriptState state;
};

#endif // SCRIPTWIDGET_H
