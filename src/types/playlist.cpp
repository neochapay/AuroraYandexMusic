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

Playlist::Playlist(const Playlist& other, QObject* parent)
    : QObject(parent)
    , d_ptr(other.d_ptr)
{
}

Playlist::Playlist(QJsonObject object, QObject* parent)
    : QObject(parent)
    , d_ptr(new PlaylistPrivate())
{
    d_ptr->available = object.take("available").toBool();
    d_ptr->backgroundImageUrl = object.take("backgroundImageUrl").toString();
    d_ptr->backgroundVideoUrl = object.take("backgroundVideoUrl").toString();
    d_ptr->cover = Cover(object.take("cover").toObject());
    d_ptr->coverWithoutText = Cover(object.take("coverWithoutText").toObject());
    d_ptr->created = QDateTime::fromString(object.take("created").toString());
    d_ptr->description = object.take("description").toString();
    d_ptr->descriptionFormatted = object.take("descriptionFormatted").toString();
    d_ptr->durationMs = object.take("durationMs").toInt();
    d_ptr->everPlayed = object.take("everPlayed").toBool();
    d_ptr->generatedPlaylistType = object.take("generatedPlaylistType").toString();
    d_ptr->idForFrom = object.take("idForFrom").toString();
    d_ptr->isBanner = object.take("isBanner").toBool();
    d_ptr->isPremiere = object.take("isPremiere").toBool();
    d_ptr->kind = object.take("kind").toInt();
    d_ptr->modified = QDateTime::fromString(object.take("modified").toString());
    d_ptr->ogImage = object.take("ogImage").toString();
    d_ptr->playlistUuid = object.take("playlistUuid").toString();
    d_ptr->revision = object.take("revision").toInt();
    d_ptr->snapshot = object.take("snapshot").toInt();
    // d_ptr->tags = object.take("tags").toArray();
    d_ptr->title = object.take("title").toString();
    d_ptr->trackCount = object.take("trackCount").toInt();
}

Playlist::~Playlist()
{
    delete d_ptr;
}

Playlist& Playlist::operator=(const Playlist& other)
{
    *d_ptr = *other.d_ptr;
    return *this;
}

bool Playlist::available() const
{
    return d_ptr->available;
}

const QString& Playlist::backgroundImageUrl() const
{
    return d_ptr->backgroundImageUrl;
}

const QString& Playlist::backgroundVideoUrl() const
{
    return d_ptr->backgroundVideoUrl;
}

const Cover& Playlist::cover() const
{
    return d_ptr->cover;
}

const Cover& Playlist::coverWithoutText() const
{
    return d_ptr->coverWithoutText;
}

const QDateTime& Playlist::created() const
{
    return d_ptr->created;
}

const QString& Playlist::description() const
{
    return d_ptr->description;
}

const QString& Playlist::descriptionFormatted() const
{
    return d_ptr->descriptionFormatted;
}

int Playlist::durationMs() const
{
    return d_ptr->durationMs;
}

bool Playlist::everPlayed() const
{
    return d_ptr->everPlayed;
}

const QString& Playlist::generatedPlaylistType() const
{
    return d_ptr->generatedPlaylistType;
}

const QString& Playlist::idForFrom() const
{
    return d_ptr->idForFrom;
}

bool Playlist::isBanner() const
{
    return d_ptr->isBanner;
}

bool Playlist::isPremiere() const
{
    return d_ptr->isPremiere;
}

int Playlist::kind() const
{
    return d_ptr->kind;
}

const QDateTime& Playlist::modified() const
{
    return d_ptr->modified;
}

const QString& Playlist::ogImage() const
{
    return d_ptr->ogImage;
}

const QString& Playlist::playlistUuid() const
{
    return d_ptr->playlistUuid;
}

int Playlist::revision() const
{
    return d_ptr->revision;
}

int Playlist::snapshot() const
{
    return d_ptr->snapshot;
}

const QStringList& Playlist::tags() const
{
    return d_ptr->tags;
}

const QString& Playlist::title() const
{
    return d_ptr->title;
}

int Playlist::trackCount() const
{
    return d_ptr->trackCount;
}
