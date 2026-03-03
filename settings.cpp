#include "settings.h"
#include <QSettings>

Settings::Settings() {}

QVariant Settings::load(const QString &key, const QString &group) {
    return settings.value(key);
}

void Settings::save(const QString &key, const QVariant &value, const QString &group) {
    settings.setValue(key,value);
}
