#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "appsettings.h"

PreferencesDialog::PreferencesDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PreferencesDialog) {
    ui->setupUi(this);
    Settings settings = appSettings.load();
    ui->fontComboBox->setCurrentFont(settings.font);
    ui->spinBox->setValue(settings.font.pointSize());
}

PreferencesDialog::~PreferencesDialog() {
    delete ui;
}

void PreferencesDialog::on_acceptButton_clicked(){
    QFont font = ui->fontComboBox->currentFont();
    font.setPointSize(ui->spinBox->value());
    Settings settings;
    settings.font = font;
    emit settingsChanged(settings);
    appSettings.save(settings);
    accept();
}

void PreferencesDialog::on_cancelButton_clicked() {
    emit settingsChanged(appSettings.load());
    reject();
}

void PreferencesDialog::on_applyButton_clicked() {
    QFont font = ui->fontComboBox->currentFont();
    font.setPointSize(ui->spinBox->value());
    Settings settings;
    settings.font = font;
    emit settingsChanged(settings);
}
