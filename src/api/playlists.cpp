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

#include "playlists.h"
#include "request.h"

#include <QUrlQuery>
#include <QJsonArray>

Playlists::Playlists(QObject* parent)
    : QObject(parent)
    , m_userID(0)
{
}

void Playlists::getUserLists()
{
    if (m_userID == 0) {
        return;
    }

    Request* getUserListsRequest = new Request("/users/" + QString::number(m_userID) + "/playlists/list");
    connect(getUserListsRequest, &Request::dataReady, this, &Playlists::getUserListsRequestHandler);

    getUserListsRequest->get();
}

void Playlists::getUserList(int kind)
{
    if (m_userID == 0 || kind == 0) {
        return;
    }

    Request* getUserListsRequest = new Request("/users/" + QString::number(m_userID) + "/playlists/" + QString::number(kind));
    connect(getUserListsRequest, &Request::dataReady, this, &Playlists::getUserListRequestHandler);

    getUserListsRequest->get();
}

void Playlists::getUserLists(QList<int> kinds)
{
    if (m_userID == 0 || kinds.isEmpty()) {
        return;
    }

    QString playlists;
    for (const int i : kinds) {
        playlists += QString::number(i) + ",";
    }

    Request* getUserListsRequest = new Request("/users/" + QString::number(m_userID) + "/playlists?user_id=" + m_userID + "&kinds=" + playlists + "&mixed=false&rich-tracks=false");
    connect(getUserListsRequest, &Request::dataReady, this, &Playlists::getUserListRequestHandler);

    getUserListsRequest->get();
}

void Playlists::create(QString title, QString visibility)
{
    if (m_userID == 0) {
        return;
    }

    QUrlQuery query;
    query.addQueryItem("title", title);
    query.addQueryItem("visibility", visibility);

    Request* createRequest = new Request("/users/" + QString::number(m_userID) + "/playlists/create");
    connect(createRequest, &Request::dataReady, this, &Playlists::createRequestHandler);

    createRequest->post(query.toString());
}

void Playlists::rename(int kind, QString value)
{
    if (m_userID == 0) {
        return;
    }

    QUrlQuery query;
    query.addQueryItem("value", value);

    Request* renameRequest = new Request("/users/" + QString::number(m_userID) + "/playlists/" + kind + "/name");
    connect(renameRequest, &Request::dataReady, this, &Playlists::renameRequestHandler);

    renameRequest->post(query.toString());
}

void Playlists::deleteList(int kind)
{
    if (m_userID == 0) {
        return;
    }

    Request* deleteListRequest = new Request("/users/" + QString::number(m_userID) + "/playlists/" + kind + "/delete");
    connect(deleteListRequest, &Request::dataReady, this, &Playlists::deleteListRequestHandler);

    deleteListRequest->post();
}

void Playlists::change(int kind, QList<int> addedTracks, QList<int> deletedTraks)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENTED YEAT!";
    Q_UNUSED(addedTracks)
    Q_UNUSED(deletedTraks)
    return;
    //Используй '{"diff":{"op":"insert","at":0,"tracks":[{"id":"20599729","albumId":"2347459"}]}}' - для добавления, {"diff":{"op":"delete","from":0,"to":1,"tracks":[{"id":"20599729","albumId":"2347459"}]}} - для удаления треков
    if (m_userID == 0) {
        return;
    }

    QString changeDiffJSON;

    QUrlQuery query;
    query.addQueryItem("diff", changeDiffJSON);
    query.addQueryItem("revision", QString());

    Request* deleteListRequest = new Request("/users/" + QString::number(m_userID) + "/playlists/" + kind + "/change-relative");
    connect(deleteListRequest, &Request::dataReady, this, &Playlists::deleteListRequestHandler);

    deleteListRequest->post();
}

void Playlists::recommindations(int kind)
{
    if (m_userID == 0 || kind == 0) {
        return;
    }

    Request* recommindationsRequest = new Request("/users/" + QString::number(m_userID) + "/playlists/" + kind + "/recommendations");
    connect(recommindationsRequest, &Request::dataReady, this, &Playlists::recommindationsRequestHandler);

    recommindationsRequest->get();
}

void Playlists::changeVisiblity(int kind, QString visiblity)
{
    if (m_userID == 0) {
        return;
    }

    QUrlQuery query;
    query.addQueryItem("value", visiblity);

    Request* visiblityRequest = new Request("/users/" + QString::number(m_userID) + "/playlists/" + kind + "/visiblity");
    connect(visiblityRequest, &Request::dataReady, this, &Playlists::changeVisiblityRequestHandler);

    visiblityRequest->post(query.toString());
}

int Playlists::userID() const
{
    return m_userID;
}

void Playlists::setUserID(int newUserID)
{
    if (m_userID == newUserID)
        return;
    m_userID = newUserID;
    emit userIDChanged();
}

void Playlists::getUserListRequestHandler(QJsonValue value)
{
    Playlist* playlist = new Playlist(value.toObject());
    if(playlist->tracks().count() > 0) {
        emit playlistChanged(playlist);
    }

    Request* request = qobject_cast<Request*>(sender());
    if(request != nullptr) {
        delete request;
    }
}

void Playlists::getUserListsRequestHandler(QJsonValue value)
{
    QList<QObject *> playlists;

    QJsonArray playlistArray = value.toArray();
    for (const QJsonValue& v : playlistArray) {
        QJsonObject playListObject = v.toObject();
        Playlist *playlist = new Playlist(playListObject);

        if(!playlist->title().isEmpty()) {
            playlists.push_back(playlist);
        }
    }

    emit userPlaylistsReady();

    Request* request = qobject_cast<Request*>(sender());
    if(request != nullptr) {
        delete request;
    }
}

void Playlists::createRequestHandler(QJsonValue value)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENTED YEAT!" << value;

    Request* request = qobject_cast<Request*>(sender());
    if(request != nullptr) {
        delete request;
    }
}

void Playlists::renameRequestHandler(QJsonValue value)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENTED YEAT!" << value;

    Request* request = qobject_cast<Request*>(sender());
    if(request != nullptr) {
        delete request;
    }
}

void Playlists::deleteListRequestHandler(QJsonValue value)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENTED YEAT!" << value;

    Request* request = qobject_cast<Request*>(sender());
    if(request != nullptr) {
        delete request;
    }
}

void Playlists::changeRequestHandler(QJsonValue value)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENTED YEAT!" << value;

    Request* request = qobject_cast<Request*>(sender());
    if(request != nullptr) {
        delete request;
    }
}

void Playlists::recommindationsRequestHandler(QJsonValue value)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENTED YEAT!" << value;

    Request* request = qobject_cast<Request*>(sender());
    if(request != nullptr) {
        delete request;
    }
}

void Playlists::changeVisiblityRequestHandler(QJsonValue value)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENTED YEAT!" << value;

    Request* request = qobject_cast<Request*>(sender());
    if(request != nullptr) {
        delete request;
    }
}
