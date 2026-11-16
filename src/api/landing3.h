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

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>

#include "../types/LandingResultBlock.h"

class Landing3 : public QObject {
    Q_OBJECT
public:
    explicit Landing3(QObject* parent = nullptr);

    // GET /landing3?blocks=...
    Q_INVOKABLE void get(const QString &blocks = QStringLiteral("personalplaylists,promotions,new-releases,new-playlists,mixes,chart,artists,albums,playlists,play_contexts,podcasts"));
    Q_INVOKABLE void getLanding(const QString &blocks = QString()); // alias

    // GET /landing3/{landingBlock} (new-playlists,new-releases,chart,podcasts)
    Q_INVOKABLE void getLandingBlock(const QString &blockName);
    Q_INVOKABLE void loadingBlock(const QString &blockName); // compatibility alias

    // Specific endpoints
    Q_INVOKABLE void getNewReleases();
    Q_INVOKABLE void getNewPodcasts();
    Q_INVOKABLE void getNewPlaylists();
    Q_INVOKABLE void getChart(const QString &chartType = QStringLiteral("russia")); // russia|world
    Q_INVOKABLE void chart(const QString &chartType); // compatibility alias

signals:
    void landingBlocksReady(QList<QObject*> blocks);
    void landingReady(const QJsonObject &result, const QList<QObject*> &blocks);
    void landingBlockReady(const QString &blockName, const QJsonValue &result);
    void newReleasesReady(const QJsonValue &result);
    void newPodcastsReady(const QJsonValue &result);
    void newPlaylistsReady(const QJsonValue &result);
    void chartReady(const QString &chartType, const QJsonValue &result);
    void error(const QString &message);

private slots:
    void getLandingRequestHandler(QJsonValue value);
    void loadingBlockRequestHandler(QJsonValue value);
    void newReleasesRequestHandler(QJsonValue value);
    void newPodcastsRequestHandler(QJsonValue value);
    void newPlaylistsRequestHandler(QJsonValue value);
    void chartRequestHandler(QJsonValue value);

private:
    QList<QObject*> parseBlocks(const QJsonObject &landingResult) const;
    QObject* parseEntity(const QString &blockType, const QJsonValue &entityValue) const;

    QString m_gettingBlocks;
    QString m_requestedBlock;
    QString m_requestedChartType;
};

#endif // LANDING3_H

