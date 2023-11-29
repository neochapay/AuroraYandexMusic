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

Artist::Artist(QObject* parent)
    : QObject(parent)
{
}

Artist::Artist(QJsonObject object, QObject* parent)
    : QObject(parent)
{
    m_composer = object.take("composer").toBool();
    m_coverPrefix = object.take("cover").toObject().take("prefix").toString();
    m_coverType = object.take("cover").toObject().take("type").toString();
    m_coverUri = object.take("cover").toObject().take("uri").toString();
    // m_disclaimers = object.take("disclaimers").toArray();
    // m_genres = object.take("genres").toArray();
    m_name = object.take("name").toString();
    m_artistId = object.take("artistId").toInt();
    m_various = object.take("various").toBool();
}

bool Artist::composer() const
{
    return m_composer;
}

void Artist::setComposer(bool newComposer)
{
    if (m_composer == newComposer)
        return;
    m_composer = newComposer;
    emit composerChanged();
}

const QString& Artist::coverPrefix() const
{
    return m_coverPrefix;
}

void Artist::setCoverPrefix(const QString& newCoverPrefix)
{
    if (m_coverPrefix == newCoverPrefix)
        return;
    m_coverPrefix = newCoverPrefix;
    emit coverPrefixChanged();
}

const QString& Artist::coverType() const
{
    return m_coverType;
}

void Artist::setCoverType(const QString& newCoverType)
{
    if (m_coverType == newCoverType)
        return;
    m_coverType = newCoverType;
    emit coverTypeChanged();
}

const QString& Artist::coverUri() const
{
    return m_coverUri;
}

void Artist::setCoverUri(const QString& newCoverUri)
{
    if (m_coverUri == newCoverUri)
        return;
    m_coverUri = newCoverUri;
    emit coverUriChanged();
}

const QStringList& Artist::disclaimers() const
{
    return m_disclaimers;
}

void Artist::setDisclaimers(const QStringList& newDisclaimers)
{
    if (m_disclaimers == newDisclaimers)
        return;
    m_disclaimers = newDisclaimers;
    emit disclaimersChanged();
}

const QStringList& Artist::genres() const
{
    return m_genres;
}

void Artist::setGenres(const QStringList& newGenres)
{
    if (m_genres == newGenres)
        return;
    m_genres = newGenres;
    emit genresChanged();
}

const QString& Artist::name() const
{
    return m_name;
}

void Artist::setName(const QString& newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}

int Artist::artistId() const
{
    return m_artistId;
}

void Artist::setArtistId(int newArtistId)
{
    if (m_artistId == newArtistId)
        return;
    m_artistId = newArtistId;
    emit artistIdChanged();
}

bool Artist::various() const
{
    return m_various;
}

void Artist::setVarious(bool newVarious)
{
    if (m_various == newVarious)
        return;
    m_various = newVarious;
    emit variousChanged();
}
