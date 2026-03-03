#ifndef SETTINGS_H
#define SETTINGS_H

#include <QFont>
#include <QObject>
#include <QSettings>

class Settings {
public:
    Settings();
    QVariant load(const QString &key, const QString &group);
    void save(const QString &key, const QVariant &value, const QString &group);

private:
    QSettings settings;
};

#endif // SETTINGS_H
