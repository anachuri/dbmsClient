#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "defines.h"
#include <QSettings>

PreferencesDialog::PreferencesDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PreferencesDialog) {
    ui->setupUi(this);
    QSettings settings(ORGNAME,APPNAME);
    QFont font = settings.value(FONT).value<QFont>();;
    ui->fontComboBox->setCurrentFont(font);
    ui->spinBox->setValue(font.pointSize());
}

PreferencesDialog::~PreferencesDialog() {
    delete ui;
}

void PreferencesDialog::on_acceptButton_clicked(){
    QSettings settings(ORGNAME,APPNAME);
    Settings currentSettings;
    QFont font = ui->fontComboBox->currentFont();
    font.setPointSize(ui->spinBox->value());
    settings.setValue(FONT,font);
    currentSettings.font = font;
    emit settingsChanged(currentSettings);
    accept();
}

void PreferencesDialog::on_cancelButton_clicked() {
    QSettings settings(ORGNAME,APPNAME);
    Settings currentSettings;
    currentSettings.font = settings.value(FONT).value<QFont>();
    emit settingsChanged(currentSettings);
    reject();
}

void PreferencesDialog::on_applyButton_clicked() {
    QFont font = ui->fontComboBox->currentFont();
    font.setPointSize(ui->spinBox->value());
    Settings settings;
    settings.font = font;
    emit settingsChanged(settings);
}
