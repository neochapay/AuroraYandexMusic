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

#ifndef ALBUMINFO_H
#define ALBUMINFO_H

#include <QObject>
#include <QJsonValue>

#include "../types/album.h"

class AlbumInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int albumId READ albumId WRITE setAlbumId NOTIFY albumIdChanged)

public:
    explicit AlbumInfo(QObject *parent = nullptr);
    Q_INVOKABLE void get(bool withTracks = true);

    int albumId() const;
    void setAlbumId(int newAlbumId);

signals:
    void albumIdChanged();
    void albumReady(Album* album);

private slots:
    void getAlbumRequestHandler(QJsonValue value);

private:
    int m_albumId;
};

#endif // ALBUMINFO_H
