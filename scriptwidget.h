#ifndef SCRIPTWIDGET_H
#define SCRIPTWIDGET_H

#include <QWidget>

namespace Ui {
class ScriptWidget;
}

class ScriptWidget : public QWidget {
    Q_OBJECT

public:
    //explicit ScriptWidget(QWidget *parent = nullptr);
    ScriptWidget(QWidget *parent = nullptr,QString *filePath = nullptr);
    ~ScriptWidget();
    void loadScript(QString content);
private:
    Ui::ScriptWidget *ui;
    QString *filePath;
};

#endif // SCRIPTWIDGET_H
