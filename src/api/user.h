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

#ifndef USER_H
#define USER_H

#include <QJsonObject>
#include <QObject>

#include "../types/track.h"

struct LikedTrack {
public:
    QString trackId;
    QString albumId;
    QDateTime timestamp;
};

class User : public QObject {
    Q_OBJECT
    Q_PROPERTY(int userID READ userID NOTIFY userIDChanged)
    Q_PROPERTY(QString displayName READ displayName NOTIFY displayNameChanged)

public:
    explicit User(QObject* parent = nullptr);
    Q_INVOKABLE void getAccountStatus();
    Q_INVOKABLE void getFeed();

    Q_INVOKABLE void like(QString type, int id, bool remove = false);
    Q_INVOKABLE void dislike(QString type, int id);

    Q_INVOKABLE void loadLikedTracks();
    Q_INVOKABLE bool isTrackLiked(Track* track);

    int userID() const;
    const QString& displayName() const;

private slots:
    void getAccountStatusHandler(QJsonValue value);
    void getFeedHandler(QJsonValue value);

    void likeRequestHandler(QJsonValue value);
    void likedTracksHandler(QJsonValue value);

signals:
    void userIDChanged();
    void displayNameChanged();
    void wrongUser();
    void likeActionFinished(int actionId, QString action);

private:
    int m_userID;
    int m_likeActionID;

    QString m_displayName;
    QString m_likeAction;
    QList<LikedTrack*> m_likedTrackList;
};

#endif // USER_H
