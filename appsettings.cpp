#include "appsettings.h"

AppSettings::AppSettings()
    : settings(QSettings(ORGNAME,APPNAME)) {
}

void AppSettings::save(Settings appSettings) {
    settings.setValue(FONT,appSettings.font);
}

Settings AppSettings::load() {
    Settings appSettings;
    appSettings.font = settings.value(FONT).value<QFont>();
    return appSettings;
}
