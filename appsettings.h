#ifndef APPSETTINGS_H
#define APPSETTINGS_H
#include "defines.h"
#include <QSettings>

class AppSettings {
public:
    AppSettings();
    void save(Settings appSettings);
    Settings load();
private:
    QSettings settings;
};

#endif // APPSETTINGS_H
