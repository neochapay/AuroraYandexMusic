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

Playlist::Playlist(QObject* parent)
    : QObject(parent)
{
}

Playlist::Playlist(QJsonObject object, QObject* parent)
    : QObject(parent)
{
    m_available = object.take("available").toBool();
    m_backgroundImageUrl = object.take("backgroundImageUrl").toString();
    m_backgroundVideoUrl = object.take("backgroundVideoUrl").toString();
    m_cover = Cover(object.take("cover").toObject());
    m_coverWithoutText = Cover(object.take("coverWithoutText").toObject());
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

void Playlist::setAvailable(bool newAvailable)
{
    if (m_available == newAvailable)
        return;
    m_available = newAvailable;
    emit availableChanged();
}

const QString& Playlist::backgroundImageUrl() const
{
    return m_backgroundImageUrl;
}

void Playlist::setBackgroundImageUrl(const QString& newBackgroundImageUrl)
{
    if (m_backgroundImageUrl == newBackgroundImageUrl)
        return;
    m_backgroundImageUrl = newBackgroundImageUrl;
    emit backgroundImageUrlChanged();
}

const QString& Playlist::backgroundVideoUrl() const
{
    return m_backgroundVideoUrl;
}

void Playlist::setBackgroundVideoUrl(const QString& newBackgroundVideoUrl)
{
    if (m_backgroundVideoUrl == newBackgroundVideoUrl)
        return;
    m_backgroundVideoUrl = newBackgroundVideoUrl;
    emit backgroundVideoUrlChanged();
}

const Cover& Playlist::cover() const
{
    return m_cover;
}

void Playlist::setCover(const Cover& newCover)
{
    if (m_cover == newCover)
        return;
    m_cover = newCover;
    emit coverChanged();
}

const Cover& Playlist::coverWithoutText() const
{
    return m_coverWithoutText;
}

void Playlist::setCoverWithoutText(const Cover& newCoverWithoutText)
{
    if (m_coverWithoutText == newCoverWithoutText)
        return;
    m_coverWithoutText = newCoverWithoutText;
    emit coverWithoutTextChanged();
}

const QDateTime& Playlist::created() const
{
    return m_created;
}

void Playlist::setCreated(const QDateTime& newCreated)
{
    if (m_created == newCreated)
        return;
    m_created = newCreated;
    emit createdChanged();
}

const QString& Playlist::description() const
{
    return m_description;
}

void Playlist::setDescription(const QString& newDescription)
{
    if (m_description == newDescription)
        return;
    m_description = newDescription;
    emit descriptionChanged();
}

const QString& Playlist::descriptionFormatted() const
{
    return m_descriptionFormatted;
}

void Playlist::setDescriptionFormatted(const QString& newDescriptionFormatted)
{
    if (m_descriptionFormatted == newDescriptionFormatted)
        return;
    m_descriptionFormatted = newDescriptionFormatted;
    emit descriptionFormattedChanged();
}

int Playlist::durationMs() const
{
    return m_durationMs;
}

void Playlist::setDurationMs(int newDurationMs)
{
    if (m_durationMs == newDurationMs)
        return;
    m_durationMs = newDurationMs;
    emit durationMsChanged();
}

bool Playlist::everPlayed() const
{
    return m_everPlayed;
}

void Playlist::setEverPlayed(bool newEverPlayed)
{
    if (m_everPlayed == newEverPlayed)
        return;
    m_everPlayed = newEverPlayed;
    emit everPlayedChanged();
}

const QString& Playlist::generatedPlaylistType() const
{
    return m_generatedPlaylistType;
}

void Playlist::setGeneratedPlaylistType(const QString& newGeneratedPlaylistType)
{
    if (m_generatedPlaylistType == newGeneratedPlaylistType)
        return;
    m_generatedPlaylistType = newGeneratedPlaylistType;
    emit generatedPlaylistTypeChanged();
}

const QString& Playlist::idForFrom() const
{
    return m_idForFrom;
}

void Playlist::setIdForFrom(const QString& newIdForFrom)
{
    if (m_idForFrom == newIdForFrom)
        return;
    m_idForFrom = newIdForFrom;
    emit idForFromChanged();
}

bool Playlist::isBanner() const
{
    return m_isBanner;
}

void Playlist::setIsBanner(bool newIsBanner)
{
    if (m_isBanner == newIsBanner)
        return;
    m_isBanner = newIsBanner;
    emit isBannerChanged();
}

bool Playlist::isPremiere() const
{
    return m_isPremiere;
}

void Playlist::setIsPremiere(bool newIsPremiere)
{
    if (m_isPremiere == newIsPremiere)
        return;
    m_isPremiere = newIsPremiere;
    emit isPremiereChanged();
}

int Playlist::kind() const
{
    return m_kind;
}

void Playlist::setKind(int newKind)
{
    if (m_kind == newKind)
        return;
    m_kind = newKind;
    emit kindChanged();
}

const QDateTime& Playlist::modified() const
{
    return m_modified;
}

void Playlist::setModified(const QDateTime& newModified)
{
    if (m_modified == newModified)
        return;
    m_modified = newModified;
    emit modifiedChanged();
}

const QString& Playlist::ogImage() const
{
    return m_ogImage;
}

void Playlist::setOgImage(const QString& newOgImage)
{
    if (m_ogImage == newOgImage)
        return;
    m_ogImage = newOgImage;
    emit ogImageChanged();
}

const QString& Playlist::playlistUuid() const
{
    return m_playlistUuid;
}

void Playlist::setPlaylistUuid(const QString& newPlaylistUuid)
{
    if (m_playlistUuid == newPlaylistUuid)
        return;
    m_playlistUuid = newPlaylistUuid;
    emit playlistUuidChanged();
}

int Playlist::revision() const
{
    return m_revision;
}

void Playlist::setRevision(int newRevision)
{
    if (m_revision == newRevision)
        return;
    m_revision = newRevision;
    emit revisionChanged();
}

int Playlist::snapshot() const
{
    return m_snapshot;
}

void Playlist::setSnapshot(int newSnapshot)
{
    if (m_snapshot == newSnapshot)
        return;
    m_snapshot = newSnapshot;
    emit snapshotChanged();
}

const QStringList& Playlist::tags() const
{
    return m_tags;
}

void Playlist::setTags(const QStringList& newTags)
{
    if (m_tags == newTags)
        return;
    m_tags = newTags;
    emit tagsChanged();
}

const QString& Playlist::title() const
{
    return m_title;
}

void Playlist::setTitle(const QString& newTitle)
{
    if (m_title == newTitle)
        return;
    m_title = newTitle;
    emit titleChanged();
}

int Playlist::trackCount() const
{
    return m_trackCount;
}

void Playlist::setTrackCount(int newTrackCount)
{
    if (m_trackCount == newTrackCount)
        return;
    m_trackCount = newTrackCount;
    emit trackCountChanged();
}
