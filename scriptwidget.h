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

private:
    Ui::ScriptWidget *ui;
    const QString filePath;
};

#endif // SCRIPTWIDGET_H
