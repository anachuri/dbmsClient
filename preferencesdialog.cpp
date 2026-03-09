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
    on_applyButton_clicked();
    saveSettings();
    accept();
}

void PreferencesDialog::on_cancelButton_clicked() {
    emit settingsChanged(settings);
    reject();
}

void PreferencesDialog::on_applyButton_clicked() {
    QFont font = ui->fontComboBox->currentFont();
    font.setPointSize(ui->spinBox->value());
    Settings settings(font);
    emit settingsChanged(settings);
}

void PreferencesDialog::saveSettings() {
    this->qSettings.setValue(FONT, settings.font);
}
