#include "dbtreewidget.h"
#include <QAction>

DbTreeWidget::DbTreeWidget(QWidget *parent)
    : QTreeWidget(parent) {
    newTableAction = new QAction(QIcon(":/img/cells.png"), "New table", this);
    addAction(newTableAction);
}
