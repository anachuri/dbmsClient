#ifndef NEWTABLEDIALOG_H
#define NEWTABLEDIALOG_H

#include <QDialog>

namespace Ui {
class NewTableDialog;
}

class NewTableDialog : public QDialog {
    Q_OBJECT

public:
    explicit NewTableDialog(QWidget *parent = nullptr);
    ~NewTableDialog();

private slots:
    void on_acceptButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::NewTableDialog *ui;
};

#endif // NEWTABLEDIALOG_H
