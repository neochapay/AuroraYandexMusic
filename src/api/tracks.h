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

#ifndef TRACKS_H
#define TRACKS_H

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QStringList>
#include <QVariant>

#include "../types/track.h"

class Tracks : public QObject
{
    Q_OBJECT
public:
    explicit Tracks(QObject *parent = nullptr);

    // Batch tracks info (POST /tracks/)
    Q_INVOKABLE void getTracksInfo(const QStringList &ids, bool withPositions = false); // TRACK_ID or TRACK_ID:ALBUM_ID
    void getTracksInfo(QList<QString> ids); // compatibility overload

    // Single track endpoints
    Q_INVOKABLE void getDownloadInfo(const QString &trackId);
    Q_INVOKABLE void getSupplement(const QString &trackId);
    Q_INVOKABLE void getSimilarTracks(const QString &trackId);
    Q_INVOKABLE void getLyrics(const QString &trackId, const QString &format = QString(), const QString &timeStamp = QString(), const QString &sign = QString());

signals:
    void tracksInfoReady(QList<Track*> tracks);
    void downloadInfoReady(const QString &trackId, const QJsonArray &downloadInfo);
    void supplementReady(const QString &trackId, const QJsonObject &supplement);
    void similarTracksReady(const QString &trackId, const QList<Track*> &similarTracks, const QJsonObject &rawResult);
    void lyricsReady(const QString &trackId, const QJsonObject &lyrics);

    void error(const QString &message);

private slots:
    void getTracksInfoHandler(QJsonValue value);
    void getDownloadInfoHandler(QJsonValue value);
    void getSupplementHandler(QJsonValue value);
    void getSimilarTracksHandler(QJsonValue value);
    void getLyricsHandler(QJsonValue value);

private:
    QString m_trackId;
};

#endif // TRACKS_H

