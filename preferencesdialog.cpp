#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "defines.h"
#include <QSettings>

PreferencesDialog::PreferencesDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PreferencesDialog) {
    ui->setupUi(this);
    QSettings settings(ORGNAME,APPNAME);
    currentSettings.font = settings.value(FONT).value<QFont>();
}

PreferencesDialog::~PreferencesDialog() {
    delete ui;
}

void PreferencesDialog::on_acceptButton_clicked(){
    QFont font = ui->fontComboBox->currentFont();
    font.setPointSize(ui->spinBox->value());
    QSettings settings(ORGNAME,APPNAME);
    settings.setValue(FONT,font);
    currentSettings.font = font;
    emit settingsChanged(currentSettings);
    accept();
}

void PreferencesDialog::on_cancelButton_clicked() {
    emit settingsChanged(currentSettings);
    close();
}

void PreferencesDialog::on_applyButton_clicked() {
    QFont font = ui->fontComboBox->currentFont();
    font.setPointSize(ui->spinBox->value());
    Settings settings;
    settings.font = font;
    emit settingsChanged(settings);
}
