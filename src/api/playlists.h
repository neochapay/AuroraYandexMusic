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

#include <QJsonValue>
#include <QObject>

#include "../types/playlist.h"

class Playlists : public QObject {
    Q_OBJECT
    Q_PROPERTY(int userID READ userID WRITE setUserID NOTIFY userIDChanged)

public:
    explicit Playlists(QObject* parent = nullptr);

    Q_INVOKABLE void getUserLists();
    Q_INVOKABLE void getUserList(int kind);
    Q_INVOKABLE void getUserLists(QList<int> kinds);
    Q_INVOKABLE void create(QString title, QString visibility);
    Q_INVOKABLE void rename(int kind, QString value);
    Q_INVOKABLE void deleteList(int kind);
    Q_INVOKABLE void change(int kind, QList<int> addedTracks = QList<int>(), QList<int> deletedTraks = QList<int>());
    Q_INVOKABLE void recommindations(int kind);
    //Необходимо передать "public" или "private" в качестве значения
    Q_INVOKABLE void changeVisiblity(int kind, QString visiblity);

    int userID() const;
    void setUserID(int newUserID);

signals:
    void userIDChanged();
    void playlistChanged(Playlist* playlist);
    void userPlaylistsReady();

private slots:
    void getUserListRequestHandler(QJsonValue value);
    void getUserListsRequestHandler(QJsonValue value);
    void createRequestHandler(QJsonValue value);
    void renameRequestHandler(QJsonValue value);
    void deleteListRequestHandler(QJsonValue value);
    void changeRequestHandler(QJsonValue value);
    void recommindationsRequestHandler(QJsonValue value);
    void changeVisiblityRequestHandler(QJsonValue value);

private:
    int m_userID;
};

#endif // PLAYLISTS_H
