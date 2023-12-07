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

import ru.neochapay.yandexmusic 1.0

import "../components"

Page {
    id: mainPage

    Feed{
        id: feed
        onErrorReady: {
            feedView.visible = false
            errorLabel.visible = true
        }
    }

    Component.onCompleted: {
        rotor.getStationTracks();
    }

    Connections{
        target: user
        onUserIDChanged: feed.get()
    }

    Connections{
        target: rotor
        onStantionTrackReady: {
            currentPlayListModel.push(track)
            busyIndicator.visible = false
        }
    }

    Label{
        id: errorLabel
        visible: false
        anchors.centerIn: parent
        text: qsTr("Oops. We have a problem.")
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
                topMargin: Theme.paddingLarge
                left: parent.left
                leftMargin: Theme.paddingLarge
            }
            width: parent.width-Theme.paddingLarge*2
            height: parent.height-header.height-Theme.paddingLarge*2

            visible: !busyIndicator.visible
            spacing: Theme.paddingLarge

            MyWavePlayer{
                id: myWavePlayer
            }

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
                spacing: Theme.paddingLarge

                delegate: PlaylistCoverFeed{
                    title: modelData.title
                    destription: modelData.description
                    cover: modelData.cover.uri
                }
            }
        }
    }
}
