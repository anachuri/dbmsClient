#include "newtabledialog.h"
#include "ui_newtabledialog.h"

NewTableDialog::NewTableDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewTableDialog) {
    ui->setupUi(this);
}

NewTableDialog::~NewTableDialog() {
    delete ui;
}

void NewTableDialog::on_acceptButton_clicked() {}

void NewTableDialog::on_cancelButton_clicked() {}
