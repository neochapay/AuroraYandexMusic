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

#include "user.h"
#include "request.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

User::User(QObject* parent)
    : QObject(parent)
    , m_userID(-1)
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

void User::like(QString type, int id, bool remove)
{
    if (type != "track" && type != "artist" && type != "playlist") {
        qWarning() << "Wrong type" << type;
        return;
    }

    if (m_userID == 0) {
        qWarning() << "Wrong user id";
        return;
    }

    if (id == 0) {
        qWarning() << "Wrong item id";
        return;
    }

    QString action = "add-multiple";
    if (remove) {
        action = "remove";
    }

    m_likeActionID = id;

    QUrlQuery query;
    query.addQueryItem(type + "-ids", QString::number(id));

    Request* likeRequest = new Request("/users/" + QString::number(m_userID) + "/likes/" + type + "s/" + action);
    likeRequest->setDebug(true);
    connect(likeRequest, &Request::dataReady, this, &User::likeRequestHandler);

    likeRequest->post(query.toString());
}

void User::dislike(int id, bool remove)
{
    if (m_userID == 0) {
        qWarning() << "Wrong user id";
        return;
    }

    if (id == 0) {
        qWarning() << "Wrong item id";
        return;
    }
    QString action = "add-multiple";
    if (remove) {
        action = "remove";
    }

    m_likeActionID = id;

    QUrlQuery query;
    query.addQueryItem("track-ids-ids", QString::number(id));

    Request* likeRequest = new Request("/users/" + QString::number(m_userID) + "/dislikes/tracks/" + action);
    connect(likeRequest, &Request::dataReady, this, &User::likeRequestHandler);

    likeRequest->post(query.toString());
}

void User::loadLikedTracks()
{
    if (m_userID == 0) {
        qWarning() << "user ID is empty";
        return;
    }
    Request* likeRequest = new Request("/users/" + QString::number(m_userID) + "/likes/tracks/");
    likeRequest->setDebug(true);
    connect(likeRequest, &Request::dataReady, this, &User::likedTracksHandler);
    likeRequest->get();
}

bool User::isTrackLiked(Track* track)
{
    QString trackId = track->trackId();
    if (trackId.isEmpty()) {
        return false;
    }

    foreach (LikedTrack* track, m_likedTrackList) {
        if (track->trackId == trackId) {
            return true;
        }
    }

    return false;
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
        qWarning() << "uers ID is null";
        emit wrongUser();
    }

    if (userId != m_userID) {
        m_userID = userId;
        emit userIDChanged();
    }
}

void User::getFeedHandler(QJsonValue value)
{
    qDebug() << value;
}

void User::likeRequestHandler(QJsonValue value)
{
    if (!value.toObject().value("revision").toString().isEmpty()) {
        emit likeActionFinished(m_likeActionID);
    }
}

void User::dislikeRequestHandler(QJsonValue value)
{
    if (!value.toObject().value("revision").toString().isEmpty()) {
        emit likeActionFinished(m_likeActionID);
    }
}

void User::likedTracksHandler(QJsonValue value)
{
    m_likedTrackList.clear();
    QJsonArray likedTracksArray = value.toObject().value("library").toObject().value("tracks").toArray();
    for (const QJsonValue& v : likedTracksArray) {
        QJsonObject trackObject = v.toObject();
        LikedTrack* track = new LikedTrack;
        track->trackId = trackObject.value("id").toString();
        track->albumId = trackObject.value("albumId").toString();
        track->timestamp = QDateTime::fromString(trackObject.value("timestamp").toString());

        m_likedTrackList.push_back(track);
    }
}

int User::userID() const
{
    return m_userID;
}

const QString& User::displayName() const
{
    return m_displayName;
}
