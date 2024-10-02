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
#include "../types/playlist.h"
#include "request.h"

#include <QUrlQuery>
#include <QJsonArray>

Landing3::Landing3(QObject* parent)
    : QObject(parent)
{
}

void Landing3::get(QString blocks)
{
    QUrlQuery query;
    query.addQueryItem("blocks", blocks);

    m_gettingBlocks = blocks;

    Request* getLandingRequest = new Request("/landing3");
    connect(getLandingRequest, &Request::dataReady, this, &Landing3::getLandingRequestHandler);
    getLandingRequest->get(query);
}

void Landing3::loadingBlock(QString blockName)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENTED YEAT!" << blockName;
}

void Landing3::chart(QString charType)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENTED YEAT!" << charType;
}

void Landing3::getLandingRequestHandler(QJsonValue value)
{
    QList<QObject*> blocks;

    for(const QJsonValue &vBlock: value.toObject().value("blocks").toArray()) {
        QJsonObject valueObject = vBlock.toObject();

        LandingResultBlock* block = new LandingResultBlock();
        block->type = valueObject.value("type").toString();
        block->title = valueObject.value("title").toString();
        block->description = valueObject.value("description").toString();

        QJsonArray entities = valueObject.value("entities").toArray();

        for(const QJsonValue &entitiesValue: entities) {
            QJsonObject entitiesObject = entitiesValue.toObject();
            if(block->type == "personal-playlists") {
                Playlist* playlist = new Playlist(entitiesObject.value("data").toObject().value("data").toObject());
                if(playlist != nullptr && !playlist->title().isEmpty()) {
                    block->entities.push_back(playlist);
                }
            }

            if(block->type == "new-releases") {
                if(entitiesObject.value("type").toString() == "album") { //Now support only albums
                    Album* album = new Album(entitiesObject.value("data").toObject());
                    if(album != nullptr && album->albumId() != 0) {
                        block->entities.push_back(album);
                    }
                }
            }
        }

        blocks.push_back(block);
    }

    emit landingBlocksReady(blocks);
}

void Landing3::loadingBlockRequestHandler(QJsonValue value)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENTED YEAT!" << value;
}

void Landing3::chartRequestHandler(QJsonValue value)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENTED YEAT!" << value;
}
