#ifndef ALBUMS_H
#define ALBUMS_H

#include <QObject>
#include <QStringList>

#include "../types/album.h"

class Albums : public QObject
{
    Q_OBJECT
public:
    explicit Albums(QObject *parent = nullptr);

    Q_INVOKABLE void getAlbums(const QStringList &albumIds);
    Q_INVOKABLE void getAlbumsByIds(const QString &albumIdsCsv);
    Q_INVOKABLE void getAlbum(int albumId, bool withTracks = true);
    Q_INVOKABLE void getAlbumWithTracks(int albumId);

signals:
    void albumsReady(const QList<Album*> &albums);
    void albumReady(Album* album);
    void error(const QString &message);

private slots:
    void albumsRequestHandler(QJsonValue value);
    void albumRequestHandler(QJsonValue value);
};

#endif // ALBUMS_H
