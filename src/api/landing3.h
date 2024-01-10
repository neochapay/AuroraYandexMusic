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

#ifndef LANDING3_H
#define LANDING3_H

#include <QJsonValue>
#include <QObject>

#include "../types/LandingResultBlock.h"

class Landing3 : public QObject {
    Q_OBJECT
public:
    explicit Landing3(QObject* parent = nullptr);
    Q_INVOKABLE void get(QString blocks = "personalplaylists,promotions,new-releases,new-playlists,mixes,chart,artists,albums,playlists,play_contexts,podcasts");
    // Available values :  "personalplaylists,promotions,new-releases,new-playlists,mixes,chart,artists,albums,playlists,play_contexts,podcasts"
    Q_INVOKABLE void loadingBlock(QString blockName);
    // Available values : russia, world
    Q_INVOKABLE void chart(QString charType);

signals:
    void landingBlocksReady(QList<QObject*> blocks);

private slots:
    void getLandingRequestHandler(QJsonValue value);
    void loadingBlockRequestHandler(QJsonValue value);
    void chartRequestHandler(QJsonValue value);

private:
    QString m_gettingBlocks;
};

#endif // LANDING3_H
