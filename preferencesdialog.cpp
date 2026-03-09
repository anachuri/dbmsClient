#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent)
    : QDialog(parent)
    , qSettings(ORGNAME, APPNAME)
    , settings(qSettings.value(FONT).value<QFont>())
    , ui(new Ui::PreferencesDialog) {
    ui->setupUi(this);
    ui->fontComboBox->setCurrentFont(settings.font);
    ui->spinBox->setValue(settings.font.pointSize());
}

PreferencesDialog::~PreferencesDialog() {
    delete ui;
}

void PreferencesDialog::on_acceptButton_clicked(){
    Settings settings = getSettings();
    emit settingsChanged(settings);
    saveSettings(settings);
    accept();
}

void PreferencesDialog::on_cancelButton_clicked() {
    emit settingsChanged(settings);
    saveSettings(settings);
    reject();
}

void PreferencesDialog::on_applyButton_clicked() {
    emit settingsChanged(getSettings());
}

Settings PreferencesDialog::getSettings() {
    QFont font = ui->fontComboBox->currentFont();
    font.setPointSize(ui->spinBox->value());
    return Settings(font);
}

void PreferencesDialog::saveSettings(Settings settings) {
    this->qSettings.setValue(FONT, settings.font);
}
