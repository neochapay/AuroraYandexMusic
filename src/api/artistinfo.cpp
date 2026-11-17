/*
 * Copyright (C) 2023 Chupligin Sergey <neochapay@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "artistinfo.h"
#include "request.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QUrlQuery>

ArtistInfo::ArtistInfo(QObject* parent)
    : QObject(parent)
{
}

int ArtistInfo::artistId() const
{
    return m_artistId;
}

void ArtistInfo::setArtistId(int newArtistId)
{
    if (m_artistId == newArtistId)
        return;
    m_artistId = newArtistId;
    emit artistIdChanged();
}

void ArtistInfo::getTrackIdsByRating()
{
    if (m_artistId <= 0) {
        qWarning() << "Wrong artist ID!";
        return;
    }
    Request* request = new Request(QString("/artists/%1/track-ids-by-rating").arg(m_artistId));
    connect(request, &Request::dataReady, this, &ArtistInfo::getTrackIdsByRatingRequestHandler);
    request->get();
}

void ArtistInfo::getBriefInfo()
{
    if (m_artistId <= 0) {
        qWarning() << "Wrong artist ID!";
        return;
    }

    Request* request = new Request(QString("/artists/%1/brief-info").arg(m_artistId));
    connect(request, &Request::dataReady, this, &ArtistInfo::getBriefInfoRequestHandler);
    request->get();
}

void ArtistInfo::getTracks(int page, int pageSize)
{
    if (m_artistId <= 0) {
        qWarning() << "Wrong artist ID!";
        return;
    }

    QUrlQuery query;
    if (page >= 0) query.addQueryItem(QStringLiteral("page"), QString::number(page));
    if (pageSize > 0) query.addQueryItem(QStringLiteral("page-size"), QString::number(pageSize));

    Request* request = new Request(QString("/artists/%1/tracks").arg(m_artistId));
    connect(request, &Request::dataReady, this, &ArtistInfo::getTracksRequestHandler);
    if (query.isEmpty()) request->get(); else request->get(query);
}

void ArtistInfo::getDirectAlbums(const QString &sortBy, int page, int pageSize)
{
    if (m_artistId <= 0) {
        qWarning() << "Wrong artist ID!";
        return;
    }

    QUrlQuery query;
    if (!sortBy.isEmpty()) query.addQueryItem(QStringLiteral("sort-by"), sortBy);
    if (page >= 0) query.addQueryItem(QStringLiteral("page"), QString::number(page));
    if (pageSize > 0) query.addQueryItem(QStringLiteral("page-size"), QString::number(pageSize));

    Request* request = new Request(QString("/artists/%1/direct-albums").arg(m_artistId));
    connect(request, &Request::dataReady, this, &ArtistInfo::getDirectAlbumsRequestHandler);
    if (query.isEmpty()) request->get(); else request->get(query);
}

void ArtistInfo::getTrackIdsByRatingRequestHandler(QJsonValue value)
{
    QJsonObject obj = value.toObject();
    QStringList ids;
    const QJsonArray arr = obj.value(QStringLiteral("tracks")).toArray();
    for (const QJsonValue &v : arr) {
        if (v.isString()) ids << v.toString();
        else if (v.isDouble()) ids << QString::number(v.toInt());
        else if (v.isObject() && v.toObject().contains("id")) ids << v.toObject().value("id").toString();
    }
    emit trackIdsByRatingReady(ids, obj);
}

void ArtistInfo::getBriefInfoRequestHandler(QJsonValue value)
{
    QJsonObject obj = value.toObject();
    ArtistInfoData *aData = new ArtistInfoData();

    if (obj.value(QStringLiteral("artist")).isObject()) {
        Artist* artist = new Artist(obj.value(QStringLiteral("artist")).toObject());
        aData->artist = artist;
    } else {
        aData->artist = nullptr;
    }

    const QJsonArray popularTracks = obj.value(QStringLiteral("popularTracks")).toArray();
    for (const QJsonValue &v : popularTracks) {
        Track *track = new Track(v.toObject());
        if (track && (!track->title().isEmpty() || track->trackId() != 0)) {
            aData->popularTracks.push_back(track);
        } else {
            delete track;
        }
    }

    const QJsonArray albums = obj.value(QStringLiteral("albums")).toArray();
    for (const QJsonValue &v : albums) {
        Album* album = new Album(v.toObject());
        if (album && (!album->title().isEmpty() || album->albumId() != 0)) {
            aData->albums.push_back(album);
        } else {
            delete album;
        }
    }

    const QJsonArray playlists = obj.value(QStringLiteral("playlists")).toArray();
    for (const QJsonValue &v : playlists) {
        Playlist* playlist = new Playlist(v.toObject());
        if (playlist && !playlist->title().isEmpty()) {
            aData->playlists.push_back(playlist);
        } else {
            delete playlist;
        }
    }

    const QJsonArray similar = obj.value(QStringLiteral("similarArtists")).toArray();
    for (const QJsonValue &v : similar) {
        Artist* artist = new Artist(v.toObject());
        aData->similarArtists.push_back(artist);
    }

    emit getBriefInfoArtistInfoReady(aData); // compatibility
    emit briefInfoReady(aData, obj);
}

void ArtistInfo::getTracksRequestHandler(QJsonValue value)
{
    QList<Track*> tracks;
    QJsonObject obj = value.toObject();
    QJsonArray arr;
    if (value.isArray()) {
        arr = value.toArray();
    } else if (obj.value(QStringLiteral("tracks")).isArray()) {
        arr = obj.value(QStringLiteral("tracks")).toArray();
    } else if (obj.value(QStringLiteral("tracks")).isObject() && obj.value(QStringLiteral("tracks")).toObject().value(QStringLiteral("items")).isArray()) {
        arr = obj.value(QStringLiteral("tracks")).toObject().value(QStringLiteral("items")).toArray();
    } else if (obj.value(QStringLiteral("results")).isArray()) {
        arr = obj.value(QStringLiteral("results")).toArray();
    }

    for (const QJsonValue &v : arr) {
        QJsonObject t = v.toObject();
        if (t.contains("track") && t.value("track").isObject()) t = t.value("track").toObject();
        Track *track = new Track(t);
        if (track && (track->trackId() != 0 || !track->title().isEmpty())) {
            tracks.push_back(track);
        } else {
            delete track;
        }
    }
    emit tracksReady(tracks, obj);
}

void ArtistInfo::getDirectAlbumsRequestHandler(QJsonValue value)
{
    QList<Album*> albums;
    QJsonObject obj = value.toObject();
    QJsonArray arr;
    if (value.isArray()) {
        arr = value.toArray();
    } else if (obj.value(QStringLiteral("albums")).isArray()) {
        arr = obj.value(QStringLiteral("albums")).toArray();
    } else if (obj.value(QStringLiteral("directAlbums")).isArray()) {
        arr = obj.value(QStringLiteral("directAlbums")).toArray();
    } else if (obj.value(QStringLiteral("albums")).isObject() && obj.value(QStringLiteral("albums")).toObject().value(QStringLiteral("items")).isArray()) {
        arr = obj.value(QStringLiteral("albums")).toObject().value(QStringLiteral("items")).toArray();
    } else if (obj.value(QStringLiteral("results")).isArray()) {
        arr = obj.value(QStringLiteral("results")).toArray();
    }

    for (const QJsonValue &v : arr) {
        QJsonObject a = v.toObject();
        if (a.contains("album") && a.value("album").isObject()) a = a.value("album").toObject();
        Album* album = new Album(a);
        if (album && (album->albumId() != 0 || !album->title().isEmpty())) {
            albums.push_back(album);
        } else {
            delete album;
        }
    }
    emit directAlbumsReady(albums, obj);
}

