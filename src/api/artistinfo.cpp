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

ArtistInfo::ArtistInfo(QObject* parent)
    : QObject(parent)
{
}

void ArtistInfo::getTrackIdsByRating()
{
}

void ArtistInfo::getBriefInfo()
{
    if (m_artistId == 0) {
        qWarning() << "Wrong artist ID!";
        return;
    }

    Request* getBriefInfoRequest = new Request(QString("/artists/%1/brief-info").arg(m_artistId));
    connect(getBriefInfoRequest, &Request::dataReady, this, &ArtistInfo::getBriefInfoRequestHandler);

    getBriefInfoRequest->get();
}

void ArtistInfo::getTracks()
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENTED YEAT!";
}

void ArtistInfo::getDirectAlbums(QString sortBy)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENTED YEAT!" << sortBy;
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

void ArtistInfo::getTrackIdsByRatingRequestHandler(QJsonValue value)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENTED YEAT!" << value;

    Request* request = qobject_cast<Request*>(sender());
    delete request;
}

void ArtistInfo::getBriefInfoRequestHandler(QJsonValue value)
{
    ArtistInfoData *aData = new ArtistInfoData();
    Artist* artist = new Artist(value.toObject().value("artist").toObject());
    aData->artist = reinterpret_cast<QObject*>(artist);

    QJsonArray popularTracks = value.toObject().value("popularTracks").toArray();
    for(const QJsonValue &v: popularTracks) {
        Track *track = new Track(v.toObject());
        if(!track->title().isEmpty()) {
            aData->popularTracks.push_back(track);
        }
    }

    QJsonArray albums = value.toObject().value("albums").toArray();
    for(const QJsonValue &v: albums) {
        Album* album = new Album(v.toObject());
        if(!album->title().isEmpty()) {
            aData->albums.push_back(album);
        }
    }

    emit getBriefInfoArtistInfoReady(aData);

    Request* request = qobject_cast<Request*>(sender());
    delete request;
}

void ArtistInfo::getTracksRequestHandler(QJsonValue value)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENTED YEAT!" << value;

    Request* request = qobject_cast<Request*>(sender());
    delete request;
}

void ArtistInfo::getDirectAlbumsRequestHandler(QJsonValue value)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENTED YEAT!" << value;

    Request* request = qobject_cast<Request*>(sender());
    delete request;
}
