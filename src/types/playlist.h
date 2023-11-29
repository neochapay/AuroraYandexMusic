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
#include <QDateTime>
#include <QObject>

class Playlist : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool available READ available)
    Q_PROPERTY(QString backgroundImageUrl READ backgroundImageUrl)
    Q_PROPERTY(QString backgroundVideoUrl READ backgroundVideoUrl)
    Q_PROPERTY(Cover cover READ cover)
    Q_PROPERTY(Cover coverWithoutText READ coverWithoutText)
    Q_PROPERTY(QDateTime created READ created)
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(QString descriptionFormatted READ descriptionFormatted)
    Q_PROPERTY(int durationMs READ durationMs)
    Q_PROPERTY(bool everPlayed READ everPlayed)
    Q_PROPERTY(QString generatedPlaylistType READ generatedPlaylistType)
    Q_PROPERTY(QString idForFrom READ idForFrom)
    Q_PROPERTY(bool isBanner READ isBanner)
    Q_PROPERTY(bool isPremiere READ isPremiere)
    Q_PROPERTY(int kind READ kind)
    // TODO: add madeFor
    Q_PROPERTY(QDateTime modified READ modified)
    Q_PROPERTY(QString ogImage READ ogImage)
    // TODO: add owner
    Q_PROPERTY(QString playlistUuid READ playlistUuid)
    Q_PROPERTY(int revision READ revision)
    Q_PROPERTY(int snapshot READ snapshot)
    Q_PROPERTY(QStringList tags READ tags)
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(int trackCount READ trackCount)
    // TODO: add tracks

public:
    explicit Playlist(QJsonObject object, QObject* parent = nullptr);

    bool available() const;
    const QString& backgroundImageUrl() const;
    const QString& backgroundVideoUrl() const;
    const Cover& cover() const;
    const Cover& coverWithoutText() const;
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

private:
    bool m_available;
    QString m_backgroundImageUrl;
    QString m_backgroundVideoUrl;
    Cover m_cover;
    Cover m_coverWithoutText;
    QDateTime m_created;
    QString m_description;
    QString m_descriptionFormatted;
    int m_durationMs;
    bool m_everPlayed;
    QString m_generatedPlaylistType;
    QString m_idForFrom;
    bool m_isBanner;
    bool m_isPremiere;
    int m_kind;
    QDateTime m_modified;
    QString m_ogImage;
    QString m_playlistUuid;
    int m_revision;
    int m_snapshot;
    QStringList m_tags;
    QString m_title;
    int m_trackCount;
};

#endif // PLAYLIST_H
