#include "settings.h"

Settings::Settings(QObject* parent)
    : QSettings(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/org.ilyavysotsky/yasailmusic/yasailmusic.conf", QSettings::NativeFormat,
        parent)
{
}
