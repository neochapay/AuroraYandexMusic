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

Tracks::Tracks(QObject *parent) : QObject(parent)
{

}

void Tracks::getTracksInfo(QList<QString> ids)
{
    if(ids.count() == 0) {
        return;
    }

    QString idsString;
    for(QString id : ids) {
        idsString += id + ",";
    }

    QUrlQuery query;
    query.addQueryItem("track-ids", idsString);

    Request* trackRequest = new Request("/tracks/");
    connect(trackRequest, &Request::dataReady, this, &Tracks::getTracksInfoHandler);

    trackRequest->post(query.toString());
}

void Tracks::getTracksInfoHandler(QJsonValue value)
{
    QList<Track*> tracks;
    QJsonArray tracksArray = value.toArray();

    for (const QJsonValue& v : tracksArray) {
        Track* track = new Track(v.toObject());
        if(track->trackId() != 0) {
            tracks.push_back(track);
        }
    }
    emit tracksInfoReady(tracks);
}
