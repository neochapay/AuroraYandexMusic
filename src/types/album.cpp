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

#include "album.h"

#include <QJsonArray>
#include <QJsonValueRef>

Album::Album(QObject* parent)
    : QObject(parent)
    , d_ptr(new AlbumPrivate())
{
}

Album::Album(const Album& other, QObject* parent)
    : QObject(parent)
    , d_ptr(other.d_ptr)
{
}

Album::Album(QJsonObject object, QObject* parent)
    : QObject(parent)
    , d_ptr(new AlbumPrivate())
{
    for (const QJsonValue& v : object.take("artists").toArray()) {
        d_ptr->artists.push_back(Artist(v.toObject()));
    }

    d_ptr->available = object.take("available").toBool();
    d_ptr->availableForMobile = object.take("availableForMobile").toBool();

    for (const QJsonValue& v : object.take("availableForOptions").toArray()) {
        d_ptr->availableForOptions.push_back(v.toString());
    }

    d_ptr->availableForPremiumUsers = object.take("availableForPremiumUsers").toBool();
    d_ptr->availablePartially = object.take("availablePartially").toBool();

    for (const QJsonValue& v : object.take("bests").toArray()) {
        d_ptr->bests.push_back(v.toInt());
    }

    d_ptr->contentWarning = object.take("contentWarning").toString();
    d_ptr->coverUri = object.take("coverUri").toString();

    for (const QJsonValue& v : object.take("disclaimers").toArray()) {
        d_ptr->disclaimers.push_back(v.toString());
    }

    d_ptr->genere = object.take("genere").toString();
    d_ptr->albumId = object.take("albumId").toInt();
    // QList<Label> labels;
    d_ptr->likesConut = object.take("likesConut").toInt();
    d_ptr->metaType = object.take("metaType").toString();
    d_ptr->orImage = object.take("orImage").toString();
    d_ptr->recent = object.take("recent").toBool();
    d_ptr->releaseDate = QDateTime::fromString(object.take("releaseDate").toString());
    d_ptr->title = object.take("title").toString();
    d_ptr->trackCount = object.take("trackCount").toInt();
    d_ptr->trackPositionIndex = object.take("trackPosition").toObject().take("index").toInt();
    d_ptr->trackPositionVolume = object.take("trackPosition").toObject().take("volume").toInt();
    d_ptr->year = object.take("year").toInt();
}

Album::~Album()
{
    delete d_ptr;
}

Album& Album::operator=(const Album& other)
{
    *d_ptr = *other.d_ptr;
    return *this;
}

const QList<Artist>& Album::artists() const
{
    return d_ptr->artists;
}

bool Album::available() const
{
    return d_ptr->available;
}

bool Album::availableForMobile() const
{
    return d_ptr->availableForMobile;
}

const QStringList& Album::availableForOptions() const
{
    return d_ptr->availableForOptions;
}

bool Album::availableForPremiumUsers() const
{
    return d_ptr->availableForPremiumUsers;
}

bool Album::availablePartially() const
{
    return d_ptr->availablePartially;
}

const QList<int>& Album::bests() const
{
    return d_ptr->bests;
}

const QString& Album::contentWarning() const
{
    return d_ptr->contentWarning;
}

const QString& Album::coverUri() const
{
    return d_ptr->coverUri;
}

const QStringList& Album::disclaimers() const
{
    return d_ptr->disclaimers;
}

const QString& Album::genere() const
{
    return d_ptr->genere;
}

int Album::albumId() const
{
    return d_ptr->albumId;
}

const QList<Label> Album::labels() const
{
    return d_ptr->labels;
}

int Album::likesConut() const
{
    return d_ptr->likesConut;
}

const QString& Album::metaType() const
{
    return d_ptr->metaType;
}

const QString& Album::orImage() const
{
    return d_ptr->orImage;
}

bool Album::recent() const
{
    return d_ptr->recent;
}

const QDateTime& Album::releaseDate() const
{
    return d_ptr->releaseDate;
}

const QString& Album::title() const
{
    return d_ptr->title;
}

int Album::trackCount() const
{
    return d_ptr->trackCount;
}

int Album::trackPositionIndex() const
{
    return d_ptr->trackPositionIndex;
}

int Album::trackPositionVolume() const
{
    return d_ptr->trackPositionVolume;
}

int Album::year() const
{
    return d_ptr->year;
}
