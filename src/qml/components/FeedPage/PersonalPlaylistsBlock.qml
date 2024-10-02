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

import "../"

Item {
    id: personalPlaylistsBlock
    height: childrenRect.height
    clip: true
    property var blockData

    Label{
        id: personalPlaylistsBlockTitle
        width: parent.width
        color: Theme.primaryColor
        font.pixelSize: Theme.fontSizeLarge
        font.bold: true
        horizontalAlignment: Text.AlignLeft
        text: personalPlaylistsBlock.blockData.title

        anchors{
            top: parent.top
            topMargin: Theme.paddingLarge
        }
    }

    ListView{
        id: generatedPlayListView
        width: parent.width

        anchors{
            top: personalPlaylistsBlockTitle.bottom
            topMargin: Theme.paddingLarge
        }
        model: personalPlaylistsBlock.blockData.entities
        orientation: ListView.Horizontal
        spacing: Theme.paddingLarge

        delegate: PlaylistCoverFeed{
            id: playlistCoverFeed
            title: modelData.title
            destription: modelData.description
            cover: modelData.cover.uri

            onClicked: pageStack.push(Qt.resolvedUrl("../../pages/PlaylistPage.qml"), {
                                          title: modelData.title,
                                          kind: modelData.kind,
                                          uid: modelData.ownerUid});
        }
    }
}
