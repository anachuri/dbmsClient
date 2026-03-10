#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QSettings>
#include "defines.h"

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog {
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = nullptr);
    ~PreferencesDialog();
signals:
    void settingsChanged(Settings);
private slots:
    void on_acceptButton_clicked();
    void on_cancelButton_clicked();
    void on_applyButton_clicked();

public:
    void saveSettings(const Settings &settings);
    Settings getSettings();

private:
    Ui::PreferencesDialog *ui;
    QSettings qSettings;
    Settings settings;
};

#endif // PREFERENCESDIALOG_H
