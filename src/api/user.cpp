/*
 * Copyright (C) 2023-2024 Chupligin Sergey <neochapay@gmail.com>
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
#include "user.h"
#include "request.h"

#include "../types/playlist.h"

#include <QDebug>
#include <QHash>
#include <QJsonArray>
#include <QJsonObject>
#include <QUrlQuery>

User::User(QObject* parent)
    : QObject(parent)
    , m_userID(-1)
    , m_likeActionID(0)
    , m_displayName("Loading..")
{
}

void User::getAccountStatus()
{
    Request* getAccoutStatusApiRequest = new Request("/account/status");
    connect(getAccoutStatusApiRequest, &Request::dataReady, this, &User::getAccountStatusHandler);
    getAccoutStatusApiRequest->get();
}

void User::getFeed()
{
    Request* getFeedApiRequest = new Request("/feed");
    connect(getFeedApiRequest, &Request::dataReady, this, &User::getFeedHandler);
    getFeedApiRequest->get();
}

void User::sendTrackAction(const QString &path, const QStringList &trackIds, const char *slot, const QString &actionName, int actionId)
{
    if (m_userID <= 0) {
        qWarning() << "Wrong user id";
        return;
    }
    if (trackIds.isEmpty()) {
        qWarning() << "Empty track ids";
        return;
    }

    QUrlQuery query;
    for (const QString &id : trackIds) {
        query.addQueryItem(QStringLiteral("track-ids"), id);
    }

    m_likeAction = actionName;
    m_likeActionID = actionId;

    Request* request = new Request(path);
    connect(request, &Request::dataReady, this, slot);
    request->post(query.toString(QUrl::FullyEncoded));
}

void User::like(const QString &type, int id, bool remove)
{
    if (type != "track" && type != "artist" && type != "playlist" && type != "album") {
        qWarning() << "Wrong type" << type;
        return;
    }

    if (m_userID <= 0) {
        qWarning() << "Wrong user id";
        return;
    }

    if (id <= 0) {
        qWarning() << "Wrong item id";
        return;
    }

    // Use dedicated track endpoints per spec
    if (type == "track") {
        const QStringList ids{QString::number(id)};
        if (remove) {
            removeLikedTracks(ids);
        } else {
            likeTracks(ids);
        }
        return;
    }

    m_likeAction = remove ? QStringLiteral("remove") : QStringLiteral("add-multiple");
    m_likeActionID = id;

    QUrlQuery query;
    query.addQueryItem(type + "-ids", QString::number(id));

    Request* likeRequest = new Request("/users/" + QString::number(m_userID) + "/likes/" + type + "s/" + m_likeAction);
    connect(likeRequest, &Request::dataReady, this, &User::likeRequestHandler);
    likeRequest->post(query.toString(QUrl::FullyEncoded));
}

void User::dislike(const QString &type, int id)
{
    // Backward-compatible: in existing UI "dislike" is used as unlike/remove-like for tracks.
    if (type == "track") {
        removeLikedTracks({QString::number(id)});
        return;
    }
    like(type, id, true);
}

void User::loadLikedTracks()
{
    if (m_userID <= 0) {
        qWarning() << "user ID is empty";
        return;
    }
    Request* request = new Request("/users/" + QString::number(m_userID) + "/likes/tracks");
    connect(request, &Request::dataReady, this, &User::likedTracksHandler);
    request->get();
}

void User::likeTracks(const QStringList &trackIds)
{
    if (m_userID <= 0) {
        return;
    }
    const int actionId = trackIds.value(0).toInt();
    sendTrackAction("/users/" + QString::number(m_userID) + "/likes/tracks/add-multiple",
                    trackIds, SLOT(likeRequestHandler(QJsonValue)), QStringLiteral("add-multiple"), actionId);
}

void User::removeLikedTracks(const QStringList &trackIds)
{
    if (m_userID <= 0) {
        return;
    }
    const int actionId = trackIds.value(0).toInt();
    sendTrackAction("/users/" + QString::number(m_userID) + "/likes/tracks/remove",
                    trackIds, SLOT(likeRequestHandler(QJsonValue)), QStringLiteral("remove"), actionId);
}

bool User::isTrackLiked(int trackId) const
{
    return m_likedTrackIds.contains(trackId);
}

bool User::isTrackLiked(Track* track) const
{
    if (!track || track->trackId() == 0) {
        return false;
    }
    return m_likedTrackIds.contains(track->trackId());
}

void User::loadDislikedTracks()
{
    if (m_userID <= 0) {
        qWarning() << "user ID is empty";
        return;
    }
    Request* request = new Request("/users/" + QString::number(m_userID) + "/dislikes/tracks");
    connect(request, &Request::dataReady, this, &User::dislikedTracksHandler);
    request->get();
}

void User::dislikeTracks(const QStringList &trackIds)
{
    if (m_userID <= 0) {
        return;
    }
    const int actionId = trackIds.value(0).toInt();
    sendTrackAction("/users/" + QString::number(m_userID) + "/dislikes/tracks/add-multiple",
                    trackIds, SLOT(dislikeRequestHandler(QJsonValue)), QStringLiteral("dislike-add"), actionId);
}

void User::removeDislikedTracks(const QStringList &trackIds)
{
    if (m_userID <= 0) {
        return;
    }
    const int actionId = trackIds.value(0).toInt();
    sendTrackAction("/users/" + QString::number(m_userID) + "/dislikes/tracks/remove",
                    trackIds, SLOT(dislikeRequestHandler(QJsonValue)), QStringLiteral("dislike-remove"), actionId);
}

bool User::isTrackDisliked(int trackId) const
{
    return m_dislikedTrackIds.contains(trackId);
}

bool User::isTrackDisliked(Track* track) const
{
    if (!track || track->trackId() == 0) {
        return false;
    }
    return m_dislikedTrackIds.contains(track->trackId());
}

void User::getAccountStatusHandler(QJsonValue value)
{
    QJsonObject accountObject = value.toObject().value("account").toObject();
    QString dName = accountObject.value("displayName").toString();
    if (dName != m_displayName) {
        m_displayName = dName;
        emit displayNameChanged();
    }

    int userId = accountObject.value("uid").toInt();

    if (userId == 0) {
        qWarning() << "user ID is null";
        emit wrongUser();
        return;
    }

    if (userId != m_userID) {
        m_userID = userId;
        emit userIDChanged();
    }
}

void User::getFeedHandler(QJsonValue value)
{
    Q_UNUSED(value)
}

void User::likeRequestHandler(QJsonValue value)
{
    Q_UNUSED(value)
    emit likeActionFinished(m_likeActionID, m_likeAction);
    loadLikedTracks();
}

void User::dislikeRequestHandler(QJsonValue value)
{
    Q_UNUSED(value)
    emit likeActionFinished(m_likeActionID, m_likeAction);
    loadDislikedTracks();
}

void User::requestTrackIds(const QString &pathLabel, const QJsonValue &value, bool likedList)
{
    Q_UNUSED(pathLabel)
    QList<QObject*> &targetList = likedList ? m_likedTrackList : m_dislikedTrackList;
    QSet<int> &targetIds = likedList ? m_likedTrackIds : m_dislikedTrackIds;

    qDeleteAll(targetList);
    targetList.clear();
    targetIds.clear();

    QStringList ids;
    QJsonArray tracksArray;
    const QJsonObject obj = value.toObject();

    // Likes endpoint: result.library.tracks ; Dislikes endpoint: result.tracks (TracksList) in many implementations
    if (obj.contains("library")) {
        tracksArray = obj.value("library").toObject().value("tracks").toArray();
    } else if (obj.contains("tracks")) {
        tracksArray = obj.value("tracks").toArray();
    } else if (value.isArray()) {
        tracksArray = value.toArray();
    }

    for (const QJsonValue& v : tracksArray) {
        QJsonObject trackObject = v.toObject();
        QString trackID = trackObject.value("id").toString();
        if (trackID.isEmpty()) {
            trackID = QString::number(trackObject.value("id").toInt());
        }
        QString tAlbum = trackObject.value("albumId").toString();
        if (tAlbum.isEmpty()) {
            // albumId may be inside albums[] or track might be "id:albumId" not present; try to keep id only for fetch fallback
        }

        if (!trackID.isEmpty() && trackID != "0") {
            if (!tAlbum.isEmpty()) {
                ids.push_back(trackID + ":" + tAlbum);
            } else {
                ids.push_back(trackID);
            }
            targetIds.insert(trackID.toInt());
        }
    }

    if (ids.isEmpty()) {
        if (likedList) {
            emit likedTracksChanged();
        } else {
            emit dislikedTracksChanged();
        }
        return;
    }

    Tracks* tracks = new Tracks();
    tracks->getTracksInfo(ids);
    connect(tracks, &Tracks::tracksInfoReady, [=](const QList<Track*> resolvedTracks) mutable {
        QList<QObject*> &list = likedList ? m_likedTrackList : m_dislikedTrackList;
        QSet<int> &idSet = likedList ? m_likedTrackIds : m_dislikedTrackIds;
        qDeleteAll(list);
        list.clear();
        idSet.clear();

        QHash<int, Track*> tracksById;
        for (Track* track : resolvedTracks) {
            if (track != nullptr) {
                tracksById.insert(track->trackId(), track);
            }
        }

        for (const QString &trackIdPair : ids) {
            bool ok = false;
            int trackId = trackIdPair.section(':', 0, 0).toInt(&ok);
            if (!ok) {
                continue;
            }
            Track* track = tracksById.value(trackId, nullptr);
            if (track != nullptr) {
                list.push_back(track);
                idSet.insert(track->trackId());
                tracksById.remove(trackId);
            }
        }

        if (likedList) {
            emit likedTracksChanged();
        } else {
            emit dislikedTracksChanged();
        }
        tracks->deleteLater();
    });
}

void User::likedTracksHandler(QJsonValue value)
{
    requestTrackIds(QStringLiteral("likes"), value, true);
}

void User::dislikedTracksHandler(QJsonValue value)
{
    requestTrackIds(QStringLiteral("dislikes"), value, false);
}

int User::userID() const
{
    return m_userID;
}

const QString& User::displayName() const
{
    return m_displayName;
}

