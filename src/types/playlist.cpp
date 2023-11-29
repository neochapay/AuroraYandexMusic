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

#include "playlist.h"

Playlist::Playlist(QJsonObject object, QObject* parent)
    : QObject(parent)
    , m_cover(Cover(object.take("cover").toObject()))
    , m_coverWithoutText(Cover(object.take("coverWithoutText").toObject()))
{
    m_available = object.take("available").toBool();
    m_backgroundImageUrl = object.take("backgroundImageUrl").toString();
    m_backgroundVideoUrl = object.take("backgroundVideoUrl").toString();
    m_created = QDateTime::fromString(object.take("created").toString());
    m_description = object.take("description").toString();
    m_descriptionFormatted = object.take("descriptionFormatted").toString();
    m_durationMs = object.take("durationMs").toInt();
    m_everPlayed = object.take("everPlayed").toBool();
    m_generatedPlaylistType = object.take("generatedPlaylistType").toString();
    m_idForFrom = object.take("idForFrom").toString();
    m_isBanner = object.take("isBanner").toBool();
    m_isPremiere = object.take("isPremiere").toBool();
    m_kind = object.take("kind").toInt();
    m_modified = QDateTime::fromString(object.take("modified").toString());
    m_ogImage = object.take("ogImage").toString();
    m_playlistUuid = object.take("playlistUuid").toString();
    m_revision = object.take("revision").toInt();
    m_snapshot = object.take("snapshot").toInt();
    // m_tags = object.take("tags").toArray();
    m_title = object.take("title").toString();
    m_trackCount = object.take("trackCount").toInt();
}

bool Playlist::available() const
{
    return m_available;
}

const QString& Playlist::backgroundImageUrl() const
{
    return m_backgroundImageUrl;
}

const QString& Playlist::backgroundVideoUrl() const
{
    return m_backgroundVideoUrl;
}

const Cover& Playlist::cover() const
{
    return m_cover;
}

const Cover& Playlist::coverWithoutText() const
{
    return m_coverWithoutText;
}

const QDateTime& Playlist::created() const
{
    return m_created;
}

const QString& Playlist::description() const
{
    return m_description;
}

const QString& Playlist::descriptionFormatted() const
{
    return m_descriptionFormatted;
}

int Playlist::durationMs() const
{
    return m_durationMs;
}

bool Playlist::everPlayed() const
{
    return m_everPlayed;
}

const QString& Playlist::generatedPlaylistType() const
{
    return m_generatedPlaylistType;
}

const QString& Playlist::idForFrom() const
{
    return m_idForFrom;
}

bool Playlist::isBanner() const
{
    return m_isBanner;
}

bool Playlist::isPremiere() const
{
    return m_isPremiere;
}

int Playlist::kind() const
{
    return m_kind;
}

const QDateTime& Playlist::modified() const
{
    return m_modified;
}

const QString& Playlist::ogImage() const
{
    return m_ogImage;
}

const QString& Playlist::playlistUuid() const
{
    return m_playlistUuid;
}

int Playlist::revision() const
{
    return m_revision;
}

int Playlist::snapshot() const
{
    return m_snapshot;
}

const QStringList& Playlist::tags() const
{
    return m_tags;
}

const QString& Playlist::title() const
{
    return m_title;
}

int Playlist::trackCount() const
{
    return m_trackCount;
}
