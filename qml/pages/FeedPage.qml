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

import org.ilyavysotsky.yasailmusic 1.0

import "../components"

Page {
    id: mainPage

    Feed{
        id: feed
        onFeedReady: {
            busyIndicator.visible = false
            currentPlayListModel.push(tracksToPlay[0])
        }
    }

    Connections{
        target: user
        onUserIDChanged: feed.get()
    }

    SilicaFlickable {
        id: feedView
        anchors.fill: parent

        PageHeader {
            id: header
            title: qsTr("You feed")
        }

        BusyIndicator {
            id: busyIndicator
            running: visible
            visible: true
            anchors.centerIn: parent
        }

        Column{
            id: feedColumn
            anchors{
                top: header.bottom
                topMargin: Theme.paddingSmall
                left: parent.left
                leftMargin: Theme.paddingSmall
            }
            width: parent.width-Theme.paddingSmall*2
            height: parent.height-header.height-Theme.paddingSmall*2

            visible: !busyIndicator.visible
            spacing: Theme.paddingMedium

            Label{
                width: parent.width
                color: Theme.highlightColor
                text: qsTr("Collect for you")
                font.pixelSize: Theme.fontSizeExtraLarge
                horizontalAlignment: Text.AlignHCenter
            }

            ListView{
                id: generatedPlayListView
                width: parent.width
                height: Theme.itemSizeHuge
                model: feed.generatedPlaylists
                orientation: ListView.Horizontal
                spacing: Theme.paddingMedium

                delegate: PlaylistCoverFeed{
                    title: modelData.title
                    destription: modelData.description
                    cover: modelData.ogImage
                }
            }
        }
    }
}
