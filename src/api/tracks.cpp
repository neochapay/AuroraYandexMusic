/*
 * Copyright (C) 2024 Chupligin Sergey <neochapay@gmail.com>
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

#include "tracks.h"
#include "request.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QUrlQuery>

Tracks::Tracks(QObject *parent) : QObject(parent)
{
}

void Tracks::getTracksInfo(QList<QString> ids)
{
    QStringList list;
    for (const QString &id : ids) {
        list << id;
    }
    getTracksInfo(list, false);
}

void Tracks::getTracksInfo(const QStringList &ids, bool withPositions)
{
    if (ids.isEmpty()) {
        return;
    }

    QUrlQuery query;
    // send track-ids as comma-separated string (compatible) and also individual items if needed by server
    query.addQueryItem(QStringLiteral("track-ids"), ids.join(','));
    if (withPositions) {
        query.addQueryItem(QStringLiteral("with-positions"), QStringLiteral("true"));
    }

    Request* request = new Request("/tracks/");
    connect(request, &Request::dataReady, this, &Tracks::getTracksInfoHandler);
    request->post(query.toString(QUrl::FullyEncoded));
}

void Tracks::getDownloadInfo(const QString &trackId)
{
    if (trackId.isEmpty()) {
        return;
    }
    m_trackId = trackId;
    Request* request = new Request("/tracks/" + trackId + "/download-info");
    connect(request, &Request::dataReady, this, &Tracks::getDownloadInfoHandler);
    request->get();
}

void Tracks::getSupplement(const QString &trackId)
{
    if (trackId.isEmpty()) {
        return;
    }
    m_trackId = trackId;
    Request* request = new Request("/tracks/" + trackId + "/supplement");
    connect(request, &Request::dataReady, this, &Tracks::getSupplementHandler);
    request->get();
}

void Tracks::getSimilarTracks(const QString &trackId)
{
    if (trackId.isEmpty()) {
        return;
    }
    m_trackId = trackId;
    Request* request = new Request("/tracks/" + trackId + "/similar");
    connect(request, &Request::dataReady, this, &Tracks::getSimilarTracksHandler);
    request->get();
}

void Tracks::getLyrics(const QString &trackId, const QString &format, const QString &timeStamp, const QString &sign)
{
    if (trackId.isEmpty()) {
        return;
    }
    m_trackId = trackId;
    QUrlQuery query;
    if (!format.isEmpty()) query.addQueryItem(QStringLiteral("format"), format);
    if (!timeStamp.isEmpty()) query.addQueryItem(QStringLiteral("timeStamp"), timeStamp);
    if (!sign.isEmpty()) query.addQueryItem(QStringLiteral("sign"), sign);

    QString path = "/tracks/" + trackId + "/lyrics";
    if (!query.isEmpty()) {
        path += "?" + query.toString(QUrl::FullyEncoded);
    }
    Request* request = new Request(path);
    connect(request, &Request::dataReady, this, &Tracks::getLyricsHandler);
    request->get();
}

void Tracks::getTracksInfoHandler(QJsonValue value)
{
    QList<Track*> tracks;
    QJsonArray tracksArray;
    if (value.isArray()) {
        tracksArray = value.toArray();
    } else if (value.isObject() && value.toObject().contains("tracks")) {
        tracksArray = value.toObject().value("tracks").toArray();
    }

    for (const QJsonValue& v : tracksArray) {
        if (!v.isObject()) {
            continue;
        }
        Track* track = new Track(v.toObject());
        if (track != nullptr && track->trackId() != 0) {
            tracks.push_back(track);
        } else {
            delete track;
        }
    }
    emit tracksInfoReady(tracks);
}

void Tracks::getDownloadInfoHandler(QJsonValue value)
{
    QJsonArray arr;
    if (value.isArray()) {
        arr = value.toArray();
    } else if (value.isObject() && value.toObject().contains("result")) {
        // in case raw wrapped result is passed
        arr = value.toObject().value("result").toArray();
    }
    emit downloadInfoReady(m_trackId, arr);
}

void Tracks::getSupplementHandler(QJsonValue value)
{
    QJsonObject obj;
    if (value.isObject()) {
        obj = value.toObject();
    }
    emit supplementReady(m_trackId, obj);
}

void Tracks::getSimilarTracksHandler(QJsonValue value)
{
    QList<Track*> tracks;
    QJsonObject obj = value.toObject();
    QJsonArray tracksArray = obj.value("similarTracks").toArray();
    for (const QJsonValue& v : tracksArray) {
        if (!v.isObject()) continue;
        Track* track = new Track(v.toObject());
        if (track && track->trackId() != 0) {
            tracks.push_back(track);
        } else {
            delete track;
        }
    }
    emit similarTracksReady(m_trackId, tracks, obj);
}

void Tracks::getLyricsHandler(QJsonValue value)
{
    QJsonObject obj;
    if (value.isObject()) {
        obj = value.toObject();
    } else if (value.isString()) {
        obj.insert(QStringLiteral("lyrics"), value.toString());
    }
    emit lyricsReady(m_trackId, obj);
}

