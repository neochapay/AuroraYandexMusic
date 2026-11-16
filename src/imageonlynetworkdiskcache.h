#ifndef IMAGEONLYNETWORKDISKCACHE_H
#define IMAGEONLYNETWORKDISKCACHE_H

#include <QNetworkDiskCache>

class ImageOnlyNetworkDiskCache : public QNetworkDiskCache
{
public:
    explicit ImageOnlyNetworkDiskCache(QObject *parent = nullptr);

    QIODevice *prepare(const QNetworkCacheMetaData &metaData) override;
};

#endif // IMAGEONLYNETWORKDISKCACHE_H
