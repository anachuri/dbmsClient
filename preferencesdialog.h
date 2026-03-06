#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include "appsettings.h"

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

private:
    Ui::PreferencesDialog *ui;
    AppSettings appSettings;
};

#endif // PREFERENCESDIALOG_H
