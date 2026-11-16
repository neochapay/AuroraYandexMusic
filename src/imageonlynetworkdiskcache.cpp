#include "imageonlynetworkdiskcache.h"

#include <QUrl>

ImageOnlyNetworkDiskCache::ImageOnlyNetworkDiskCache(QObject *parent)
    : QNetworkDiskCache(parent)
{
}

QIODevice *ImageOnlyNetworkDiskCache::prepare(const QNetworkCacheMetaData &metaData)
{
    const QUrl url = metaData.url();
    const QString host = url.host();
    const QString path = url.path().toLower();

    if (host == QLatin1String("api.music.yandex.net")) {
        return nullptr;
    }

    if (path.endsWith(QLatin1String(".json")) || path.endsWith(QLatin1String(".xml"))) {
        return nullptr;
    }

    return QNetworkDiskCache::prepare(metaData);
}
