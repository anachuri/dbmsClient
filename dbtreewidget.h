#ifndef DBTREEWIDGET_H
#define DBTREEWIDGET_H

#include <QTreeWidget>
#include <QWidget>

class DbTreeWidget : public QTreeWidget {
public:
    explicit DbTreeWidget(QWidget *parent = nullptr);
};

#endif // DBTREEWIDGET_H
