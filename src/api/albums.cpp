#include "albums.h"
#include "request.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QUrlQuery>

Albums::Albums(QObject *parent) : QObject(parent)
{
}

void Albums::getAlbums(const QStringList &albumIds)
{
    if (albumIds.isEmpty()) {
        return;
    }

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("album-ids"), albumIds.join(','));

    Request* request = new Request("/albums");
    connect(request, &Request::dataReady, this, &Albums::albumsRequestHandler);
    request->post(query.toString(QUrl::FullyEncoded));
}

void Albums::getAlbumsByIds(const QString &albumIdsCsv)
{
    if (albumIdsCsv.trimmed().isEmpty()) {
        return;
    }
    getAlbums(albumIdsCsv.split(',', QString::SkipEmptyParts));
}

void Albums::getAlbum(int albumId, bool withTracks)
{
    if (albumId <= 0) {
        return;
    }

    QString path = QString("/albums/%1").arg(albumId);
    if (withTracks) {
        path += "/with-tracks";
    }

    Request* request = new Request(path);
    connect(request, &Request::dataReady, this, &Albums::albumRequestHandler);
    request->get();
}

void Albums::getAlbumWithTracks(int albumId)
{
    getAlbum(albumId, true);
}

void Albums::albumsRequestHandler(QJsonValue value)
{
    QList<Album*> albums;
    QJsonArray arr;
    if (value.isArray()) {
        arr = value.toArray();
    } else if (value.isObject() && value.toObject().contains("albums")) {
        arr = value.toObject().value("albums").toArray();
    }

    for (const QJsonValue &v : arr) {
        if (!v.isObject()) continue;
        Album* album = new Album(v.toObject());
        if (album && album->albumId() != 0) {
            albums.push_back(album);
        } else {
            delete album;
        }
    }

    emit albumsReady(albums);
}

void Albums::albumRequestHandler(QJsonValue value)
{
    if (!value.isObject()) {
        emit error(QStringLiteral("Invalid album response"));
        return;
    }
    Album* album = new Album(value.toObject());
    if (album && album->albumId() != 0) {
        emit albumReady(album);
    } else {
        delete album;
        emit error(QStringLiteral("Invalid album"));
    }
}
