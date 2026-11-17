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

#ifndef USER_H
#define USER_H

#include <QJsonObject>
#include <QObject>
#include <QSet>
#include <QStringList>

#include "../types/track.h"

class User : public QObject {
    Q_OBJECT
    Q_PROPERTY(int userID READ userID NOTIFY userIDChanged)
    Q_PROPERTY(QString displayName READ displayName NOTIFY displayNameChanged)

public:
    explicit User(QObject* parent = nullptr);
    Q_INVOKABLE void getAccountStatus();
    Q_INVOKABLE void getFeed();

    // Generic likes (tracks/albums/artists/playlists)
    Q_INVOKABLE void like(const QString &type, int id, bool remove = false);
    Q_INVOKABLE void dislike(const QString &type, int id);

    // Tracks: likes
    Q_INVOKABLE void loadLikedTracks();
    Q_INVOKABLE void likeTracks(const QStringList &trackIds);
    Q_INVOKABLE void removeLikedTracks(const QStringList &trackIds);
    Q_INVOKABLE QList<QObject*> likedTracks() { return m_likedTrackList; }
    Q_INVOKABLE bool isTrackLiked(int trackId) const;
    Q_INVOKABLE bool isTrackLiked(Track* track) const;

    // Tracks: dislikes ("Не рекомендовать")
    Q_INVOKABLE void loadDislikedTracks();
    Q_INVOKABLE void dislikeTracks(const QStringList &trackIds);
    Q_INVOKABLE void removeDislikedTracks(const QStringList &trackIds);
    Q_INVOKABLE QList<QObject*> dislikedTracks() { return m_dislikedTrackList; }
    Q_INVOKABLE bool isTrackDisliked(int trackId) const;
    Q_INVOKABLE bool isTrackDisliked(Track* track) const;

    int userID() const;
    const QString& displayName() const;

private slots:
    void getAccountStatusHandler(QJsonValue value);
    void getFeedHandler(QJsonValue value);

    void likeRequestHandler(QJsonValue value);
    void dislikeRequestHandler(QJsonValue value);
    void likedTracksHandler(QJsonValue value);
    void dislikedTracksHandler(QJsonValue value);

signals:
    void userIDChanged();
    void displayNameChanged();
    void wrongUser();
    void likeActionFinished(int actionId, const QString &action);
    void likedTracksChanged();
    void dislikedTracksChanged();

private:
    void requestTrackIds(const QString &path, const QJsonValue &value, bool likedList);
    void sendTrackAction(const QString &path, const QStringList &trackIds, const char *slot, const QString &actionName, int actionId = 0);

    int m_userID;
    int m_likeActionID;

    QString m_displayName;
    QString m_likeAction;
    QList<QObject*> m_likedTrackList;
    QList<QObject*> m_dislikedTrackList;
    QSet<int> m_likedTrackIds;
    QSet<int> m_dislikedTrackIds;
};

#endif // USER_H

