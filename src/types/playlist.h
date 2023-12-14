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

#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "cover.h"
#include "track.h"
#include <QDateTime>
#include <QObject>

class PlaylistPrivate {
public:
    bool available;
    QString backgroundImageUrl;
    QString backgroundVideoUrl;
    Cover cover;
    Cover coverWithoutText;
    QDateTime created;
    QString description;
    QString descriptionFormatted;
    int durationMs;
    bool everPlayed;
    QString generatedPlaylistType;
    QString idForFrom;
    bool isBanner;
    bool isPremiere;
    int kind;
    QDateTime modified;
    QString ogImage;
    int ownerUid;
    QString ownerName;
    QString ownerLogin;
    QString playlistUuid;
    int revision;
    int snapshot;
    QStringList tags;
    QString title;
    int trackCount;
    QList<Track*> tracks;
};

class Playlist : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool available READ available NOTIFY playlistChanged)
    Q_PROPERTY(QString backgroundImageUrl READ backgroundImageUrl NOTIFY playlistChanged)
    Q_PROPERTY(QString backgroundVideoUrl READ backgroundVideoUrl NOTIFY playlistChanged)
    Q_PROPERTY(QObject* cover READ cover NOTIFY playlistChanged)
    Q_PROPERTY(QObject* coverWithoutText READ coverWithoutText NOTIFY playlistChanged)
    Q_PROPERTY(QDateTime created READ created NOTIFY playlistChanged)
    Q_PROPERTY(QString description READ description NOTIFY playlistChanged)
    Q_PROPERTY(QString descriptionFormatted READ descriptionFormatted NOTIFY playlistChanged)
    Q_PROPERTY(int durationMs READ durationMs NOTIFY playlistChanged)
    Q_PROPERTY(bool everPlayed READ everPlayed NOTIFY playlistChanged)
    Q_PROPERTY(QString generatedPlaylistType READ generatedPlaylistType NOTIFY playlistChanged)
    Q_PROPERTY(QString idForFrom READ idForFrom NOTIFY playlistChanged)
    Q_PROPERTY(bool isBanner READ isBanner NOTIFY playlistChanged)
    Q_PROPERTY(bool isPremiere READ isPremiere NOTIFY playlistChanged)
    Q_PROPERTY(int kind READ kind NOTIFY playlistChanged)
    // TODO: add madeFor
    Q_PROPERTY(QDateTime modified READ modified NOTIFY playlistChanged)
    Q_PROPERTY(QString ogImage READ ogImage NOTIFY playlistChanged)

    Q_PROPERTY(int ownerUid READ ownerUid NOTIFY playlistChanged)
    Q_PROPERTY(QString ownerName READ ownerName NOTIFY playlistChanged)
    Q_PROPERTY(QString ownerLogin READ ownerLogin NOTIFY playlistChanged)

    Q_PROPERTY(QString playlistUuid READ playlistUuid NOTIFY playlistChanged)
    Q_PROPERTY(int revision READ revision NOTIFY playlistChanged)
    Q_PROPERTY(int snapshot READ snapshot NOTIFY playlistChanged)
    Q_PROPERTY(QStringList tags READ tags NOTIFY playlistChanged)
    Q_PROPERTY(QString title READ title NOTIFY playlistChanged)
    Q_PROPERTY(int trackCount READ trackCount NOTIFY playlistChanged)
    // TODO: add tracks
    Q_PROPERTY(QList<QObject*> tracks READ tracks NOTIFY playlistChanged)

public:
    explicit Playlist(QObject* parent = nullptr);
    explicit Playlist(const Playlist& other, QObject* parent = nullptr);
    explicit Playlist(QJsonObject object, QObject* parent = nullptr);
    virtual ~Playlist();
    Playlist& operator=(const Playlist& other);

    bool available() const;
    const QString& backgroundImageUrl() const;
    const QString& backgroundVideoUrl() const;
    QObject* cover() const;
    QObject* coverWithoutText() const;
    const QDateTime& created() const;
    const QString& description() const;
    const QString& descriptionFormatted() const;
    int durationMs() const;
    bool everPlayed() const;
    const QString& generatedPlaylistType() const;
    const QString& idForFrom() const;
    bool isBanner() const;
    bool isPremiere() const;
    int kind() const;
    const QDateTime& modified() const;
    const QString& ogImage() const;
    const QString& playlistUuid() const;
    int revision() const;
    int snapshot() const;
    const QStringList& tags() const;
    const QString& title() const;
    int trackCount() const;
    int ownerUid() const;;
    const QString &ownerName() const;
    const QString &ownerLogin() const;
    const QList<QObject *> &tracks() const;

signals:
    void playlistChanged();

private:
    PlaylistPrivate* d_ptr;
};

Q_DECLARE_METATYPE(Playlist)

#endif // PLAYLIST_H
