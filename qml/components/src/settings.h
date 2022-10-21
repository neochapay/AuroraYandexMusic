#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QStandardPaths>

class Settings : public QSettings {
    Q_OBJECT
public:
    explicit Settings(QObject* parent = nullptr);
};

#endif // SETTINGS_H
