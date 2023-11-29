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

#include "track.h"
#include <QJsonArray>
#include <QJsonValue>

Track::Track(QObject* parent)
    : QObject(parent)
    , d_ptr(new TrackPrivate())
{
}

Track::Track(const Track& other, QObject* parent)
    : QObject(parent)
    , d_ptr(other.d_ptr)
{
}

Track::Track(QJsonObject object, QObject* parent)
    : QObject(parent)
    , d_ptr(new TrackPrivate())
{
    for (const QJsonValue& v : object.take("albuns").toArray()) {
        d_ptr->albums.push_back(Album(v.toObject()));
    }

    for (const QJsonValue& v : object.take("artists").toArray()) {
        d_ptr->artists.push_back(Artist(v.toObject()));
    }

    d_ptr->available = object.take("available").toBool();

    for (const QJsonValue& v : object.take("availableForOptions").toArray()) {
        d_ptr->availableForOptions.push_back(v.toString());
    }

    d_ptr->availableForPremiumUsers = object.take("availableForPremiumUsers").toBool();
    d_ptr->availableFullWithoutPermission = object.take("availableFullWithoutPermission").toBool();
    d_ptr->coverUri = object.take("coverUri").toString();

    DerivedColors derivedColors;
    derivedColors.accent = object.take("derivedColors").toObject().take("accent").toString();
    derivedColors.average = object.take("derivedColors").toObject().take("average").toString();
    derivedColors.miniPlayer = object.take("derivedColors").toObject().take("miniPlayer").toString();
    derivedColors.waveText = object.take("derivedColors").toObject().take("waveText").toString();

    for (const QJsonValue& v : object.take("disclaimers").toArray()) {
        d_ptr->disclaimers.push_back(v.toString());
    }

    d_ptr->durationMs = object.take("durationMs").toInt();

    Fade fade;
    fade.inStart = object.take("fade").toObject().take("inStart").toDouble();
    fade.inStop = object.take("fade").toObject().take("inStop").toDouble();
    fade.outStart = object.take("fade").toObject().take("outStart").toDouble();
    fade.outStop = object.take("fade").toObject().take("outStop").toDouble();
    d_ptr->fade = fade;

    d_ptr->fileSize = object.take("fileSize").toInt();
    d_ptr->trackId = object.take("trackId").toInt();
    d_ptr->lyricsAvailable = object.take("lyricsAvailable").toBool();
    d_ptr->ogImage = object.take("ogImage").toString();
    d_ptr->previewDurationMs = object.take("previewDurationMs").toInt();
    d_ptr->realId = object.take("realId").toInt();
    d_ptr->rememberPosition = object.take("rememberPosition").toBool();
    d_ptr->storageDir = object.take("storageDir").toString();
    d_ptr->title = object.take("title").toString();
    d_ptr->trackSharingFlag = object.take("trackSharingFlag").toString();
    d_ptr->trackSource = object.take("trackSource").toString();
    d_ptr->type = object.take("type").toString();
    d_ptr->contentWarning = object.take("contentWarning").toString();
}

Track::~Track()
{
    delete d_ptr;
}

Track& Track::operator=(const Track& other)
{
    *d_ptr = *other.d_ptr;
    return *this;
}

const QList<Album>& Track::albums() const
{
    return d_ptr->albums;
}

const QList<Artist>& Track::artists() const
{
    return d_ptr->artists;
}

bool Track::available() const
{
    return d_ptr->available;
}

const QStringList& Track::availableForOptions() const
{
    return d_ptr->availableForOptions;
}

bool Track::availableForPremiumUsers() const
{
    return d_ptr->availableForPremiumUsers;
}

bool Track::availableFullWithoutPermission() const
{
    return d_ptr->availableFullWithoutPermission;
}

const QString& Track::coverUri() const
{
    return d_ptr->coverUri;
}

const DerivedColors& Track::derivedColors() const
{
    return d_ptr->derivedColors;
}

const QStringList& Track::disclaimers() const
{
    return d_ptr->disclaimers;
}

int Track::durationMs() const
{
    return d_ptr->durationMs;
}

const Fade& Track::fade() const
{
    return d_ptr->fade;
}

int Track::fileSize() const
{
    return d_ptr->fileSize;
}

int Track::trackId() const
{
    return d_ptr->trackId;
}

bool Track::lyricsAvailable() const
{
    return d_ptr->lyricsAvailable;
}

const QString& Track::ogImage() const
{
    return d_ptr->ogImage;
}

int Track::previewDurationMs() const
{
    return d_ptr->previewDurationMs;
}

int Track::realId() const
{
    return d_ptr->realId;
}

bool Track::rememberPosition() const
{
    return d_ptr->rememberPosition;
}

const QString& Track::storageDir() const
{
    return d_ptr->storageDir;
}

const QString& Track::title() const
{
    return d_ptr->title;
}

const QString& Track::trackSharingFlag() const
{
    return d_ptr->trackSharingFlag;
}

const QString& Track::trackSource() const
{
    return d_ptr->trackSource;
}

const QString& Track::type() const
{
    return d_ptr->type;
}

const QString& Track::contentWarning() const
{
    return d_ptr->contentWarning;
}
