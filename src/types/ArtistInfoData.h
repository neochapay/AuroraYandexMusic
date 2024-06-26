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

#ifndef ARTISTINFODATA_H
#define ARTISTINFODATA_H

#include <QObject>

class ArtistInfoData : public QObject{
    Q_OBJECT
    Q_PROPERTY(QObject* artist MEMBER artist)
    Q_PROPERTY(QList<QObject*> popularTracks MEMBER popularTracks)
    Q_PROPERTY(QList<QObject*> albums MEMBER albums)

public:
    QObject* artist;
    QList<QObject*> popularTracks;
    QList<QObject*> albums;
    QList<QObject*> playlists;
    QList<QObject*> clips; //TODO
    QList<QObject*> similarArtists;
    QList<QObject*> concerts; //TODO
    QList<QObject*> urls; //TODO
};

#endif // ARTISTINFODATA_H
