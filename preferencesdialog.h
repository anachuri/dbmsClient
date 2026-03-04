#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QFont>
#include "defines.h"

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog {
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = nullptr,Settings &settings);
    ~PreferencesDialog();
signals:
    void fontChanged(QFont);
    void fontSizeChanged(int);
private slots:
    void on_acceptButton_clicked();
    void on_cancelButton_clicked();
    void on_applyButton_clicked();

private:
    Ui::PreferencesDialog *ui;
    Settings settings;
};

#endif // PREFERENCESDIALOG_H
