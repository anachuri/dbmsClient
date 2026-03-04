#include "mainwindow.h"
#include <QApplication>
#include "defines.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setApplicationName(APPNAME);
    a.setOrganizationName(ORGNAME);
    a.setApplicationVersion(APPVERSION);
    MainWindow w;
    w.show();
    return a.exec();
}
