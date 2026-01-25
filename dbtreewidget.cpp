#include "dbtreewidget.h"

DbTreeWidget::DbTreeWidget(QWidget *parent)
    : QTreeWidget(parent) {
    //setHeaderLabel("Databases");
    headerItem()->setText(0, "Databases");
    headerItem()->setIcon(0, QIcon(":/img/database.png"));
}
