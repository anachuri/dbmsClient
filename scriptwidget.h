#ifndef SCRIPTWIDGET_H
#define SCRIPTWIDGET_H

#include <QWidget>

namespace Ui {
class ScriptWidget;
}

class ScriptWidget : public QWidget {
    Q_OBJECT

public:
    explicit ScriptWidget(QWidget *parent = nullptr);
    ScriptWidget(QWidget *parent,const QString &filePath);
    ~ScriptWidget();
    void loadScript(const QString &content);
    bool isFilePathEmpty();
    bool isTextEditEmpty();
    QString getFilePath() const { return filePath; }
    QString getScriptText() const;
    bool isEdited()const {return edited;}
    //bool isEmptyClosableScript(){return !edited && filePath.isEmpty();}
    bool isClosable(){return !edited && filePath.isEmpty();}
private slots:
    void on_textEdit_textChanged();

private:
    Ui::ScriptWidget *ui;
    const QString filePath;
    bool edited = false;
};

#endif // SCRIPTWIDGET_H
