#include "settings.h"
#include <QSettings>

Settings::Settings() {}

QVariant Settings::load(const QString &key, const QVariant &defaultValue, const QString &group) {}

void Settings::save(const QString &key, const QVariant &value, const QString &group) {}
