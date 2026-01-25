#include "dbtreewidget.h"
#include <QAction>

DbTreeWidget::DbTreeWidget(QWidget *parent)
    : QTreeWidget(parent) {
    //setHeaderLabel("Databases");
    // headerItem()->setText(0, "Databases");
    // headerItem()->setIcon(0, QIcon(":/img/database.png"));
    newTableAction = new QAction(QIcon(":/img/cells.png"), "New table", this);
    addAction(newTableAction);
}
