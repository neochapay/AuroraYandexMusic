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

#ifndef PLAYLISTS_H
#define PLAYLISTS_H

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QStringList>
#include <QVariantList>

#include "../types/playlist.h"

class Playlists : public QObject {
    Q_OBJECT
    Q_PROPERTY(int userID READ userID WRITE setUserID NOTIFY userIDChanged)

public:
    explicit Playlists(QObject* parent = nullptr);

    // User playlists
    Q_INVOKABLE void getUserLists();
    Q_INVOKABLE void getUserList(int kind);
    Q_INVOKABLE void getUserLists(QList<int> kinds);
    Q_INVOKABLE void getPlaylistsByIds(const QStringList &playlistIds); // "uid:kind" pairs, POST /playlists/list

    // Playlist management
    Q_INVOKABLE void create(const QString &title, const QString &visibility = QStringLiteral("public"));
    Q_INVOKABLE void rename(int kind, const QString &value);
    Q_INVOKABLE void deleteList(int kind);

    // Track changes (change-relative)
    Q_INVOKABLE void changeTracks(int kind, int revision, const QJsonObject &diff);
    Q_INVOKABLE void addTracks(int kind, int revision, const QVariantList &tracks, int at = 0);      // tracks: [{id, albumId}, ...] or ["id:albumId"]
    Q_INVOKABLE void removeTracks(int kind, int revision, const QVariantList &tracks, int from = 0, int to = -1);
    Q_INVOKABLE void change(int kind, QList<int> addedTracks = QList<int>(), QList<int> deletedTraks = QList<int>()); // legacy/limited

    Q_INVOKABLE void recommindations(int kind);
    Q_INVOKABLE void recommendations(int kind);

    // Visibility
    Q_INVOKABLE void changeVisiblity(int kind, QString visiblity); // legacy typo name
    Q_INVOKABLE void changeVisibility(int kind, const QString &visibility);

    int userID() const;
    void setUserID(int newUserID);

signals:
    void userIDChanged();
    void playlistChanged(Playlist* playlist);
    void playlistsReady(const QList<Playlist*> &playlists);
    void userPlaylistsReady(); // compatibility

    void playlistCreated(Playlist* playlist);
    void playlistRenamed(Playlist* playlist);
    void playlistDeleted(int kind, bool success);
    void playlistTracksChanged(Playlist* playlist);
    void recommendationsReady(int kind, const QJsonObject &result);
    void visibilityChanged(Playlist* playlist);
    void error(const QString &message);

private slots:
    void getUserListRequestHandler(QJsonValue value);
    void getUserListsRequestHandler(QJsonValue value);
    void getPlaylistsByIdsRequestHandler(QJsonValue value);
    void createRequestHandler(QJsonValue value);
    void renameRequestHandler(QJsonValue value);
    void deleteListRequestHandler(QJsonValue value);
    void changeRequestHandler(QJsonValue value);
    void recommindationsRequestHandler(QJsonValue value);
    void changeVisiblityRequestHandler(QJsonValue value);

private:
    QJsonArray tracksToJsonArray(const QVariantList &tracks) const;
    void emitPlaylistOrList(QJsonValue value, bool asListSignal);

    int m_userID;
    int m_currentKind = 0;
};

#endif // PLAYLISTS_H

