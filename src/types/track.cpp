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
#include <QDebug>
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
    for (const QJsonValue& v : object.value("albums").toArray()) {
        Album* album = new Album(v.toObject());
        d_ptr->albums.push_back(album);
    }

    for (const QJsonValue& v : object.value("artists").toArray()) {
        Artist* artist = new Artist(v.toObject());
        d_ptr->artists.push_back(artist);
    }

    d_ptr->available = object.value("available").toBool();

    for (const QJsonValue& v : object.value("availableForOptions").toArray()) {
        d_ptr->availableForOptions.push_back(v.toString());
    }

    d_ptr->availableForPremiumUsers = object.value("availableForPremiumUsers").toBool();
    d_ptr->availableFullWithoutPermission = object.value("availableFullWithoutPermission").toBool();
    d_ptr->coverUri = object.value("coverUri").toString();

    DerivedColors derivedColors;
    derivedColors.accent = object.value("derivedColors").toObject().take("accent").toString();
    derivedColors.average = object.value("derivedColors").toObject().take("average").toString();
    derivedColors.miniPlayer = object.value("derivedColors").toObject().take("miniPlayer").toString();
    derivedColors.waveText = object.value("derivedColors").toObject().take("waveText").toString();
    d_ptr->derivedColors = derivedColors;

    for (const QJsonValue& v : object.value("disclaimers").toArray()) {
        d_ptr->disclaimers.push_back(v.toString());
    }

    d_ptr->durationMs = object.value("durationMs").toInt();

    Fade fade;
    fade.inStart = object.value("fade").toObject().take("inStart").toDouble();
    fade.inStop = object.value("fade").toObject().take("inStop").toDouble();
    fade.outStart = object.value("fade").toObject().take("outStart").toDouble();
    fade.outStop = object.value("fade").toObject().take("outStop").toDouble();
    d_ptr->fade = fade;

    d_ptr->fileSize = object.value("fileSize").toInt();
    d_ptr->trackId = object.value("id").toString();
    d_ptr->lyricsAvailable = object.value("lyricsAvailable").toBool();
    d_ptr->ogImage = object.value("ogImage").toString();
    d_ptr->previewDurationMs = object.value("previewDurationMs").toInt();
    d_ptr->realId = object.value("realId").toInt();
    d_ptr->rememberPosition = object.value("rememberPosition").toBool();
    d_ptr->storageDir = object.value("storageDir").toString();
    d_ptr->title = object.value("title").toString();
    d_ptr->trackSharingFlag = object.value("trackSharingFlag").toString();
    d_ptr->trackSource = object.value("trackSource").toString();
    d_ptr->type = object.value("type").toString();
    d_ptr->contentWarning = object.value("contentWarning").toString();
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

const QList<QObject*>& Track::albums() const
{
    return *reinterpret_cast<const QList<QObject*>*>(&d_ptr->albums);
}

const QList<QObject*>& Track::artists() const
{
    return *reinterpret_cast<const QList<QObject*>*>(&d_ptr->artists);
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

QString Track::trackId() const
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
