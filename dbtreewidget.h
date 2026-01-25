#ifndef DBTREEWIDGET_H
#define DBTREEWIDGET_H

#include <QTreeWidget>

class QAction;

class DbTreeWidget : public QTreeWidget {
public:
    explicit DbTreeWidget(QWidget *parent = nullptr);

private:
    QAction *newTableAction;
};

#endif // DBTREEWIDGET_H
