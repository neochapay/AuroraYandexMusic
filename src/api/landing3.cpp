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

#include "landing3.h"
#include "../types/album.h"
#include "../types/artist.h"
#include "../types/playlist.h"
#include "../types/track.h"
#include "request.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QUrlQuery>

Landing3::Landing3(QObject* parent)
    : QObject(parent)
{
}

void Landing3::get(const QString &blocks)
{
    QUrlQuery query;
    if (!blocks.isEmpty()) {
        query.addQueryItem(QStringLiteral("blocks"), blocks);
    }

    m_gettingBlocks = blocks;

    Request* request = new Request("/landing3");
    connect(request, &Request::dataReady, this, &Landing3::getLandingRequestHandler);
    request->get(query);
}

void Landing3::getLanding(const QString &blocks)
{
    get(blocks.isEmpty() ? QStringLiteral("personalplaylists,promotions,new-releases,new-playlists,mixes,chart,artists,albums,playlists,play_contexts,podcasts") : blocks);
}

void Landing3::getLandingBlock(const QString &blockName)
{
    if (blockName.isEmpty()) {
        return;
    }
    m_requestedBlock = blockName;
    Request* request = new Request("/landing3/" + blockName);
    connect(request, &Request::dataReady, this, &Landing3::loadingBlockRequestHandler);
    request->get();
}

void Landing3::loadingBlock(const QString &blockName)
{
    // compatibility alias; route known dedicated endpoints
    if (blockName == QStringLiteral("new-releases")) {
        getNewReleases();
        return;
    }
    if (blockName == QStringLiteral("podcasts")) {
        getNewPodcasts();
        return;
    }
    if (blockName == QStringLiteral("new-playlists")) {
        getNewPlaylists();
        return;
    }
    if (blockName == QStringLiteral("chart")) {
        getChart(QStringLiteral("russia"));
        return;
    }
    getLandingBlock(blockName);
}

void Landing3::getNewReleases()
{
    m_requestedBlock = QStringLiteral("new-releases");
    Request* request = new Request("/landing3/new-releases");
    connect(request, &Request::dataReady, this, &Landing3::newReleasesRequestHandler);
    request->get();
}

void Landing3::getNewPodcasts()
{
    m_requestedBlock = QStringLiteral("podcasts");
    Request* request = new Request("/landing3/podcasts");
    connect(request, &Request::dataReady, this, &Landing3::newPodcastsRequestHandler);
    request->get();
}

void Landing3::getNewPlaylists()
{
    m_requestedBlock = QStringLiteral("new-playlists");
    Request* request = new Request("/landing3/new-playlists");
    connect(request, &Request::dataReady, this, &Landing3::newPlaylistsRequestHandler);
    request->get();
}

void Landing3::getChart(const QString &chartType)
{
    m_requestedChartType = chartType.isEmpty() ? QStringLiteral("russia") : chartType;
    Request* request = new Request("/landing3/chart/" + m_requestedChartType);
    connect(request, &Request::dataReady, this, &Landing3::chartRequestHandler);
    request->get();
}

void Landing3::chart(const QString &chartType)
{
    getChart(chartType);
}

QObject* Landing3::parseEntity(const QString &blockType, const QJsonValue &entityValue) const
{
    const QJsonObject entitiesObject = entityValue.toObject();
    const QString entityType = entitiesObject.value(QStringLiteral("type")).toString();
    QJsonObject data = entitiesObject.value(QStringLiteral("data")).toObject();

    // Some blocks wrap payload as data.data
    if (data.contains(QStringLiteral("data")) && data.value(QStringLiteral("data")).isObject()) {
        const QJsonObject inner = data.value(QStringLiteral("data")).toObject();
        if (inner.contains(QStringLiteral("kind")) || inner.contains(QStringLiteral("title")) || inner.contains(QStringLiteral("trackCount"))) {
            data = inner;
        }
    }

    auto makePlaylist = [](const QJsonObject &o) -> QObject* {
        Playlist* playlist = new Playlist(o);
        if (playlist && (!playlist->title().isEmpty() || playlist->kind() != 0)) {
            return playlist;
        }
        delete playlist;
        return nullptr;
    };
    auto makeAlbum = [](const QJsonObject &o) -> QObject* {
        Album* album = new Album(o);
        if (album && album->albumId() != 0) {
            return album;
        }
        delete album;
        return nullptr;
    };
    auto makeTrack = [](const QJsonObject &o) -> QObject* {
        Track* track = new Track(o);
        if (track && track->trackId() != 0) {
            return track;
        }
        delete track;
        return nullptr;
    };
    auto makeArtist = [](const QJsonObject &o) -> QObject* {
        Artist* artist = new Artist(o);
        if (artist) {
            return artist;
        }
        delete artist;
        return nullptr;
    };

    if (entityType == QStringLiteral("playlist") || blockType.contains(QStringLiteral("playlist")) || blockType == QStringLiteral("mixes") || blockType == QStringLiteral("play_contexts")) {
        if (QObject* p = makePlaylist(data)) return p;
    }
    if (entityType == QStringLiteral("album") || blockType == QStringLiteral("new-releases") || blockType == QStringLiteral("albums")) {
        if (QObject* p = makeAlbum(data)) return p;
    }
    if (entityType == QStringLiteral("track") || blockType == QStringLiteral("chart")) {
        QJsonObject trackObj = data;
        if (trackObj.contains(QStringLiteral("track")) && trackObj.value(QStringLiteral("track")).isObject()) {
            trackObj = trackObj.value(QStringLiteral("track")).toObject();
        }
        if (QObject* p = makeTrack(trackObj)) return p;
        if (QObject* p = makeAlbum(data)) return p;
    }
    if (entityType == QStringLiteral("artist") || blockType == QStringLiteral("artists")) {
        if (QObject* p = makeArtist(data)) return p;
    }

    // Fallback attempts
    if (data.contains(QStringLiteral("kind")) && (data.contains(QStringLiteral("owner")) || data.contains(QStringLiteral("trackCount")))) {
        if (QObject* p = makePlaylist(data)) return p;
    }
    if (data.contains(QStringLiteral("id")) && data.contains(QStringLiteral("title")) && data.contains(QStringLiteral("artists"))) {
        if (QObject* p = makeAlbum(data)) return p;
    }
    if (data.contains(QStringLiteral("trackId")) || (data.contains(QStringLiteral("id")) && data.contains(QStringLiteral("durationMs")))) {
        if (QObject* p = makeTrack(data)) return p;
    }

    return nullptr;
}

QList<QObject*> Landing3::parseBlocks(const QJsonObject &landingResult) const
{
    QList<QObject*> blocks;
    const QJsonArray arr = landingResult.value(QStringLiteral("blocks")).toArray();
    for (const QJsonValue &vBlock : arr) {
        const QJsonObject valueObject = vBlock.toObject();

        LandingResultBlock* block = new LandingResultBlock();
        block->type = valueObject.value(QStringLiteral("type")).toString();
        block->title = valueObject.value(QStringLiteral("title")).toString();
        block->description = valueObject.value(QStringLiteral("description")).toString();

        const QJsonArray entities = valueObject.value(QStringLiteral("entities")).toArray();
        for (const QJsonValue &entitiesValue : entities) {
            if (QObject* obj = parseEntity(block->type, entitiesValue)) {
                block->entities.push_back(obj);
            }
        }

        blocks.push_back(block);
    }
    return blocks;
}

void Landing3::getLandingRequestHandler(QJsonValue value)
{
    QJsonObject obj = value.toObject();
    // Request may pass result object directly
    if (obj.contains(QStringLiteral("result")) && obj.value(QStringLiteral("result")).isObject()) {
        obj = obj.value(QStringLiteral("result")).toObject();
    }
    QList<QObject*> blocks = parseBlocks(obj);
    emit landingBlocksReady(blocks);
    emit landingReady(obj, blocks);
}

void Landing3::loadingBlockRequestHandler(QJsonValue value)
{
    emit landingBlockReady(m_requestedBlock, value);
}

void Landing3::newReleasesRequestHandler(QJsonValue value)
{
    emit newReleasesReady(value);
    emit landingBlockReady(QStringLiteral("new-releases"), value);
}

void Landing3::newPodcastsRequestHandler(QJsonValue value)
{
    emit newPodcastsReady(value);
    emit landingBlockReady(QStringLiteral("podcasts"), value);
}

void Landing3::newPlaylistsRequestHandler(QJsonValue value)
{
    emit newPlaylistsReady(value);
    emit landingBlockReady(QStringLiteral("new-playlists"), value);
}

void Landing3::chartRequestHandler(QJsonValue value)
{
    emit chartReady(m_requestedChartType, value);
    emit landingBlockReady(QStringLiteral("chart"), value);
}

