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

import QtQuick 2.0
import Sailfish.Silica 1.0

import ru.neochapay.ourmusic 1.0

import "../components"

Page {
    id: playlistPage
    property alias title: header.title
    property int kind: -1
    property alias uid: playlists.userID

    Playlists{
        id: playlists
        onPlaylistChanged: {
            busyIndicator.visible = false
            console.log(playlist.tracks[0].title)
        }
    }

    Component.onCompleted: playlists.getUserList(playlistPage.kind)

    SilicaFlickable {
        id: playlistView
        anchors.fill: parent

        PageHeader {
            id: header
            title: qsTr("Playlist")
        }

        BusyIndicator {
            id: busyIndicator
            running: visible
            visible: true
            anchors.centerIn: parent
        }
    }
}
