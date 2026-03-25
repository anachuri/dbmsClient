#include "newtabledialog.h"
#include <QCheckBox>
#include <QLineEdit>
#include <QRadioButton>
#include "ui_newtabledialog.h"

NewTableDialog::NewTableDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewTableDialog) {
    ui->setupUi(this);
    QAction *addColumn = ui->tableWidget->addAction("Add Column");
    QAction *removeColumn = ui->tableWidget->addAction("Remove Column");
    connect(addColumn, &QAction::triggered, this, &NewTableDialog::onAddColumnTriggered);
    connect(removeColumn, &QAction::triggered, [&]() {
        ui->tableWidget->removeRow(ui->tableWidget->currentRow());
    });
    ui->tableWidget->setColumnWidth(4, 150);
}

NewTableDialog::~NewTableDialog() {
    delete ui;
}

void NewTableDialog::on_acceptButton_clicked() {
    QString sql = QString("CREATE TABLE %1(").arg(ui->lineEdit->text());
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        QString name = static_cast<QLineEdit *>(ui->tableWidget->cellWidget(i, 0))->text();
        QString type = static_cast<QComboBox *>(ui->tableWidget->cellWidget(i, 1))->currentText();
        bool pk = static_cast<QRadioButton *>(ui->tableWidget->cellWidget(i, 2))->isChecked();
        bool notNull = static_cast<QCheckBox *>(ui->tableWidget->cellWidget(i, 3))->isChecked();
        bool autoIncrement = static_cast<QCheckBox *>(ui->tableWidget->cellWidget(i, 4))->isChecked();
        sql += name + " " + type;
        if (pk)
            sql += " PRIMARY KEY";
        if (autoIncrement)
            sql += " AUTOINCREMENT";
        if (notNull)
            sql += " NOT NULL";
        sql += ",";
    }
    sql[sql.size() - 1] = ')';
    emit scriptGenerated(sql, ui->lineEdit->text());
    accept();
}

void NewTableDialog::on_cancelButton_clicked() {
    reject();
}

void NewTableDialog::onAddColumnTriggered() {
    int rows = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rows);
    ui->tableWidget->setCellWidget(rows, 0, new QLineEdit(this));
    ui->tableWidget->setCellWidget(rows, 1, generateComboBox());
    ui->tableWidget->setCellWidget(rows, 2, new QRadioButton(this));
    ui->tableWidget->setCellWidget(rows, 3, new QCheckBox(this));
    ui->tableWidget->setCellWidget(rows, 4, new QCheckBox(this));
    ui->tableWidget->cellWidget(ui->tableWidget->rowCount() - 1, 0)->setFocus();
}

QComboBox *NewTableDialog::generateComboBox() {
    QComboBox *comboBox = new QComboBox(this);
    comboBox->addItems({"INTEGER", "DOUBLE", "TEXT", "DATE", "TIME", "BOOL"});
    return comboBox;
}
