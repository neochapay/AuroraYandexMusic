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

#include "artist.h"
#include <QJsonArray>

Artist::Artist(QObject* parent)
    : QObject(parent)
    , d_ptr(new ArtistPrivate())
{
}

Artist::Artist(const Artist& other, QObject* parent)
    : QObject(parent)
    , d_ptr(other.d_ptr)
{
}

Artist::Artist(QJsonObject object, QObject* parent)
    : QObject(parent)
    , d_ptr(new ArtistPrivate())
{
    d_ptr->composer = object.value("composer").toBool();
    d_ptr->coverPrefix = object.value("cover").toObject().take("prefix").toString();
    d_ptr->coverType = object.value("cover").toObject().take("type").toString();
    d_ptr->coverUri = object.value("cover").toObject().take("uri").toString();

    for(const QJsonValue &v : object.value("disclaimers").toArray()) {
        d_ptr->disclaimers.push_back(v.toString());
    }

    for(const QJsonValue &v : object.value("genres").toArray()) {
        d_ptr->genres.push_back(v.toString());
    }

    d_ptr->countsTracks = object.value("counts").toObject().take("tracks").toInt();
    d_ptr->countsDirectAlbums = object.value("counts").toObject().take("directAlbums").toInt();
    d_ptr->countsAlsoAlbums = object.value("counts").toObject().take("alsoAlbums").toInt();
    d_ptr->countsAlsoTracks = object.value("counts").toObject().take("alsoTracks").toInt();
    d_ptr->likesCount = object.value("likesCount").toInt();
    d_ptr->ogImage = object.value("ogImage").toString();

    d_ptr->name = object.value("name").toString();
    d_ptr->artistId = object.value("id").toInt();
    d_ptr->various = object.value("various").toBool();
    d_ptr->foreignAgent = false;
    for (const QJsonValue &v : object.value("disclaimers").toArray()) {
        if(v.toString() == "foreignAgent") {
            d_ptr->foreignAgent = true;
            break;
        }
    }
}

Artist::~Artist()
{
    delete d_ptr;
}

Artist& Artist::operator=(const Artist& other)
{
    *d_ptr = *other.d_ptr;
    return *this;
}

bool Artist::operator !=(const Artist &other)
{
    return d_ptr->artistId != other.artistId();
}

bool Artist::composer() const
{
    return d_ptr->composer;
}

const QString& Artist::coverPrefix() const
{
    return d_ptr->coverPrefix;
}

const QString& Artist::coverType() const
{
    return d_ptr->coverType;
}

const QString& Artist::coverUri() const
{
    return d_ptr->coverUri;
}

const QStringList& Artist::disclaimers() const
{
    return d_ptr->disclaimers;
}

const QStringList& Artist::genres() const
{
    return d_ptr->genres;
}

const QString& Artist::name() const
{
    return d_ptr->name;
}

int Artist::artistId() const
{
    return d_ptr->artistId;
}

bool Artist::various() const
{
    return d_ptr->various;
}

bool Artist::foreignAgent() const
{
    return d_ptr->foreignAgent;
}

int Artist::countsTracks() const
{
    return d_ptr->countsTracks;
}

int Artist::countsDirectAlbums() const
{
    return d_ptr->countsDirectAlbums;
}

int Artist::countsAlsoAlbums() const
{
    return d_ptr->countsAlsoAlbums;
}

int Artist::countsAlsoTracks() const
{
    return d_ptr->countsAlsoTracks;
}

int Artist::likesCount() const
{
    return d_ptr->likesCount;
}

const QString &Artist::ogImage() const
{
    return d_ptr->ogImage;
}
