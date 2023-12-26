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

#include "albuminfo.h"
#include "request.h"

AlbumInfo::AlbumInfo(QObject *parent) : QObject(parent)
{

}

void AlbumInfo::get(bool withTracks)
{
    if (m_albumId == 0) {
        qWarning() << "Wrong Album ID!";
        return;
    }

    QString requestString = QString("/albums/%1").arg(m_albumId);
    if(withTracks) {
        requestString += "/with-tracks";
    }

    Request* getAlbumRequest = new Request(requestString);
    connect(getAlbumRequest, &Request::dataReady, this, &AlbumInfo::getAlbumRequestHandler);

    getAlbumRequest->get();
}

int AlbumInfo::albumId() const
{
    return m_albumId;
}

void AlbumInfo::setAlbumId(int newAlbumId)
{
    if (m_albumId == newAlbumId)
        return;
    m_albumId = newAlbumId;
    emit albumIdChanged();
}

void AlbumInfo::getAlbumRequestHandler(QJsonValue value)
{
    Album* album = new Album(value.toObject());
    if(album->albumId() != 0) {
        emit albumReady(album);
    } else {
        qWarning() << "Wrong album";
    }
}
