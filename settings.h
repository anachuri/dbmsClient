#ifndef SETTINGS_H
#define SETTINGS_H

#include <QFont>
#include <QObject>

class Settings {
public:
    Settings();
    QVariant load(const QString &key, const QVariant &defaultValue, const QString &group);
    void save(const QString &key, const QVariant &value, const QString &group);

private:
    QString file;
};

#endif // SETTINGS_H
