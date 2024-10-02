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

#include "feed.h"
#include "../types/artist.h"

#include <QJsonArray>

Feed::Feed(QObject* parent)
    : QObject(parent)
{
    m_getFeedApiRequest = new Request("/feed");
    connect(m_getFeedApiRequest, &Request::dataReady, this, &Feed::getFeedHandler);
    connect(m_getFeedApiRequest, &Request::errorReady, this, &Feed::errorReady);
}

void Feed::get()
{
    m_getFeedApiRequest->get();
}

void Feed::getFeedHandler(QJsonValue value)
{
    m_generatedPlaylists.clear();
    QJsonArray generatedPlaylistsJsonArray = value.toObject().value("generatedPlaylists").toArray();
    for (const QJsonValue& v : generatedPlaylistsJsonArray) {
        Playlist* playList = new Playlist(v.toObject().value("data").toObject());
        m_generatedPlaylists.push_back(playList);
    }

    QJsonArray tracksToPlayArray = value.toObject().value("days").toArray().first().toObject().value("tracksToPlay").toArray();
    for (const QJsonValue& v : tracksToPlayArray) {
        Track* track = new Track(v.toObject());
        m_tracksToPlay.push_back(track);
    }

    emit feedReady();
}

const QList<QObject*>& Feed::generatedPlaylists() const
{
    return m_generatedPlaylists;
}

const QList<QObject*>& Feed::tracksToPlay() const
{
    return m_tracksToPlay;
}
