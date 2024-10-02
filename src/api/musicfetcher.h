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

#ifndef MUSICFETCHER_H
#define MUSICFETCHER_H

#include "../types/track.h"
#include "settings.h"

#include <QNetworkReply>
#include <QObject>

struct DownloadLink {
public:
    QString codec;
    bool gain;
    bool preview;
    QString downloadInfoUrl;
    bool direct;
    int bitrateInKbps;
};

class MusicFetcher : public QObject {
    Q_OBJECT
public:
    explicit MusicFetcher(QObject* parent = nullptr);
    Q_INVOKABLE void load(Track* track);

signals:
    void trackReady(QString path);
    void error(QString errorMessage);
    void downloadProgress(float progress);
//    void finalUrlReady(QString url);

private slots:
    void downloadInfoHandler(QJsonValue value);
    void downloadInfoUrlHandler();

    void dataReadyHandler(QNetworkReply* reply);
    void onDownloadProgress(qint64 bytesRead, qint64 bytesTotal);

private:
    Track* m_track;
    Settings* m_settings;
    QList<DownloadLink*> m_linksList;

    QString trackPath();
};

#endif // MUSICFETCHER_H
