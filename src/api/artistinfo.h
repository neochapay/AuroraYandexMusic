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

#ifndef ARTISTINFO_H
#define ARTISTINFO_H

#include <QJsonValue>
#include <QObject>

#include "../types/artist.h"
#include "../types/album.h"
#include "../types/playlist.h"
#include "../types/ArtistInfoData.h"

class ArtistInfo : public QObject {
    Q_OBJECT
    Q_PROPERTY(int artistId READ artistId WRITE setArtistId NOTIFY artistIdChanged)

public:
    explicit ArtistInfo(QObject* parent = nullptr);

    Q_INVOKABLE void getTrackIdsByRating();
    Q_INVOKABLE void getBriefInfo();
    Q_INVOKABLE void getTracks();
    Q_INVOKABLE void getDirectAlbums(QString sortBy = "rating"); // rating || year

    int artistId() const;
    void setArtistId(int newArtistId);

signals:
    void artistIdChanged();
    void getBriefInfoArtistInfoReady(ArtistInfoData* artistData);

private slots:
    void getTrackIdsByRatingRequestHandler(QJsonValue value);
    void getBriefInfoRequestHandler(QJsonValue value);
    void getTracksRequestHandler(QJsonValue value);
    void getDirectAlbumsRequestHandler(QJsonValue value);

private:
    int m_artistId;
};

#endif // ARTISTINFO_H
