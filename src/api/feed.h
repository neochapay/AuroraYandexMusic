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

#ifndef FEED_H
#define FEED_H

#include "../types/playlist.h"
#include "../types/track.h"
#include "request.h"

#include <QJsonObject>
#include <QObject>

class Feed : public QObject {
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> generatedPlaylists READ generatedPlaylists NOTIFY feedReady)
    Q_PROPERTY(QList<QObject*> tracksToPlay READ tracksToPlay NOTIFY feedReady)

public:
    explicit Feed(QObject* parent = nullptr);
    Q_INVOKABLE void get();

    const QList<QObject*>& generatedPlaylists() const;
    const QList<QObject*>& tracksToPlay() const;

private slots:
    void getFeedHandler(QJsonObject object);

signals:
    void feedReady();

private:
    Request* m_getFeedApiRequest;
    QList<Playlist*> m_generatedPlaylists;
    QList<Track*> m_tracksToPlay;
};

#endif // FEED_H
