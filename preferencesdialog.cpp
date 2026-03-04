#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include <QSettings>

PreferencesDialog::PreferencesDialog(QWidget *parent,Settings &settings)
    : QDialog(parent)
    , ui(new Ui::PreferencesDialog) {
    ui->setupUi(this);
}

PreferencesDialog::~PreferencesDialog() {
    delete ui;
}

void PreferencesDialog::on_acceptButton_clicked(){
    QFont font = ui->fontComboBox->currentFont();
    font.setPointSize(ui->spinBox->value());
    QSettings settings(ORGNAME,APPNAME);
    settings.setValue(FONT,font);
    currentFont = font;
    accept();
}

void PreferencesDialog::on_cancelButton_clicked() {
    emit fontChanged(currentFont);
    close();
}

void PreferencesDialog::on_applyButton_clicked() {
    QFont font = ui->fontComboBox->currentFont();
    font.setPointSize(ui->spinBox->value());
    emit fontChanged(font);
}
