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
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENED YEAT!" << blockName;
}

void Landing3::chart(QString charType)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENED YEAT!" << charType;
}

void Landing3::getLandingRequestHandler(QJsonValue value)
{
    QList<QObject*> blocks;

    for(const QJsonValue &v: value.toObject().value("blocks").toArray()) {
        LandingResultBlock* block = new LandingResultBlock;
        block->type = v.toObject().value("type").toString();
        block->title = v.toObject().value("titile").toString();
        block->description = v.toObject().value("description").toString();

        QJsonArray entities = value.toObject().value("blocks").toArray().first().toObject().value("entities").toArray();

        for(const QJsonValue &v: entities) {
            if(block->type == "personal-playlists") {
                Playlist* playlist = new Playlist(v.toObject().value("data").toObject().value("data").toObject());
                if(playlist != nullptr && !playlist->title().isEmpty()) {
                    block->entities.push_back(playlist);
                }
            }
        }

        blocks.push_back(block);
    }

    emit landingBlocksReady(blocks);
}

void Landing3::loadingBlockRequestHandler(QJsonValue value)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENED YEAT!" << value;
}

void Landing3::chartRequestHandler(QJsonValue value)
{
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENED YEAT!" << value;
}
