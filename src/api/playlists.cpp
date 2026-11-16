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

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>

Playlists::Playlists(QObject* parent)
    : QObject(parent)
    , m_userID(0)
{
}

void Playlists::getUserLists()
{
    if (m_userID <= 0) {
        return;
    }

    Request* request = new Request("/users/" + QString::number(m_userID) + "/playlists/list");
    connect(request, &Request::dataReady, this, &Playlists::getUserListsRequestHandler);
    request->get();
}

void Playlists::getUserList(int kind)
{
    if (m_userID <= 0 || kind <= 0) {
        return;
    }
    m_currentKind = kind;

    Request* request = new Request("/users/" + QString::number(m_userID) + "/playlists/" + QString::number(kind));
    connect(request, &Request::dataReady, this, &Playlists::getUserListRequestHandler);
    request->get();
}

void Playlists::getUserLists(QList<int> kinds)
{
    if (m_userID <= 0 || kinds.isEmpty()) {
        return;
    }

    QStringList kindsStr;
    for (const int i : kinds) {
        kindsStr << QString::number(i);
    }

    Request* request = new Request("/users/" + QString::number(m_userID) + "/playlists?user_id=" + QString::number(m_userID) + "&kinds=" + kindsStr.join(',') + "&mixed=false&rich-tracks=true");
    connect(request, &Request::dataReady, this, &Playlists::getUserListRequestHandler);
    request->get();
}

void Playlists::getPlaylistsByIds(const QStringList &playlistIds)
{
    if (playlistIds.isEmpty()) {
        return;
    }

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("playlist-ids"), playlistIds.join(','));

    Request* request = new Request("/playlists/list");
    connect(request, &Request::dataReady, this, &Playlists::getPlaylistsByIdsRequestHandler);
    request->post(query.toString(QUrl::FullyEncoded));
}

void Playlists::create(const QString &title, const QString &visibility)
{
    if (m_userID <= 0) {
        return;
    }

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("title"), title);
    query.addQueryItem(QStringLiteral("visibility"), visibility.isEmpty() ? QStringLiteral("public") : visibility);

    Request* request = new Request("/users/" + QString::number(m_userID) + "/playlists/create");
    connect(request, &Request::dataReady, this, &Playlists::createRequestHandler);
    request->post(query.toString(QUrl::FullyEncoded));
}

void Playlists::rename(int kind, const QString &value)
{
    if (m_userID <= 0 || kind <= 0) {
        return;
    }
    m_currentKind = kind;

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("value"), value);

    Request* request = new Request("/users/" + QString::number(m_userID) + "/playlists/" + QString::number(kind) + "/name");
    connect(request, &Request::dataReady, this, &Playlists::renameRequestHandler);
    request->post(query.toString(QUrl::FullyEncoded));
}

void Playlists::deleteList(int kind)
{
    if (m_userID <= 0 || kind <= 0) {
        return;
    }
    m_currentKind = kind;

    Request* request = new Request("/users/" + QString::number(m_userID) + "/playlists/" + QString::number(kind) + "/delete");
    connect(request, &Request::dataReady, this, &Playlists::deleteListRequestHandler);
    request->post();
}

QJsonArray Playlists::tracksToJsonArray(const QVariantList &tracks) const
{
    QJsonArray arr;
    for (const QVariant &v : tracks) {
        if (v.type() == QVariant::Map) {
            const QVariantMap m = v.toMap();
            QJsonObject t;
            if (m.contains("id")) t.insert(QStringLiteral("id"), m.value("id").toString());
            if (m.contains("albumId")) t.insert(QStringLiteral("albumId"), m.value("albumId").toString());
            if (!t.isEmpty()) arr.append(t);
        } else if (v.canConvert<QString>()) {
            const QString s = v.toString();
            const QStringList parts = s.split(':');
            if (!parts.isEmpty() && !parts.first().isEmpty()) {
                QJsonObject t;
                t.insert(QStringLiteral("id"), parts.value(0));
                if (parts.size() > 1 && !parts.value(1).isEmpty()) {
                    t.insert(QStringLiteral("albumId"), parts.value(1));
                }
                arr.append(t);
            }
        }
    }
    return arr;
}

void Playlists::changeTracks(int kind, int revision, const QJsonObject &diff)
{
    if (m_userID <= 0 || kind <= 0) {
        return;
    }
    m_currentKind = kind;

    QUrlQuery query;
    QJsonDocument doc(diff);
    query.addQueryItem(QStringLiteral("diff"), QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
    query.addQueryItem(QStringLiteral("revision"), QString::number(qMax(0, revision)));

    Request* request = new Request("/users/" + QString::number(m_userID) + "/playlists/" + QString::number(kind) + "/change-relative");
    connect(request, &Request::dataReady, this, &Playlists::changeRequestHandler);
    request->post(query.toString(QUrl::FullyEncoded));
}

void Playlists::addTracks(int kind, int revision, const QVariantList &tracks, int at)
{
    QJsonObject diffObj;
    QJsonObject diff;
    diff.insert(QStringLiteral("op"), QStringLiteral("insert"));
    diff.insert(QStringLiteral("at"), at);
    diff.insert(QStringLiteral("tracks"), tracksToJsonArray(tracks));
    diffObj.insert(QStringLiteral("diff"), diff);
    changeTracks(kind, revision, diffObj);
}

void Playlists::removeTracks(int kind, int revision, const QVariantList &tracks, int from, int to)
{
    QJsonArray tracksArr = tracksToJsonArray(tracks);
    if (to < 0) {
        to = from + tracksArr.size();
    }

    QJsonObject diffObj;
    QJsonObject diff;
    diff.insert(QStringLiteral("op"), QStringLiteral("delete"));
    diff.insert(QStringLiteral("from"), from);
    diff.insert(QStringLiteral("to"), to);
    diff.insert(QStringLiteral("tracks"), tracksArr);
    diffObj.insert(QStringLiteral("diff"), diff);
    changeTracks(kind, revision, diffObj);
}

void Playlists::change(int kind, QList<int> addedTracks, QList<int> deletedTraks)
{
    Q_UNUSED(kind)
    Q_UNUSED(addedTracks)
    Q_UNUSED(deletedTraks)
    qWarning() << Q_FUNC_INFO << "legacy signature is insufficient (needs revision and track albumId). Use addTracks/removeTracks/changeTracks.";
}

void Playlists::recommindations(int kind)
{
    recommendations(kind);
}

void Playlists::recommendations(int kind)
{
    if (m_userID <= 0 || kind <= 0) {
        return;
    }
    m_currentKind = kind;

    Request* request = new Request("/users/" + QString::number(m_userID) + "/playlists/" + QString::number(kind) + "/recommendations");
    connect(request, &Request::dataReady, this, &Playlists::recommindationsRequestHandler);
    request->get();
}

void Playlists::changeVisiblity(int kind, QString visiblity)
{
    changeVisibility(kind, visiblity);
}

void Playlists::changeVisibility(int kind, const QString &visibility)
{
    if (m_userID <= 0 || kind <= 0) {
        return;
    }
    m_currentKind = kind;

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("value"), visibility);

    Request* request = new Request("/users/" + QString::number(m_userID) + "/playlists/" + QString::number(kind) + "/visibility");
    connect(request, &Request::dataReady, this, &Playlists::changeVisiblityRequestHandler);
    request->post(query.toString(QUrl::FullyEncoded));
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

void Playlists::emitPlaylistOrList(QJsonValue value, bool asListSignal)
{
    if (value.isArray()) {
        QList<Playlist*> playlists;
        for (const QJsonValue& v : value.toArray()) {
            if (!v.isObject()) continue;
            Playlist *playlist = new Playlist(v.toObject());
            playlists.push_back(playlist);
            emit playlistChanged(playlist);
        }
        emit playlistsReady(playlists);
        if (asListSignal) {
            emit userPlaylistsReady();
        }
        return;
    }

    if (value.isObject()) {
        Playlist* playlist = new Playlist(value.toObject());
        emit playlistChanged(playlist);
        QList<Playlist*> list;
        list << playlist;
        emit playlistsReady(list);
        return;
    }
}

void Playlists::getUserListRequestHandler(QJsonValue value)
{
    emitPlaylistOrList(value, false);
}

void Playlists::getUserListsRequestHandler(QJsonValue value)
{
    if (value.isArray()) {
        QList<Playlist*> playlists;
        for (const QJsonValue& v : value.toArray()) {
            if (!v.isObject()) continue;
            Playlist *playlist = new Playlist(v.toObject());
            playlists.push_back(playlist);
            emit playlistChanged(playlist);
        }
        emit playlistsReady(playlists);
        emit userPlaylistsReady();
        return;
    }
    emitPlaylistOrList(value, false);
}

void Playlists::getPlaylistsByIdsRequestHandler(QJsonValue value)
{
    getUserListsRequestHandler(value);
}

void Playlists::createRequestHandler(QJsonValue value)
{
    if (value.isObject()) {
        Playlist* playlist = new Playlist(value.toObject());
        emit playlistCreated(playlist);
        emit playlistChanged(playlist);
    } else {
        emit error(QStringLiteral("Failed to create playlist"));
    }
}

void Playlists::renameRequestHandler(QJsonValue value)
{
    if (value.isObject()) {
        Playlist* playlist = new Playlist(value.toObject());
        emit playlistRenamed(playlist);
        emit playlistChanged(playlist);
    } else {
        emit error(QStringLiteral("Failed to rename playlist"));
    }
}

void Playlists::deleteListRequestHandler(QJsonValue value)
{
    Q_UNUSED(value)
    emit playlistDeleted(m_currentKind, true);
}

void Playlists::changeRequestHandler(QJsonValue value)
{
    if (value.isObject()) {
        Playlist* playlist = new Playlist(value.toObject());
        emit playlistTracksChanged(playlist);
        emit playlistChanged(playlist);
    } else {
        emit error(QStringLiteral("Failed to change playlist tracks"));
    }
}

void Playlists::recommindationsRequestHandler(QJsonValue value)
{
    QJsonObject obj;
    if (value.isObject()) {
        obj = value.toObject();
    } else if (value.isArray()) {
        obj.insert(QStringLiteral("tracks"), value.toArray());
    }
    emit recommendationsReady(m_currentKind, obj);
}

void Playlists::changeVisiblityRequestHandler(QJsonValue value)
{
    if (value.isObject()) {
        Playlist* playlist = new Playlist(value.toObject());
        emit visibilityChanged(playlist);
        emit playlistChanged(playlist);
    } else {
        emit error(QStringLiteral("Failed to change playlist visibility"));
    }
}

