/*
 * Copyright (C) 2024 Chupligin Sergey <neochapay@gmail.com>
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
    id: userLikedPage

    SilicaFlickable {
        id: feedView
        anchors.fill: parent

        PageHeader {
            id: header
            title: qsTr("Liked tracks")
        }

        VerticalScrollDecorator {
            id: scroll
        }


        ListView{
            id: mainData
            width: parent.width - Theme.paddingMedium*2
            spacing: Theme.paddingMedium
            height: feedView.height - header.height - Theme.paddingMedium*2
            clip: true
            anchors{
                left: parent.left
                leftMargin: Theme.paddingMedium
                top: header.bottom
                topMargin: Theme.paddingMedium
            }

            delegate: TrackListItemDelegate{
                track: modelData
            }
        }
    }

    Component.onCompleted: mainData.model = user.likedTracks()
    Connections{
        target: user
        onLikedTracksChanged: mainData.model = user.likedTracks()
    }
}
