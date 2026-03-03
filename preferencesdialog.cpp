#include "preferencesdialog.h"
#include "settings.h"
#include "ui_preferencesdialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PreferencesDialog) {
    ui->setupUi(this);
}

PreferencesDialog::~PreferencesDialog() {
    delete ui;
}

void PreferencesDialog::on_acceptButton_clicked() {
    Settings settings;
    QFont font = ui->fontComboBox->currentFont();
    font.setPointSize(ui->spinBox->value());
    settings.save(font);
    accept();
}

void PreferencesDialog::on_cancelButton_clicked() {
    close();
}

void PreferencesDialog::on_applyButton_clicked() {
    QFont font = ui->fontComboBox->currentFont();
    font.setPointSize(ui->spinBox->value());
    emit fontChanged(font);
}
